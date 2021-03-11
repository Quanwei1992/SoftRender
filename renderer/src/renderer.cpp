#include "renderer/renderer.h"
#include "render_context.h"
#include "render_device_impl.h"
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
}