#pragma once
#include "glm/glm.hpp"
namespace renderer
{
	class fragment_shader
	{
	public:
		virtual void main() = 0;

	public:
		glm::vec4 out_frag_color;
	};
}