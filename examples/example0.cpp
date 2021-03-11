#include "platform/window.h"
#include "RenderDevice.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <cmath>
#include <random>
#include <ctime>

class DefaultVertexShader : public IVertexShader
{
public:
	DefaultVertexShader()
	{
		_mvp = glm::identity<glm::mat4>();
	}
	VSOut Main(const VSIn& in) override
	{
		VSOut out;
		out.Position = _mvp * in.Position;
		return out;
	}

public:
	void SetMVP(const glm::mat4& mvp)
	{
		_mvp = mvp;
	}

private:
	glm::mat4 _mvp;
};

inline float get_random_float()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> dist(0.f, 1.f); // distribution in range [1, 6]

	return dist(rng);
}


class DefualtFragmentShader : public IFragmentShader
{
public:
	PSOut Main(const PSIn& in) override
	{
		PSOut out;
		out.FragColor = in.Color;
		return out;
	}
};



int main(int argc, char** argv)
{
	Window* window = Window::Create(800, 600, "Example 0");
	if (!window) return -1;
	RenderDevice device(window);
	
	Vertex v = {
		{-0.5f, -0.5f,.0f},{1.0f,.0f,.0f,1.0f}
	};

	std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f, 0.0f},{1.0f,.0f,.0f,1.0f }},
		{{0.5f, -0.5f, 0.0f} ,{0.0f,1.0f,.0f,1.0f }},
		{{0.0f,  0.5f, 0.0f} ,{0.0f,.0f,1.0f,1.0f }}
	};

	std::vector<uint32_t> indices{
		0,1,2
	};

	DefaultVertexShader vs;
	DefualtFragmentShader ps;

	device.SetVertices(vertices);
	device.SetIndices(indices);
	device.SetVertexShader(&vs);
	device.SetFragmentShader(&ps);
	


	while (!window->ShouldClose())
	{
		window->PollEvents();
		device.Clear();
		clock_t t = clock();
		device.Draw(EPrimitive::TRIANGLES);
		clock_t e = clock() - t;
		double sec = (double)e / CLOCKS_PER_SEC;
		window->SwapBuffer();
		Sleep(1);
	}
	delete window;
	return 0;
}