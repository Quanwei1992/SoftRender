#include "render_device_impl.h"
#include "render_context.h"
#include "vertex_array_impl.h"
#include "vertex_buffer_impl.h"
#include "renderer/vertex_shader.h"
#include "renderer/fragment_shader.h"
#include "renderer_untils.hpp"
#include <cassert>

using namespace renderer;
using namespace glm;

struct vertex
{
	vec4 pos;
	float rhw;
	vec2 spf;
	ivec2 spi;
};


render_device_impl::render_device_impl(render_context* ctx)
	:_ctx(ctx)
	,_clear_color(0)
	,_fragment_shader(nullptr)
	,_vertex_shader(nullptr)
	,_vao(nullptr)
{
	assert(ctx != nullptr);

	_framebuffer.resize(ctx->height);
	for (int j = 0; j < ctx->height; j++)
	{
		_framebuffer[j] = (uint32_t*)(ctx->framebuffer + ctx->width * 4 * j);
	}
}

render_device_impl::~render_device_impl()
{
	release_context(_ctx);
	_ctx = nullptr;
}

void render_device_impl::Clear(uint8_t mask)
{

	if (mask & (uint8_t)clear_type::COLOR_BUFFER_BIT)
	{
		for (int y = 0; y < _ctx->height; y++)
		{
			for (int x = 0; x < _ctx->width;x++)
			{
				SetPixel(x, y, _clear_color);
			}
		}
	}
}

void render_device_impl::ClearColor(float r, float g, float b, float a)
{
	_clear_color = glm::vec4(r, g, b, a);
}

void render_device_impl::SwapBuffer()
{
	swap_buffer(_ctx);
}

void render_device_impl::SetPixel(int x, int y, const::glm::vec4& color)
{
	uint32_t finalColor = (uint32_t)(color.b * 255);
	finalColor |= (uint32_t)(color.g * 255) << 8;
	finalColor |= (uint32_t)(color.r * 255) << 16;
	finalColor |= (uint32_t)(color.a * 255) << 24;
	uint32_t* dst = _framebuffer[y] + x;
	*dst = finalColor;
}

void renderer::render_device_impl::UseVertexShader(const vertex_shader* shader)
{
	_vertex_shader = shader;
}

void renderer::render_device_impl::UseFragmentShader(const fragment_shader* shader)
{
	_fragment_shader = shader;
}

void renderer::render_device_impl::UseVertexArray(const vertex_array* vao)
{
	_vao = vao;
}

void renderer::render_device_impl::DrawArrays(draw_mode mode, int first, int count)
{
	assert(_vertex_shader);
	assert(_fragment_shader);
	assert(_vao);


	if (mode == draw_mode::TRIANGLES)
	{
		for (int i = first; i < count; i+=3)
		{
			DrawTriangles(i);
		}
	}
}

bool renderer::render_device_impl::DrawTriangles(int index)
{
	const vertex_array_impl* vao = dynamic_cast<const vertex_array_impl*>(_vao);
	const array_buffer_impl* vbo = dynamic_cast<const array_buffer_impl*>(vao->GetArrayBuffer());
	const uint8_t* data = vbo->get_data();

	vertex _vertex[3];

	// initialize vertex
	for (int i = 0; i < 3; i++)
	{
		int num_attr = _vertex_shader->GetAttributesCount();
		for (int j = 0; j < num_attr; j++)
		{
			const input_vertex_arrtibute& ivr = _vertex_shader->GetAttribute(j);
			const vertex_attrib& vr = vao->GetVertexAttrib(ivr.location);
			assert(vr.enable);
			assert(vr.type == ivr.type);
			assert(vr.size == ivr.size);
			memcpy(ivr.address, data + (index + i) * vr.stride, vr.size * get_size_by_data_type(vr.type));
		}

		// 运行顶点着色程序，返回顶点坐标
		vertex& vertex = _vertex[i];
		_vertex_shader->main();
		vertex.pos = _vertex_shader->out_position;

		// 简单裁剪，任何一个顶点超过 CVV 就剔除
		float w = vertex.pos.w;
		if (w == 0.0f) return false;
		// 这里图简单，当一个点越界，立马放弃整个三角形，更精细的做法是
		// 如果越界了就在齐次空间内进行裁剪，拆分为 0-2 个三角形然后继续
		if (vertex.pos.z < 0.0f || vertex.pos.z > w) return false;
		if (vertex.pos.x < -w || vertex.pos.x > w) return false;
		if (vertex.pos.y < -w || vertex.pos.y > w) return false;

		// 计算 w 的倒数：Reciprocal of the Homogeneous W 
		vertex.rhw = 1.0f / w;

		// 齐次坐标空间 /w 归一化到单位体积 cvv
		vertex.pos *= vertex.rhw;

		// 计算屏幕坐标
		vertex.spf.x = (vertex.pos.x + 1.0f) * _ctx->width * 0.5f;
		vertex.spf.y = (1 - vertex.pos.y) * _ctx->height * 0.5f;
		// 整数屏幕坐标：加 0.5 的偏移取屏幕像素方格中心对齐
		vertex.spi.x = (int)(vertex.spf.x + 0.5f);
		vertex.spi.y = (int)(vertex.spf.y + 0.5f);
		// 更新三角形AABB
	}

	// 绘制线框
	DrawLine(_vertex[0].spi.x, _vertex[0].spi.y, _vertex[1].spi.x, _vertex[1].spi.y);
	DrawLine(_vertex[0].spi.x, _vertex[0].spi.y, _vertex[2].spi.x, _vertex[2].spi.y);
	DrawLine(_vertex[2].spi.x, _vertex[2].spi.y, _vertex[1].spi.x, _vertex[1].spi.y);

	// 判断三角形朝向
	
	// 如果背向视点，则交换顶点，保证 edge equation 判断的符号为正
	
	// 保存三个端点位置
	
	// 计算面积，为零就退出
	
	// 三角形填充时，左面和上面的边上的点需要包括，右方和下方边上的点不包括
	// 先判断是否是 TopLeft，判断出来后会和下方 Edge Equation 一起决策

	// 迭代三角形外接矩形的所有点

	// Edge Equation
	// 使用整数避免浮点误差，同时因为是左手系，所以符号取反


	return true;

}

void renderer::render_device_impl::DrawLine(int x1, int y1, int x2, int y2)
{
	glm::vec4 color(1, 1, 1, 1);
	int x, y;
	if (x1 == x2 && y1 == y2) {
		SetPixel(x1, y1, color);
		return;
	}
	else if (x1 == x2) {
		int inc = (y1 <= y2) ? 1 : -1;
		for (y = y1; y != y2; y += inc) SetPixel(x1, y, color);
		SetPixel(x2, y2, color);
	}
	else if (y1 == y2) {
		int inc = (x1 <= x2) ? 1 : -1;
		for (x = x1; x != x2; x += inc) SetPixel(x, y1, color);
		SetPixel(x2, y2, color);
	}
	else {
		int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
		int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
		int rem = 0;
		if (dx >= dy) {
			if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; x <= x2; x++) {
				SetPixel(x, y, color);
				rem += dy;
				if (rem >= dx) { rem -= dx; y += (y2 >= y1) ? 1 : -1; SetPixel(x, y, color); }
			}
			SetPixel(x2, y2, color);
		}
		else {
			if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; y <= y2; y++) {
				SetPixel(x, y, color);
				rem += dx;
				if (rem >= dy) { rem -= dy; x += (x2 >= x1) ? 1 : -1; SetPixel(x, y, color); }
			}
			SetPixel(x2, y2, color);
		}
	}
}

