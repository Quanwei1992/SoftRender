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

	protected:
		render_device_impl() = default;

	private:
		void SetPixel(int x, int y, const::glm::vec4& color);

	private:
		render_context* _ctx;
		glm::vec4 _clear_color;
		std::vector<uint32_t*> _framebuffer;



	};
}