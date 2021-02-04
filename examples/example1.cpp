#include "platform/window.h"
#include "RenderDevice.h"
#include "Sampler2D.h"
#include "Texture.h"
#include "Eigen3/Eigen"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <cmath>
#include <random>
#include <ctime>
using namespace Eigen;

class DefaultVertexShader : public IVertexShader
{
public:
	DefaultVertexShader()
	{
		_mvp = Matrix4f::Identity();
	}
	VSOut Main(const VSIn& in) override
	{
		VSOut out;
		out.Position = _mvp * in.Position;
		return out;
	}

public:
	void SetMVP(const Matrix4f mvp)
	{
		_mvp = mvp;
	}

private:
	Matrix4f _mvp;
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
	DefualtFragmentShader(const Sampler2D& samplerDiffuse)
		:_SamplerDiffuse(samplerDiffuse)
	{

	}

	PSOut Main(const PSIn& in) override
	{
		PSOut out;
		out.FragColor = _SamplerDiffuse.Sample(in.Texcoords);
		return out;
	}

private:
	const Sampler2D& _SamplerDiffuse;
};



int main(int argc, char** argv)
{
	Window* window = Window::Create(800, 600, "Example 1 Texture");
	if (!window) return -1;
	RenderDevice device(window);
	
	Vertex v = {
		{-0.5f, -0.5f,.0f},{1.0f,.0f,.0f,1.0f}
	};

	std::vector<Vertex> vertices = {
		// ----位置----       ----颜色----           - 纹理坐标 -
		{{0.5f, 0.5f, 0.0f}   ,{1.0f,0.0f,0.0f,1.0f}  ,{1.0f, 1.0f}}, // 右上
		{{0.5f, -0.5f, 0.0f}  ,{0.0f,1.0f,0.0f,1.0f}  ,{1.0f,0.0f}}, // 右下
		{{-0.5f, -0.5f, 0.0f} ,{0.0f,0.0f,1.0f,1.0f}  ,{0.0f,0.0f}}, // 左下
		{ {-0.5f, 0.5f, 0.0f} ,{1.0f,1.0f,1.0f,1.0f}  ,{0.0f,1.0f}}  // 左上
	};

	std::vector<uint32_t> indices{
		0,1,3,
		1,2,3
	};


	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char* data = stbi_load("data/container.jpg", &width, &height, &nrChannels, 0);

	Texture texture;
	texture.GenerateImage2D(0, EImageFormat::RGB, width, height, EImageDataFormat::R8G8B8, data);
	stbi_image_free(data);


	Sampler2D sampler(texture);

	DefaultVertexShader vs;
	DefualtFragmentShader ps(sampler);

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