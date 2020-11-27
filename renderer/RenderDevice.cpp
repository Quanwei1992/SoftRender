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

void RenderDevice::DrawPixel(int x, int y, uint32_t color)
{
	uint32_t* dst = _framebuffer[y] + x;
	*dst = color;
}
