#pragma once
#include "renderer/render_device.h"
#include "glm/glm.hpp"
#include <windows.h>
#include <vector>

namespace renderer
{
	struct render_context;

	class render_device_impl : public render_device
	{
	public:
		render_device_impl(render_context* ctx);
		~render_device_impl() override;
		void Clear(uint8_t mask) override;
		void ClearColor(float r, float g, float b, float a) override;
		void SwapBuffer() override;
		void UseVertexShader(const vertex_shader* shader) override;
		void UseFragmentShader(const fragment_shader* shader) override;
		void UseVertexArray(const vertex_array* vao) override;
		void DrawArrays(draw_mode mode, int first, int count) override;

	protected:
		render_device_impl() = default;

	private:
		void SetPixel(int x, int y, const::glm::vec4& color);
		bool DrawTriangles(int index);
		void DrawLine(int x1, int y1, int x2, int y2);

	private:
		render_context* _ctx;
		glm::vec4 _clear_color;
		std::vector<uint32_t*> _framebuffer;
		const vertex_shader* _vertex_shader;
		const fragment_shader* _fragment_shader;
		const vertex_array* _vao;



	};
}