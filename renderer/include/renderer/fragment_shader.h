#pragma once
#include "glm/glm.hpp"
#include "shader.h"
#include <vector>
namespace renderer
{
	class render_device_impl;
	class fragment_shader
	{
	public:
		virtual void main() const = 0;

	protected:
		friend class render_device_impl;
		mutable glm::vec4 out_frag_color;
		std::vector<shader_varying_info> _varyings;
	};
}

#define FRAGMENT_IN_VEC4(name) _varyings.push_back(shader_varying_info{#name,data_type::FLOAT,4,&##name## })