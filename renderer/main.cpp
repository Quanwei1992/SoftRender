#include "platform/window.h"
#include "RenderDevice.h"
#include "Eigen3/Eigen"
#include <cmath>
#include <random>
using namespace Eigen;

class DefaultVertexShader : public IVertexShader
{
public:
	VSOut Main(const VSIn& in) override
	{
		VSOut out;
		out.Position = in.Position;
		return out;
	}
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

		out.FragColor = Vector4f(1,0,0,1);
		return out;
	}
};



int main(int argc, char** argv)
{
	Window* window = Window::Create(800, 600, "SoftRender");
	if (!window) return -1;
	RenderDevice device(window);
	
	std::vector<float> vertices = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
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
		device.Draw(EPrimitive::LINE_STRIP);
		window->SwapBuffer();
		Sleep(1);
	}
	delete window;
	return 0;
}