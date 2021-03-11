#pragma once
#include <cstdint>
#include <vector>

#include "Shader.h"
#include <glm/glm.hpp>
class Window;
class IVertexShader;

enum class EPrimitive {
	POINTS,
	TRIANGLES,
	LINE_STRIP
};


struct Vertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 Texcoords;
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
	void DrawPixel(int x, int y, const glm::vec4& color);
	void DrawPixel(const glm::vec4& screenPos, const glm::vec4& color);
	void DrawLine(float x0, float y0, float x1, float y1);
	glm::vec4 ViewportTransform(const glm::vec4& in);
	glm::vec4 InvViewportTransform(const glm::vec4& in);

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