#include "render_device_impl.h"
#include "render_context.h"
#include <cassert>

using namespace renderer;

render_device_impl::render_device_impl(render_context* ctx)
	:_ctx(ctx)
	,_clear_color(0)
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
