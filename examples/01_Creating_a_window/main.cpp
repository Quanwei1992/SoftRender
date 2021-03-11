#include "GLFW/glfw3.h"
#define  GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#include "renderer/renderer.h"
#include <iostream>

using namespace renderer;



void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main(int argc, const char** argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(800, 600, "SoftRender 1.1 Hello Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	HWND hWnd = glfwGetWin32Window(window);
	render_device* renderer = create_renderer(hWnd);
	renderer->ClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// render loop
    // -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
			   // -----
		processInput(window);

		// render
		// ------
		
		renderer->Clear((uint8_t)clear_type::COLOR_BUFFER_BIT);
		renderer->SwapBuffer();

		// glfw: poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwPollEvents();
	}
	release_renderer(renderer);
	glfwTerminate();
	return 0;
}
