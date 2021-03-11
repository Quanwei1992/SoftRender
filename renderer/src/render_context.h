#pragma once
#include <windows.h>
#include <cstdint>
namespace renderer
{
	struct render_context
	{
		int width, height;
		HWND handle;
		HDC dc;
		HBITMAP hb;
		HBITMAP ob;
		uint8_t* framebuffer;
	};

	render_context* create_context(HWND handle);
	void release_context(render_context* ctx);
	void swap_buffer(render_context* ctx);
}