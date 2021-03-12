#pragma once

#include "render_device.h"
#include "vertex_buffer.h"
#include "vertex_array.h"
namespace renderer
{
	render_device* create_renderer(void* HWND);
	void release_renderer(render_device* device);

	array_buffer* create_array_buffer();
	void release_array_buffer(array_buffer* buffer);

	vertex_array* create_vertex_array();
	void release_vertex_array(vertex_array* array);

}