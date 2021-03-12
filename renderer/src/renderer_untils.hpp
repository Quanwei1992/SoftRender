#pragma once
#include "renderer/data_types.h"
namespace renderer
{
	int get_size_by_data_type(data_type type)
	{
		switch (type)
		{
		case renderer::data_type::FLOAT:
			return 4;
		default:
			break;
		}
		return 0;
	}
}