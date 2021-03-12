#pragma once

namespace renderer
{
	class array_buffer
	{
	public:
		virtual ~array_buffer() {};
		virtual void BufferData(size_t size, const void* data) = 0;
	};
}