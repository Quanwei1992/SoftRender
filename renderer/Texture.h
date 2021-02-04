#pragma once
#include <vector>
#include "Image.h"
enum class ETextureWrap
{
	REPEAT,			 // 对纹理的默认行为。重复纹理图像。
	MIRRORED_REPEAT, // 和 REPEAT一样，但每次重复图片是镜像放置的。
	CLAMP_TO_EDGE,   // 纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
	CLAMP_TO_BORDER  // 超出的坐标为用户指定的边缘颜色。
};

enum class ETextureFilter
{
	NEAREST,
	LINER,
	NEAREST_MIPMAP_NEAREST, // 使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样
	LINEAR_MIPMAP_NEAREST,  // 使用最邻近的多级渐远纹理级别，并使用线性插值进行采样
	NEAREST_MIPMAP_LINEAR,  // 在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样
	LINEAR_MIPMAP_LINEAR    // 在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样
};

const int MAX_MIPMAPS = 32;

class Texture
{
public:
	Texture();
	~Texture();

	const Image* GenerateImage2D(int level, EImageFormat format, int width, int height,EImageDataFormat dataformat,std::uint8_t* pixels);
	const Image* GetImage(int level) const;


private:
	ETextureWrap _WrapS;
	ETextureWrap _WrapT;
	ETextureWrap _WrapR;

	ETextureFilter _FilterMin;
	ETextureFilter _FilterMag;
	Image* _Mipmaps[MAX_MIPMAPS] ;
};