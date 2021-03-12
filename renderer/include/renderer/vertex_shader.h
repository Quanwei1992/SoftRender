#pragma once
#include "data_types.h"
#include "glm/glm.hpp"
#include <vector>
namespace renderer
{


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

	public:
		int GetAttributesCount() const;
		const input_vertex_arrtibute& GetAttribute(int index) const;

	protected:
		std::vector<input_vertex_arrtibute> _attributes;

	public:
		mutable glm::vec4 out_position;
	};
}