#pragma once
#include <cstdint>
#include <vector>

#include "Shader.h"

class Window;
class IVertexShader;

enum class EPrimitive {
	POINTS,
	TRIANGLES,
	LINE_STRIP
};


struct Vertex
{
	Eigen::Vector3f Position;
	Eigen::Vector4f Color;
	Eigen::Vector2f Texcoords;
};


class RenderDevice
{
public:
	RenderDevice(Window* context);
	void Clear();
	void SetClearColor(uint32_t color);
	void SetVertices(const std::vector<Vertex>& vertices);
	void SetIndices(const std::vector<uint32_t>& indices);
	void SetVertexShader(IVertexShader* shader);
	void SetFragmentShader(IFragmentShader* shader);
	void Draw(EPrimitive type);
private:
	void DrawPixel(int x, int y, const Eigen::Vector4f& color);
	void DrawPixel(const Eigen::Vector4f& screenPos, const Eigen::Vector4f& color);
	void DrawLine(float x0, float y0, float x1, float y1);
	Eigen::Vector4f ViewportTransform(const Eigen::Vector4f& in);
	Eigen::Vector4f InvViewportTransform(const Eigen::Vector4f& in);

	void DrawLineStrip();
	void DrawTriangles();
	void DrawPoints();
	void DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

private:
	std::vector<uint32_t*> _framebuffer;
	int _width;
	int _height;
	uint32_t _clearColor;
	std::vector<Vertex> _vertices;
	std::vector<uint32_t> _indices;
	IVertexShader* _vertexShader;
	IFragmentShader* _fragmentShader;
};