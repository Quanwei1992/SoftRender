#include "renderer/vertex_shader.h"

namespace renderer
{

	int vertex_shader::GetAttributesCount() const
	{
		return (int)_attributes.size();
	}

	const renderer::input_vertex_arrtibute& vertex_shader::GetAttribute(int index) const
	{
		return _attributes[index];
	}

}