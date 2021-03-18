#pragma once
#include "data_types.h"

namespace renderer
{
	struct shader_varying_info
	{
		const char* name;
		data_type type;
		int size;
		void* address;
	};	
}