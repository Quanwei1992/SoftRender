#include "RenderDevice.h"
#include "platform/window.h"
RenderDevice::RenderDevice(Window* context)
	:_width(0)
	,_height(0)
	,_clearColor(0)
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

inline void RenderDevice::DrawPixel(int x, int y, uint32_t color)
{
	uint32_t* dst = _framebuffer[y] + x;
	*dst = color;
}

void RenderDevice::DrawLine(int x0, int y0, int x1, int y1, uint32_t color)
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
			DrawPixel(y, x, color);
		}
		else {
			DrawPixel(x, y, color);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}
