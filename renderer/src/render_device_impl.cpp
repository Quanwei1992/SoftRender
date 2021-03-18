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


inline int cross_ivec2(ivec2 a, ivec2 b)
{
	return a.x * b.y - a.y * b.x;
}



/*
* Top-Left Fill Rule ����˵��һ�����ظպ�ѹ�������α��ϵ�ʱ��ֻ���������������ε���ߣ����������ʱ�򣬲��ж�������ر������θ���(����ͼ)��
* �ϱߺܺ��жϣ������һ������ˮƽ�ģ��ұߵ��������㶼���ڵ��������㣬�����ϱߡ�
* ��Ϊ�����ζ���ķ�����һ���ģ�˳ʱ�������ʱ��)
* ����һ��������߻������ұ�ֻ�Ƚϱߵ���������ĸ߶ȼ��ɡ�
* ������ʱ��������Ϊ���棬
* ��ߵıߵĵ�һ�������һ���ȵڶ�������ߡ�
* ����ı�������������һ���ߣ����ǵ�һ������϶��ڵڶ��������ұߡ�
*/
inline bool isTopLeftEdge(ivec2 v1,ivec2 v2)
{
	return v2.y > v1.y || (v1.y == v2.y && v1.x > v2.x);
}

// �������������� 
// https://www.bilibili.com/video/BV1X7411F744?p=9 16:12
inline const vec3 barycentric(vec2 p, vec2 a, vec2 b, vec2 c)
{
	vec3 ret;
	ret.x = (-(p.x - b.x) * (c.y - b.y) + (p.y - b.y) * (c.x - b.x)) / (-(a.x - b.x) * (c.y - b.y) + (a.y - b.y) * (c.x - b.x));
	ret.y = (-(p.x - c.x) * (a.y - c.y) + (p.y - c.y) * (a.x - c.x)) / (-(b.x - c.x) * (a.y - c.y) + (b.y - c.y)*(a.x - c.x));
	ret.z = 1 - ret.x - ret.y;
	return ret;
}


bool renderer::render_device_impl::DrawTriangles(int index)
{
	const vertex_array_impl* vao = dynamic_cast<const vertex_array_impl*>(_vao);
	const array_buffer_impl* vbo = dynamic_cast<const array_buffer_impl*>(vao->GetArrayBuffer());
	const uint8_t* data = vbo->get_data();

	vertex _vertex[3];
	ivec2 _min,_max;

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

		// ���ж�����ɫ���򣬷��ض�������
		vertex& vertex = _vertex[i];
		_vertex_shader->main();
		vertex.pos = _vertex_shader->out_position;

		// �򵥲ü����κ�һ�����㳬�� CVV ���޳�
		float w = vertex.pos.w;
		if (w == 0.0f) return false;
		// ����ͼ�򵥣���һ����Խ�磬����������������Σ�����ϸ��������
		// ���Խ���˾�����οռ��ڽ��вü������Ϊ 0-2 ��������Ȼ�����
		if (vertex.pos.z < 0.0f || vertex.pos.z > w) return false;
		if (vertex.pos.x < -w || vertex.pos.x > w) return false;
		if (vertex.pos.y < -w || vertex.pos.y > w) return false;

		// ���� w �ĵ�����Reciprocal of the Homogeneous W 
		vertex.rhw = 1.0f / w;

		// �������ռ� /w ��һ������λ��� cvv
		vertex.pos *= vertex.rhw;

		// ������Ļ����
		vertex.spf.x = (vertex.pos.x + 1.0f) * _ctx->width * 0.5f;
		vertex.spf.y = (1 - vertex.pos.y) * _ctx->height * 0.5f;
		// ������Ļ���꣺�� 0.5 ��ƫ��ȡ��Ļ���ط������Ķ���
		vertex.spi.x = (int)(vertex.spf.x + 0.5f);
		vertex.spi.y = (int)(vertex.spf.y + 0.5f);

		// ����������AABB
		if (i == 0)
		{
			_min = _max = vertex.spi;
		}
		else {
			if (vertex.spi.x < _min.x) _min.x = vertex.spi.x;
			if (vertex.spi.y < _min.y) _min.y = vertex.spi.y;
			if (vertex.spi.x > _max.x) _max.x = vertex.spi.x;
			if (vertex.spi.y > _max.y) _max.y = vertex.spi.y;
		}
	}

	_min = glm::clamp(_min, 0, _ctx->width - 1);
	_max = glm::clamp(_max, 0, _ctx->width - 1);

	// �����߿�
	//DrawLine(_vertex[0].spi.x, _vertex[0].spi.y, _vertex[1].spi.x, _vertex[1].spi.y);
	//DrawLine(_vertex[0].spi.x, _vertex[0].spi.y, _vertex[2].spi.x, _vertex[2].spi.y);
	//DrawLine(_vertex[2].spi.x, _vertex[2].spi.y, _vertex[1].spi.x, _vertex[1].spi.y);

	// �ж������γ���
	vec3 v01 = _vertex[1].pos.xyz - _vertex[0].pos.xyz;
	vec3 v02 = _vertex[2].pos.xyz - _vertex[1].pos.xyz;
	vec3 normal = glm::cross(v01, v02);
	if (normal.z == 0) return false;

	// ʹ�� vtx �����������㣬����ֱ���� _vertex ���ʣ���Ϊ���ܻ����˳��
	vertex* vtx[3] = { &_vertex[0],&_vertex[1],&_vertex[2] };

	// ����Ǳ��棬�򽻻����㣬��֤ edge equation �жϵķ���Ϊ��
	if (normal.z < 0.0f) {
		vtx[1] = & _vertex[2];
		vtx[2] = & _vertex[1];
	}

	// ���������˵�λ��
	ivec2 p0 = vtx[0]->spi;
	ivec2 p1 = vtx[1]->spi;
	ivec2 p2 = vtx[2]->spi;


	// ���������Ϊ����˳�
	int s = glm::abs(cross_ivec2(p1 - p0, p2 - p1));
	if (s <= 0) return false;

	// top-left rul 
	// https://www.zhihu.com/question/34100482/answer/58424576
	// ���������ʱ�����������ı��ϵĵ���Ҫ�������ҷ����·����ϵĵ㲻����
	// ���ж��Ƿ��� TopLeft���жϳ��������·� Edge Equation һ�����
	bool TopLeft01 = isTopLeftEdge(p0, p1);
	bool TopLeft12 = isTopLeftEdge(p1, p2);
	bool TopLeft20 = isTopLeftEdge(p2, p0);

	// ������������Ӿ��ε����е�
	for (int cy = _min.y;cy<=_max.y;cy++)
	{
		for (int cx = _min.x;cx<=_max.x;cx++)
		{
			ivec2 pxi(cx, cy);
			vec2 pxf(cx, cy);

			// Edge Equation
			// ʹ���������⸡�����
			int E01 = cross_ivec2(pxi - p0, p1 - p0);
			int E12 = cross_ivec2(pxi - p1, p2 - p1);
			int E20 = cross_ivec2(pxi - p2, p0 - p2);

			if (E01 < (TopLeft01 ? 0 : 1)) continue;   // �ڵ�һ���ߺ���
			if (E12 < (TopLeft12 ? 0 : 1)) continue;   // �ڵڶ����ߺ���
			if (E20 < (TopLeft20 ? 0 : 1)) continue;   // �ڵ������ߺ���


			_fragment_shader->main();
			SetPixel(cx, cy, _fragment_shader->out_frag_color);
		}
	}



	


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

