#include "window.h"

#define WNDCLASS_NAME "SOFTRENDER_WC"


Window::Window(int w, int h, const std::string title)
	:_w(w)
	,_h(h)
	,_handle(NULL)
	,_dc(NULL)
	,_hb(NULL)
	,_ob(NULL)
	,_fb(NULL)
	,_shouldClose(false)
{
	
}

Window::~Window()
{
	RemoveProp(_handle, "PROC_ONWER");
	ShowWindow(_handle, SW_HIDE);

	if (_dc)
	{
		if (_ob)
		{
			SelectObject(_dc, _ob);
			_ob = NULL;
		}
		DeleteDC(_dc);
		_dc = NULL;
	}

	if (_hb)
	{
		DeleteObject(_hb);
		_hb = NULL;
	}

	if (_handle)
	{
		CloseWindow(_handle);
		_handle = NULL;
	}
	_fb = NULL;
}




LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* window = (Window*)GetProp(hWnd, "PROC_ONWER");
	if (window == NULL)
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	
	switch (msg)
	{
	case WM_CLOSE:
		window->_shouldClose = true;
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return  DefWindowProc(hWnd, msg, wParam, lParam);
}

Window* Window::Create(int w, int h, const std::string title)
{
	WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)Window::WndProc, 0, 0, 0,
			NULL, NULL, NULL, NULL, WNDCLASS_NAME };

	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
		(DWORD)(w * h * 4), 0, 0, 0, 0 } };

	RECT rect = { 0, 0, w, h };

	int wx, wy, sx, sy;

	LPVOID ptr;
	HDC hDC;

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);

	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	if (!RegisterClass(&wc)) return NULL;

	HWND handle = CreateWindow(WNDCLASS_NAME, title.c_str(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	if (handle == NULL) return NULL;

	hDC = GetDC(handle);
	HDC dc = CreateCompatibleDC(hDC);
	ReleaseDC(handle, hDC);

	HBITMAP hb = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if (hb == NULL) return NULL;


	Window* window = new Window(w, h, title);

	window->_ob = (HBITMAP)SelectObject(dc, hb);
	window->_fb = (unsigned char*)ptr;
	window->_handle = handle;
	window->_dc = dc;

	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetProp(window->_handle, "PROC_ONWER",window);
	SetWindowPos(window->_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(window->_handle);
	ShowWindow(window->_handle, SW_NORMAL);
	memset(window->_fb, 0, (size_t)(w * h * 4));
	return window;
}

void Window::PollEvents()
{
	MSG msg;
	if (!PeekMessage(&msg, _handle, 0, 0, PM_NOREMOVE)) return;
	if (!GetMessage(&msg, _handle, 0, 0)) return;
	DispatchMessage(&msg);
}

bool Window::ShouldClose()
{
	return _shouldClose;
}

void Window::SwapBuffer()
{
	HDC hDC = GetDC(_handle);
	BitBlt(hDC, 0, 0, _w, _h, _dc, 0, 0, SRCCOPY);
	ReleaseDC(_handle, hDC);
}

unsigned char* Window::GetBackbuffer() const
{
	return _fb;
}

int Window::GetWidth() const
{
	return _w;
}

int Window::GetHeight() const
{
	return _h;
}

