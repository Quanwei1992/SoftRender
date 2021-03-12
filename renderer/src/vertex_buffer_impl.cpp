#include "vertex_buffer_impl.h"
#include <string>

renderer::array_buffer_impl::~array_buffer_impl()
{
	if (_data)
	{
		delete[] _data;
		_data = nullptr;
		_size = 0;
	}
}

renderer::array_buffer_impl::array_buffer_impl()
	:_data(nullptr)
	,_size(0)
{

}

void renderer::array_buffer_impl::BufferData(size_t size, const void* data)
{
	_data = new uint8_t[size];
	_size = size;
	memcpy(_data, data, size);
}

const uint8_t* renderer::array_buffer_impl::get_data() const
{
	return _data;
}

size_t renderer::array_buffer_impl::get_size() const
{
	return _size;
}

