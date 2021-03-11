#include "Sampler2D.h"
#include "Texture.h"
#include "Image.h"

Sampler2D::Sampler2D(const Texture& texture)
	:_Texture(texture)
{

}

glm::vec4 Sampler2D::Sample(glm::vec2 coord) const
{
	
	glm::vec4 result(0,0,0,1);
	const Image* img = _Texture.GetImage(0);
	if (img == nullptr) return result;

	float u = (img->GetWidth() - 1) * coord.x;
	float v = (img->GetHeight() - 1) * coord.y;

	int x = (int)(u + 0.5f);
	int y = (int)(v + 0.5f);

	int pitch = img->GetWidth() * 3;
	const std::uint8_t* data = img->GetData() + (y * pitch + x*3) ;
	result.x = data[0] / 255.0f;
	result.y = data[1] / 255.0f;
	result.z = data[2] / 255.0f;

	return result;
}

