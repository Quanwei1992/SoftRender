#pragma once

#include "render_device.h"

namespace renderer
{
	render_device* create_renderer(void* HWND);
	void release_renderer(render_device* device);
}