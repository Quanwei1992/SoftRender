#include "vertex_array_impl.h"

renderer::vertex_array_impl::vertex_array_impl()
	:_array_buffer(nullptr)
{

}

renderer::vertex_array_impl::~vertex_array_impl()
{

}

void renderer::vertex_array_impl::BindBuffer(const array_buffer* buffer)
{
	_array_buffer = buffer;
}

void renderer::vertex_array_impl::VertexAttribPointer(uint32_t index, int size, data_type type, int stride, const void* pointer)
{
	vertex_attrib& attrib = _arrtibs[index];
	attrib.size = size;
	attrib.type = type;
	attrib.stride = stride;
	attrib.pointer = pointer;
}

void renderer::vertex_array_impl::EnableVertexAttribArray(int index)
{
	_arrtibs[index].enable = true;
}

void renderer::vertex_array_impl::DisableVertexAttribArray(int index)
{
	_arrtibs[index].enable = false;
}

const renderer::array_buffer* renderer::vertex_array_impl::GetArrayBuffer() const
{
	return _array_buffer;
}

const renderer::vertex_attrib& renderer::vertex_array_impl::GetVertexAttrib(int index) const
{
	return _arrtibs[index];
}
