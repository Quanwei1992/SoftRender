#include "render_context.h"
namespace renderer
{
	render_context* create_context(HWND handle)
	{

		RECT clientRect;
		if (!GetClientRect(handle, &clientRect))
		{
			return nullptr;
		}
		int w = clientRect.right;
		int h = clientRect.bottom;

		HDC hDC = GetDC(handle);
		HDC dc = CreateCompatibleDC(hDC);
		ReleaseDC(handle, hDC);


		BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
		(DWORD)(w * h * 4), 0, 0, 0, 0 } };

		LPVOID buffer;
		HBITMAP hb = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &buffer, 0, 0);
		if (hb == NULL) return nullptr;

		memset(buffer,0, (size_t)(w * h * 4));

		render_context* ctx = new render_context;
		ctx->framebuffer = (uint8_t*)buffer;
		ctx->handle = handle;
		ctx->width = w;
		ctx->height = h;
		ctx->ob = (HBITMAP)SelectObject(dc, hb);
		ctx->dc = dc;		
		return ctx;
	}

	void release_context(render_context* ctx)
	{
		if (ctx->dc)
		{
			if (ctx->ob)
			{
				SelectObject(ctx->dc, ctx->ob);
				ctx->ob = nullptr;
			}
			DeleteDC(ctx->dc);
			ctx->dc = nullptr;
		}

		if (ctx->hb)
		{
			DeleteObject(ctx->hb);
			ctx->hb = NULL;
		}

		ctx->handle = NULL;
		ctx->framebuffer = NULL;
		delete ctx;
	}

	void swap_buffer(render_context* ctx)
	{
		HDC hDC = GetDC(ctx->handle);
		BitBlt(hDC, 0, 0,ctx->width, ctx->height, ctx->dc, 0, 0, SRCCOPY);
		ReleaseDC(ctx->handle, hDC);
	}
}