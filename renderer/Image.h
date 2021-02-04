#pragma once
#include <cstdint>
enum class EImageFormat
{
	RGB
};

enum class EImageDataFormat
{
	R8G8B8
};

class Image
{
public:
	Image(int width, int height, EImageFormat format, const std::uint8_t* data, EImageDataFormat dataformat);
	~Image();

	int GetWidth() const;
	int GetHeight() const;
	const std::uint8_t* GetData() const;

private:
	int _Width;
	int _Height;
	EImageFormat _Format;
	std::uint8_t* _Data;
};