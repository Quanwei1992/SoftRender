#pragma once
#include "renderer/vertex_buffer.h"
#include <cstdint>

namespace renderer
{
	class array_buffer_impl : public array_buffer
	{

	public:
		~array_buffer_impl();
		array_buffer_impl();
		void BufferData(size_t size, const void* data) override;
		const uint8_t* get_data() const;
		size_t get_size() const;

	private:
		uint8_t* _data;
		size_t _size;
	};
}
