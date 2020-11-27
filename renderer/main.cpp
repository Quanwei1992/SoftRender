#include "platform/window.h"
#include "RenderDevice.h"
int main(int argc, char** argv)
{
	Window* window = Window::Create(800, 600, "SoftRender");
	if (!window) return -1;
	RenderDevice device(window);
	
	while (!window->ShouldClose())
	{
		window->PollEvents();
		device.Clear();
		device.DrawLine(10, 10,100,100, 0xff0000);
		window->SwapBuffer();
		Sleep(1);
	}
	delete window;
	return 0;
}