#pragma once
#include "data_types.h"
#include "shader.h"
#include "glm/glm.hpp"
#include <vector>
namespace renderer
{

	class render_device_impl;

	struct input_vertex_arrtibute
	{
		int location;
		data_type type;
		int size;
		void* address;
	};


	class vertex_shader
	{
	public:
		virtual void main() const = 0;

	protected:
		friend class render_device_impl;
		std::vector<input_vertex_arrtibute> _attributes;
		std::vector<shader_varying_info> _varyings;
		mutable glm::vec4 out_position;
	};

}

#define VERTEX_IN_VEC3(location,name) _attributes.push_back(input_vertex_arrtibute{##location##,data_type::FLOAT,3,&##name##})
#define VERTEX_OUT_VEC4(name) _varyings.push_back(shader_varying_info{#name,data_type::FLOAT,4,&##name## })