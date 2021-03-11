#pragma once
#include <glm/glm.hpp>

class Texture;
class Sampler2D
{
public:
	Sampler2D(const Texture& texture);
	glm::vec4 Sample(glm::vec2 coord) const;

private:
	const Texture& _Texture;
};