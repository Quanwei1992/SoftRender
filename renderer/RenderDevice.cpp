#include "RenderDevice.h"
#include "Shader.h"
#include "platform/window.h"
#include <cassert>
#include <limits>

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

void RenderDevice::SetVertices(const std::vector<Vertex>& vertices)
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

void RenderDevice::DrawPixel(const Eigen::Vector4f& screenPos, const Eigen::Vector4f& color)
{
	DrawPixel((int)(screenPos.x() + 0.5f), (int)(screenPos.y() + 0.5f),color);
}

void RenderDevice::DrawLine(float x0, float y0, float x1, float y1)
{
	//bool steep = false;
	//if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
	//	std::swap(x0, y0);
	//	std::swap(x1, y1);
	//	steep = true;
	//}
	//if (x0 > x1) {
	//	std::swap(x0, x1);
	//	std::swap(y0, y1);
	//}
	//float dx = x1 - x0;
	//float dy = y1 - y0;
	//float derror2 = std::abs(dy) * 2;
	//float error2 = 0;
	//float y = y0;
	//float ddx = 1.0f / _width;
	//float ddy = 1.0f / _height;
	//
	//PSIn in;
	//for (float x = x0; x <= x1; x += ddx) {
	//	if (steep) {
	//		in.Position = Vector4f((float)y, (float)x, 0,1);
	//		DrawPixel(ViewportTransform(in.Position), _fragmentShader->Main(in).FragColor);
	//	}
	//	else {
	//		in.Position = Vector4f((float)x, (float)y, 0,1);
	//		DrawPixel(ViewportTransform(in.Position), _fragmentShader->Main(in).FragColor);
	//	}
	//	error2 += derror2;
	//	if (error2 > dx) {
	//		y += (y1 > y0 ? ddy : -ddy);
	//		error2 -= dx * 2;
	//	}
	//}
}

//void RenderDevice::DrawLine(const Eigen::Vector4f& from, const Eigen::Vector4f& to)
//{
//	const auto& a = ViewportTransform(from);
//	const auto& b = ViewportTransform(to);
//
//	int x0 = (int)a.x();
//	int y0 = (int)a.y();
//	int x1 = (int)b.x();
//	int y1 = (int)b.y();
//	bool steep = false;
//	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
//		std::swap(x0, y0);
//		std::swap(x1, y1);
//		steep = true;
//	}
//	if (x0 > x1) {
//		std::swap(x0, x1);
//		std::swap(y0, y1);
//	}
//	int dx = x1 - x0;
//	int dy = y1 - y0;
//	int derror2 = std::abs(dy) * 2;
//	int error2 = 0;
//	int y = y0;
//
//	PSIn in;
//	for (int x = x0; x <= x1; x++) {
//		if (steep) {
//			in.Position = InvViewportTransform(Vector4f((float)y, (float)x, 0, 1));
//			DrawPixel(y, x, _fragmentShader->Main(in).FragColor);
//		}
//		else {
//			in.Position = InvViewportTransform(Vector4f((float)x, (float)y, 0, 1));
//			DrawPixel(x, y, _fragmentShader->Main(in).FragColor);
//		}
//		error2 += derror2;
//		if (error2 > dx) {
//			y += (y1 > y0 ? 1 : -1);
//			error2 -= dx * 2;
//		}
//	}
//}

Eigen::Vector4f RenderDevice::ViewportTransform(const Vector4f& in)
{
	float rhw =  1.0f / in.w();
	Vector4f out;
	out.x() = (in.x() * rhw + 1) * 0.5f * _width;
	out.y() = (1 - in.y() * rhw) * 0.5f * _height;
	out.z() = in.z() * rhw;
	out.w() = 1.0f;
	return out;
}

Eigen::Vector4f RenderDevice::InvViewportTransform(const Eigen::Vector4f& in)
{
	Vector4f out;
	out.x() = in.x() / (0.5f - _width) - 1;
	out.y() = 1 + in.y() / (0.5f - _height);
	out.z() = in.z();
	out.w() = in.w();
	return out;
}

void RenderDevice::DrawLineStrip()
{
	//std::vector<VSIn> vsin(3);
	//std::vector<VSOut> vsout(3);
	//PSIn psin;
	//for (size_t i =0;i< _vertices.size();i+=9)
	//{
	//	vsin[0].Position = Vector4f(_vertices[i], _vertices[i + 1], _vertices[i + 2], 1);
	//	vsin[1].Position = Vector4f(_vertices[i+3], _vertices[i + 4], _vertices[i +5], 1);
	//	vsin[2].Position = Vector4f(_vertices[i+6], _vertices[i + 7], _vertices[i + 8], 1);

	//	for (int j =0;j<3;j++)
	//	{
	//		vsout[j] = _vertexShader->Main(vsin[j]);
	//	}
	//
	//	DrawLine(vsout[0].Position, vsout[1].Position);
	//	DrawLine(vsout[0].Position, vsout[2].Position);
	//	DrawLine(vsout[1].Position, vsout[2].Position);
	//

	//}
}


inline Vector4f toVec4(const Vector3f& in)
{
	return Vector4f(in.x(), in.y(), in.z(),1);
}


inline int checkCVV(const Vector4f& v)
{
	float w = v.w();
	int check = 0;
	if (v.z() < 0.0f)check |= 1;
	if (v.z() > w) check |= 2;
	if (v.x() < -w) check |= 4;
	if (v.x() > w) check |= 8;
	if (v.y() < -w) check |= 16;
	if (v.y() > w) check |= 32;
	return check;
}

void RenderDevice::DrawTriangles()
{

	for (size_t i=0;i<_indices.size();i+=3)
	{
		DrawTriangle(
			_vertices[_indices[i]],
			_vertices[_indices[i+1]],
			_vertices[_indices[i+2]]
		);
	}
}

void RenderDevice::DrawPoints()
{
	//VSIn vsin;
	//PSIn psin;
	//for (size_t i =0;i<_vertices.size();i+=3)
	//{

	//	// Vertex Shader
	//	vsin.Position = Vector4f(_vertices[i], _vertices[i + 1], _vertices[i + 2],1);
	//	const auto& vout = _vertexShader->Main(vsin);


	//	// Clip

	//	if(vout.Position.x() < -1 || vout.Position.x() >1) continue;
	//	if(vout.Position.y() < -1 || vout.Position.y() >1) continue;
	//	if(vout.Position.z() < -1 || vout.Position.z() >1) continue;

	//	// Fragment Shader

	//	psin.Position = vout.Position;
	//	const auto& pout = _fragmentShader->Main(psin);


	//	const auto& screenPos = ViewportTransform(psin.Position);

	//	DrawPixel((int)(screenPos.x()), (int)(screenPos.y()), pout.FragColor);
	//}
}

inline bool insideTriangle(int x, int y, const std::vector<Vector4f>& points)
{
	Vector3f Q(x + 0.5f, y + 0.5f,1.0f);
	Vector3f P0 = points[0].head<3>();
	Vector3f P1 = points[1].head<3>();
	Vector3f P2 = points[2].head<3>();

	Vector3f v0 = P1 - P0;
	Vector3f v1 = P2 - P1;
	Vector3f v2 = P0 - P2;

	Vector3f c0 = Q - P0;
	Vector3f c1 = Q - P1;
	Vector3f c2 = Q - P2;

	Vector3f a = c0.cross(v0);
	Vector3f b = c1.cross(v1);
	Vector3f c = c2.cross(v2);

	if (a.z() > 0 && b.z() > 0 && c.z() > 0) return true;
	if (a.z() < 0 && b.z() < 0 && c.z() < 0) return true;
	return false;
}

inline Vector3f barycentric(float x, float y, const std::vector<Vector4f>& v)
{
	float c1 = (x * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * y + v[1].x() * v[2].y() - v[2].x() * v[1].y()) / (v[0].x() * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * v[0].y() + v[1].x() * v[2].y() - v[2].x() * v[1].y());
	float c2 = (x * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * y + v[2].x() * v[0].y() - v[0].x() * v[2].y()) / (v[1].x() * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * v[1].y() + v[2].x() * v[0].y() - v[0].x() * v[2].y());
	float c3 = (x * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * y + v[0].x() * v[1].y() - v[1].x() * v[0].y()) / (v[2].x() * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * v[2].y() + v[0].x() * v[1].y() - v[1].x() * v[0].y());
	return { c1,c2,c3 };
}



inline Vector3f interpolate(const Vector3f& barycentric, const Vector3f& a, const Vector3f& b, const Vector3f& c)
{
	return a * barycentric.x() + b * barycentric.y() + c * barycentric.z();
}
inline Vector4f interpolate(const Vector3f& barycentric, const Vector4f& a, const Vector4f& b, const Vector4f& c)
{
	return a * barycentric.x() + b * barycentric.y() + c * barycentric.z();
}
inline Vector2f interpolate(const Vector3f& barycentric, const Vector2f& a, const Vector2f& b, const Vector2f& c)
{
	return a * barycentric.x() + b * barycentric.y() + c * barycentric.z();
}




void RenderDevice::DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	std::vector<VSOut> vsout(3);
	std::vector<Vertex> verties = {
		v1,v2,v3
	};
	VSIn in;
	for (size_t i = 0;i<3;i++)
	{	
		in.Position = toVec4(verties[i].Position);
		vsout[i] = _vertexShader->Main(in);
	}

	if (checkCVV(vsout[0].Position) != 0) return;
	if (checkCVV(vsout[1].Position) != 0) return;
	if (checkCVV(vsout[2].Position) != 0) return;

	const Vector4f& p1 = ViewportTransform(vsout[0].Position);
	const Vector4f& p2 = ViewportTransform(vsout[1].Position);
	const Vector4f& p3 = ViewportTransform(vsout[2].Position);

	std::vector<Vector4f> points = {
		p1,p2,p3
	};

	// find out the bounding box of the triangle
	int minx, maxx, miny, maxy;
	minx = maxx = (int)points[0].x();
	miny = maxy = (int)points[0].y();
	for (size_t i =1;i<3;i++)
	{
		int x = (int)points[i].x();
		int y = (int)points[i].y();
		if (x < minx) minx = x;
		if (x > maxx) maxx = x;
		if (y < miny) miny = y;
		if (y > maxy) maxy = y;
	}

	PSIn psin;
	for (int x = minx;x<=maxx;x++)
	{
		for (int y = miny;y<=maxy;y++)
		{
			if (!insideTriangle(x, y, points)) {
				continue;
			}
			const Vector3f& b = barycentric(x + 0.5f, y + 0.5f, points);
			//psin.Position = interpolate(b, vsout.pos, p2, p3);
			psin.Color = interpolate(b, v1.Color, v2.Color, v3.Color);
			const auto& psout = _fragmentShader->Main(psin);
			DrawPixel(x, y,psout.FragColor);
		}
	}

	
}
