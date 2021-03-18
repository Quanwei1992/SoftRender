#include "GLFW/glfw3.h"
#define  GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#include "renderer/renderer.h"
#include "renderer/vertex_shader.h"
#include "renderer/fragment_shader.h"
#include <iostream>

using namespace renderer;



class _vertex_shader : public renderer::vertex_shader
{

public:
	glm::vec3 position;

public:
	_vertex_shader()
		:position(0)
		,vertex_color(0)
	{
		VERTEX_IN_VEC3(0,position);
		VERTEX_OUT_VEC4(vertex_color);
	}

	void main() const override
	{
		out_position = glm::vec4(position.x,position.y,position.z, 1);
		vertex_color = glm::vec4(0.5f, 0.0, 0.0, 1.0);
	}

protected:
	mutable glm::vec4 vertex_color;
};


class _fragment_shader : public renderer::fragment_shader
{
public:
	_fragment_shader()
		:vertex_color(0)
	{
		FRAGMENT_IN_VEC4(vertex_color);
	}
	void main() const override
	{
		out_frag_color = vertex_color;
	}
protected:
	glm::vec4 vertex_color;
};



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
	GLFWwindow* window = glfwCreateWindow(800, 600, "SoftRender 1.2 Hello Triangle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	HWND hWnd = glfwGetWin32Window(window);
	render_device* renderer = create_renderer(hWnd);
	renderer->ClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};



	array_buffer* vbo = create_array_buffer();
	vbo->BufferData(sizeof(vertices), vertices);

	vertex_array* vao = create_vertex_array();
	vao->BindBuffer(vbo);
	vao->VertexAttribPointer(0, 3, data_type::FLOAT, 3 * sizeof(float), (void*)0);
	vao->EnableVertexAttribArray(0);


	_vertex_shader vshader;
	_fragment_shader fshader;

	renderer->UseVertexShader(&vshader);
	renderer->UseFragmentShader(&fshader);
	renderer->UseVertexArray(vao);

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
		renderer->DrawArrays(draw_mode::TRIANGLES, 0, 3);
		renderer->SwapBuffer();

		// glfw: poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwPollEvents();
	}
	release_array_buffer(vbo);
	release_vertex_array(vao);
	release_renderer(renderer);
	glfwTerminate();
	return 0;
}
