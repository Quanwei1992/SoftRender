#pragma once
#include <vector>
#include "Image.h"
enum class ETextureWrap
{
	REPEAT,			 // �������Ĭ����Ϊ���ظ�����ͼ��
	MIRRORED_REPEAT, // �� REPEATһ������ÿ���ظ�ͼƬ�Ǿ�����õġ�
	CLAMP_TO_EDGE,   // ��������ᱻԼ����0��1֮�䣬�����Ĳ��ֻ��ظ���������ı�Ե������һ�ֱ�Ե�������Ч����
	CLAMP_TO_BORDER  // ����������Ϊ�û�ָ���ı�Ե��ɫ��
};

enum class ETextureFilter
{
	NEAREST,
	LINER,
	NEAREST_MIPMAP_NEAREST, // ʹ�����ڽ��Ķ༶��Զ������ƥ�����ش�С����ʹ���ڽ���ֵ�����������
	LINEAR_MIPMAP_NEAREST,  // ʹ�����ڽ��Ķ༶��Զ�����𣬲�ʹ�����Բ�ֵ���в���
	NEAREST_MIPMAP_LINEAR,  // ��������ƥ�����ش�С�Ķ༶��Զ����֮��������Բ�ֵ��ʹ���ڽ���ֵ���в���
	LINEAR_MIPMAP_LINEAR    // �������ڽ��Ķ༶��Զ����֮��ʹ�����Բ�ֵ����ʹ�����Բ�ֵ���в���
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