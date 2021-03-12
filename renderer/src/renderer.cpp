#include "renderer/renderer.h"
#include "render_context.h"
#include "render_device_impl.h"
#include "vertex_buffer_impl.h"
#include "vertex_array_impl.h"
namespace renderer
{
	render_device* create_renderer(void* handle)
	{
		render_context* ctx = create_context((HWND)handle);
		if (ctx == nullptr) {
			return nullptr;
		}
		return new render_device_impl(ctx);
	}

	void release_renderer(render_device* device)
	{
		delete device;
	}

	array_buffer* create_array_buffer()
	{
		return new array_buffer_impl;
	}
	void release_array_buffer(array_buffer* buffer)
	{
		delete buffer;
	}

	vertex_array* create_vertex_array()
	{
		return new vertex_array_impl;
	}

	void release_vertex_array(vertex_array* array)
	{
		delete array;
	}

}