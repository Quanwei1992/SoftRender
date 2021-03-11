#pragma once
#include <cstdint>

namespace renderer
{

	enum class clear_type : uint8_t
	{
		COLOR_BUFFER_BIT = 1,
		DEPTH_BUFFER_BIT = 1 << 1,
		STENCIL_BUFFER_BIT = 1 << 2
	};


	class render_device
	{
	public:
		virtual ~render_device() {};
		virtual void ClearColor(float r, float g, float b, float a) = 0;
		virtual void Clear(uint8_t mask) = 0;
		virtual void SwapBuffer() = 0;

	protected:
		render_device() {};
	};

}

