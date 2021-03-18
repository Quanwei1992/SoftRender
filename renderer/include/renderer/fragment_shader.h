#pragma once
#include "glm/glm.hpp"
namespace renderer
{
	class fragment_shader
	{
	public:
		virtual void main() const = 0;

	public:
		mutable glm::vec4 out_frag_color;
	};
}