#include "Image.h"
#include <cstring>


Image::Image(int width, int height, EImageFormat format, const std::uint8_t* data, EImageDataFormat dataformat)
	:_Width(width)
	, _Height(height)
	, _Format(format)
	, _Data(nullptr)
{
	_Data = new std::uint8_t[width * height * 3];
	memcpy(_Data, data, width * height * 3);
}

Image::~Image()
{
	delete[] _Data;
	_Data = nullptr;
}

int Image::GetWidth() const
{
	return _Width;
}

int Image::GetHeight() const
{
	return _Height;
}

const std::uint8_t* Image::GetData() const
{
	return _Data;
}
