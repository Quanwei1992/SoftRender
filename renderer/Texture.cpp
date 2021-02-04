#include "Texture.h"

Texture::Texture()
	:_WrapS(ETextureWrap::REPEAT)
	,_WrapT(ETextureWrap::REPEAT)
	,_WrapR(ETextureWrap::REPEAT)
	,_FilterMin(ETextureFilter::NEAREST)
	,_FilterMag(ETextureFilter::NEAREST)
{
	for (int i =0 ;i<MAX_MIPMAPS;i++)
	{
		_Mipmaps[i] = nullptr;
	}
}

Texture::~Texture()
{
	for (int i = 0; i < MAX_MIPMAPS; i++)
	{
		if (_Mipmaps[i] != nullptr)
		{
			delete _Mipmaps[i];
			_Mipmaps[i] = nullptr;
		}
	}
}

const Image* Texture::GenerateImage2D(int level, EImageFormat format, int width, int height, EImageDataFormat dataformat, std::uint8_t* pixels)
{
	if (pixels == nullptr) return nullptr;
	if (level < 0 || level >= MAX_MIPMAPS) return nullptr;

	if(_Mipmaps[level] != nullptr)
	{
		delete _Mipmaps[level];
		_Mipmaps[level] = nullptr;
	}

	Image* img = new Image(width, height, format, pixels, dataformat);
	_Mipmaps[level] = img;
	return img;
}

const Image* Texture::GetImage(int level) const
{
	if (level < 0 || level >= MAX_MIPMAPS) return nullptr;
	return _Mipmaps[level];
}

