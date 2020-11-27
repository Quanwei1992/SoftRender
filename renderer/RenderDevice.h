#pragma once
#include <cstdint>
#include <vector>

class Window;

class RenderDevice
{
public:
	RenderDevice(Window* context);

	void Clear();
	void SetClearColor(uint32_t color);

public:
	void DrawPixel(int x, int y, uint32_t color);
	void DrawLine(int x0, int y0, int x1, int y1, uint32_t color);

private:
	std::vector<uint32_t*> _framebuffer;
	int _width;
	int _height;
	uint32_t _clearColor;
};