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


class RenderDevice
{
public:
	RenderDevice(Window* context);
	void Clear();
	void SetClearColor(uint32_t color);
	void SetVertices(const std::vector<float>& vertices);
	void SetIndices(const std::vector<uint32_t>& indices);
	void SetVertexShader(IVertexShader* shader);
	void SetFragmentShader(IFragmentShader* shader);
	void Draw(EPrimitive type);
private:
	void DrawPixel(int x, int y, const Eigen::Vector4f& color);
	void DrawPixel(const Eigen::Vector3f& screenPos, const Eigen::Vector4f& color);
	void DrawLine(const Eigen::Vector3f& from, const Eigen::Vector3f& to);
	void DrawLine(float x0, float y0, float x1, float y1);
	Eigen::Vector3f ViewportTransform(const Eigen::Vector3f& in);
	Eigen::Vector3f InvViewportTransform(const Eigen::Vector3f& in);

	void DrawLineStrip();
	void DrawTriangles();
	void DrawPoints();


private:
	std::vector<uint32_t*> _framebuffer;
	int _width;
	int _height;
	uint32_t _clearColor;
	std::vector<float> _vertices;
	std::vector<uint32_t> _indices;
	IVertexShader* _vertexShader;
	IFragmentShader* _fragmentShader;
};