#pragma once
#include <cstdint>

namespace renderer
{
	class vertex_shader;
	class fragment_shader;
	class vertex_array;
	enum class clear_type : uint8_t
	{
		COLOR_BUFFER_BIT = 1,
		DEPTH_BUFFER_BIT = 1 << 1,
		STENCIL_BUFFER_BIT = 1 << 2
	};

	enum class draw_mode
	{
		TRIANGLES
	};


	class render_device
	{
	public:
		virtual ~render_device() {};
		virtual void ClearColor(float r, float g, float b, float a) = 0;
		virtual void Clear(uint8_t mask) = 0;
		virtual void SwapBuffer() = 0;
		virtual void UseVertexShader(const vertex_shader* shader) = 0;
		virtual void UseFragmentShader(const fragment_shader* shader) = 0;
		virtual void UseVertexArray(const vertex_array* vao) = 0;
		virtual void DrawArrays(draw_mode mode,int first,int count) = 0;

	protected:
		render_device() {};
	};

}

