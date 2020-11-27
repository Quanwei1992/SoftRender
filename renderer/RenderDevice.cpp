#include "RenderDevice.h"
#include "Shader.h"
#include "platform/window.h"
#include <cassert>

using namespace Eigen;

RenderDevice::RenderDevice(Window* context)
	:_width(0)
	,_height(0)
	,_clearColor(0)
	,_vertexShader(nullptr)
	,_fragmentShader(nullptr)
{
	_width = context->GetWidth();
	_height = context->GetHeight();
	unsigned char* backbuffer = context->GetBackbuffer();
	_framebuffer.resize(_height);
	for (int j =0;j<_height;j++)
	{
		this->_framebuffer[j] = (uint32_t*)(backbuffer + _width * 4 * j);
	}
}

void RenderDevice::Clear()
{
	for (int y =0;y<_height;y++)
	{
		uint32_t* dst = _framebuffer[y];
		for (int x = 0;x<_width;dst++,x++)
		{
			*dst = _clearColor;
		}
	}
}

void RenderDevice::SetClearColor(uint32_t color)
{
	_clearColor = color;
}

void RenderDevice::SetVertices(const std::vector<float>& vertices)
{
	_vertices = vertices;
}



void RenderDevice::SetIndices(const std::vector<uint32_t>& indices)
{
	_indices = indices;
}

void RenderDevice::SetVertexShader(IVertexShader* shader)
{
	_vertexShader = shader;
}

void RenderDevice::SetFragmentShader(IFragmentShader* shader)
{
	_fragmentShader = shader;
}

void RenderDevice::Draw(EPrimitive type)
{
	assert(_vertexShader);
	assert(_fragmentShader);

	switch (type)
	{
	case EPrimitive::POINTS:
		DrawPoints();
		break;
	case EPrimitive::TRIANGLES:
		DrawTriangles();
		break;
	case EPrimitive::LINE_STRIP:
		DrawLineStrip();
		break;
	default:
		break;
	}
}


void RenderDevice::DrawPixel(int x, int y, const Eigen::Vector4f& color)
{
	uint32_t finalColor = (uint32_t)(color.z() * 255);
	finalColor |= (uint32_t)(color.y() * 255) << 8;
	finalColor |= (uint32_t)(color.x() * 255) << 16;
	finalColor |= (uint32_t)(color.w() * 255) << 24;
	uint32_t* dst = _framebuffer[y] + x;
	*dst = finalColor;
}

void RenderDevice::DrawPixel(const Eigen::Vector3f& screenPos, const Eigen::Vector4f& color)
{
	DrawPixel((int)(screenPos.x() + 0.5f), (int)(screenPos.y() + 0.5f),color);
}

void RenderDevice::DrawLine(float x0, float y0, float x1, float y1)
{
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	float dx = x1 - x0;
	float dy = y1 - y0;
	float derror2 = std::abs(dy) * 2;
	float error2 = 0;
	float y = y0;
	float ddx = 1.0f / _width;
	float ddy = 1.0f / _height;
	
	PSIn in;
	for (float x = x0; x <= x1; x += ddx) {
		if (steep) {
			in.Position = Vector3f((float)y, (float)x, 0);
			DrawPixel(ViewportTransform(in.Position), _fragmentShader->Main(in).FragColor);
		}
		else {
			in.Position = Vector3f((float)x, (float)y, 0);
			DrawPixel(ViewportTransform(in.Position), _fragmentShader->Main(in).FragColor);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? ddy : -ddy);
			error2 -= dx * 2;
		}
	}
}

void RenderDevice::DrawLine(const Eigen::Vector3f& from, const Eigen::Vector3f& to)
{
	const auto& a = ViewportTransform(from);
	const auto& b = ViewportTransform(to);

	int x0 = (int)a.x();
	int y0 = (int)a.y();
	int x1 = (int)b.x();
	int y1 = (int)b.y();
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;

	PSIn in;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			in.Position = InvViewportTransform(Vector3f((float)y, (float)x, 0));
			DrawPixel(y,x, _fragmentShader->Main(in).FragColor);
		}
		else {
			in.Position = InvViewportTransform(Vector3f((float)x, (float)y, 0));
			DrawPixel(x,y, _fragmentShader->Main(in).FragColor);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

Eigen::Vector3f RenderDevice::ViewportTransform(const Vector3f& in)
{
	Vector3f out;
	out.x() = (in.x() + 1) * 0.5f * _width;
	out.y() = (1 - in.y()) * 0.5f * _height;
	out.z() = in.z();
	return out;
}

Eigen::Vector3f RenderDevice::InvViewportTransform(const Eigen::Vector3f& in)
{
	Vector3f out;
	out.x() = in.x() / (0.5f - _width) - 1;
	out.y() = 1 + in.y() / (0.5f - _height);
	out.z() = in.z();
	return out;
}

void RenderDevice::DrawLineStrip()
{
	std::vector<VSIn> vsin(3);
	std::vector<VSOut> vsout(3);
	PSIn psin;
	for (size_t i =0;i< _vertices.size();i+=9)
	{
		vsin[0].Position = Vector3f(_vertices[i], _vertices[i + 1], _vertices[i + 2]);
		vsin[1].Position = Vector3f(_vertices[i+3], _vertices[i + 4], _vertices[i +5]);
		vsin[2].Position = Vector3f(_vertices[i+6], _vertices[i + 7], _vertices[i + 8]);

		for (int j =0;j<3;j++)
		{
			vsout[j] = _vertexShader->Main(vsin[j]);
		}
	
		DrawLine(vsout[0].Position, vsout[1].Position);
		DrawLine(vsout[0].Position, vsout[2].Position);
		DrawLine(vsout[1].Position, vsout[2].Position);
	

	}
}

void RenderDevice::DrawTriangles()
{

}

void RenderDevice::DrawPoints()
{
	VSIn vsin;
	PSIn psin;
	for (size_t i =0;i<_vertices.size();i+=3)
	{

		// Vertex Shader
		vsin.Position = Vector3f(_vertices[i], _vertices[i + 1], _vertices[i + 2]);
		const auto& vout = _vertexShader->Main(vsin);


		// Clip

		if(vout.Position.x() < -1 || vout.Position.x() >1) continue;
		if(vout.Position.y() < -1 || vout.Position.y() >1) continue;
		if(vout.Position.z() < -1 || vout.Position.z() >1) continue;

		// Fragment Shader

		psin.Position = vout.Position;
		const auto& pout = _fragmentShader->Main(psin);


		const auto& screenPos = ViewportTransform(psin.Position);

		DrawPixel((int)(screenPos.x()), (int)(screenPos.y()), pout.FragColor);
	}
}
