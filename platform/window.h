#pragma once
#include <string>
#include <windows.h>
class Window
{

public:
	static Window* Create(int w, int h, const std::string title);

	~Window();
	void PollEvents();
	bool ShouldClose();
	void SwapBuffer();
	unsigned char* GetBackbuffer() const;
	int GetWidth() const;
	int GetHeight() const;


protected:
	Window(int w, int h, const std::string title);

private:
	int _w,_h;
	HWND _handle;
	HDC _dc;
	HBITMAP _hb;
	HBITMAP _ob;
	unsigned char* _fb;
	bool _shouldClose;

private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
