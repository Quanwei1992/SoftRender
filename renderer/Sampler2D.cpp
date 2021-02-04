#include "Sampler2D.h"
#include "Texture.h"
#include "Image.h"
using namespace Eigen;

Sampler2D::Sampler2D(const Texture& texture)
	:_Texture(texture)
{

}

Vector4f Sampler2D::Sample(Vector2f coord) const
{
	
	Vector4f result(0,0,0,1);
	const Image* img = _Texture.GetImage(0);
	if (img == nullptr) return result;

	float u = (img->GetWidth() - 1) * coord.x();
	float v = (img->GetHeight() - 1) * coord.y();

	int x = (int)(u + 0.5f);
	int y = (int)(v + 0.5f);

	int pitch = img->GetWidth() * 3;
	const std::uint8_t* data = img->GetData() + (y * pitch + x*3) ;
	result.x() = data[0] / 255.0f;
	result.y() = data[1] / 255.0f;
	result.z() = data[2] / 255.0f;

	return result;
}

