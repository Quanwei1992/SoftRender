#include "Rasterizer.h"
#include "TGAImage.h"
using namespace Eigen;

Rasterizer::Rasterizer(int width, int height)
	:_width(width)
	,_height(height)
{
	size_t size = (size_t)(width * height);
	_framebuf.resize(size);
	_depthbuf.resize(size);
	_view = Matrix4f::Identity();
	_model = Matrix4f::Identity();
	_projection = Matrix4f::Identity();
}

void Rasterizer::SetPositions(const std::vector<Eigen::Vector3f>& positions)
{
	_positions = positions;
}

void Rasterizer::SetIndices(const std::vector<unsigned int>& indices)
{
	_indices = indices;
}

void Rasterizer::SetModel(const Eigen::Matrix4f& m)
{
	_model = m;
}

void Rasterizer::SetView(const Eigen::Matrix4f& m)
{
	_view = m;
}

void Rasterizer::SetProjection(const Eigen::Matrix4f& m)
{
	_projection = m;
}

void Rasterizer::Clear(EBuffer buff)
{
	if ((buff & EBuffer::Color) == EBuffer::Color)
	{
		std::fill(_framebuf.begin(), _framebuf.end(), Vector3f(0, 0, 0));
	}
	if ((buff & EBuffer::Depth) == EBuffer::Depth)
	{
		std::fill(_depthbuf.begin(), _depthbuf.end(), std::numeric_limits<float>::infinity());
	}
}


auto to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
	return Vector4f(v3.x(), v3.y(), v3.z(), w);
}




void Rasterizer::Draw(EPrimitive type)
{
	Matrix4f mvp = _projection * _view * _model;
	for (int i =0;i<_indices.size();i+=3)
	{
		// to clip space
		Vector4f v[] = {
			mvp * to_vec4(_positions[_indices[i]]),
			mvp * to_vec4(_positions[_indices[i+1]]),
			mvp * to_vec4(_positions[_indices[i+2]])
		};

		// to NDC space
		for (auto& vec : v)
		{
			vec /= vec.w();
		}

		// to screen space
		for (auto& vert : v)
		{
			vert.x() = (vert.x() + 1) * 0.5f * _width;
			vert.y() = (vert.y() + 1) * 0.5f * _height;
		}
		const Vector3f color(1.0f, 1.0f, 1.0f);
		DrawLine((int)v[0].x(), (int)v[0].y(), (int)v[1].x(), (int)v[1].y(), color);
		DrawLine((int)v[0].x(), (int)v[0].y(), (int)v[2].x(), (int)v[2].y(), color);
		DrawLine((int)v[1].x(), (int)v[1].y(), (int)v[2].x(), (int)v[2].y(), color);
	}
}

void Rasterizer::SaveFrameBufferToTGA(const std::string& filename) const
{
	TGAImage image(_width, _height, TGAImage::Format::RGB);
	for (int x=0;x<_width;x++)
	{
		for (int y=0;y<_height;y++)
		{
			auto& color = _framebuf[y * _width + x];	
			image.set(x, y, TGAColor(color.x() * 255, color.y() * 255, color.z() * 255, 255));
		}
	}
	image.flip_vertically();
	image.write_tga_file(filename.c_str());
}

void Rasterizer::DrawLine(int x0, int y0, int x1, int y1, const Eigen::Vector3f& color)
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
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			SetPixel(y, x, color);
		}
		else {
			SetPixel(x, y, color);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void Rasterizer::SetPixel(int x, int y, const Eigen::Vector3f& color)
{
	size_t index = y * _width + x;
	if (index < _framebuf.size()) {
		_framebuf[index] = color;
	}
}

