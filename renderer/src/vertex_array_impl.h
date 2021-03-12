#pragma once
#include "renderer/vertex_array.h"

namespace renderer
{

	struct vertex_attrib
	{
		bool enable;
		int size;
		data_type type;
		int stride;
		const void* pointer;

		vertex_attrib()
			:enable(false)
			,size(0)
			,type(data_type::FLOAT)
			,stride(0)
			,pointer(nullptr)
		{

		}
	};

	class vertex_array_impl : public vertex_array
	{
	public:
		vertex_array_impl();
		~vertex_array_impl();


		void BindBuffer(const array_buffer* buffer) override;


		void VertexAttribPointer(uint32_t index, int size, data_type type, int stride, const void* pointer) override;


		void EnableVertexAttribArray(int index) override;


		void DisableVertexAttribArray(int index) override;

		const array_buffer* GetArrayBuffer() const;
		const vertex_attrib& GetVertexAttrib(int index) const;

	private:
		const array_buffer* _array_buffer;
		vertex_attrib _arrtibs[MAX_VERTEX_ARRTIB_NUM];


	};
}