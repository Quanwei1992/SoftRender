#pragma once
#include "data_types.h"
#include <cstdint>
namespace renderer
{
	const int MAX_VERTEX_ARRTIB_NUM = 8;
	class array_buffer;
	class vertex_array
	{
	public:
		virtual ~vertex_array() {};
		virtual void BindBuffer(const array_buffer* buffer) = 0;
		virtual void VertexAttribPointer(uint32_t index, int size, data_type type, int stride, const void* pointer) = 0;
		virtual void EnableVertexAttribArray(int index) = 0;
		virtual void DisableVertexAttribArray(int index) = 0;

	};
}