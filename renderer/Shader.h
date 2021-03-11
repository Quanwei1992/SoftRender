#pragma once
#include <glm/glm.hpp>

struct VSOut
{
	glm::vec4 Position;
};

struct VSIn
{
	glm::vec4 Position;
};

struct PSIn {
	glm::vec4 Color;
	glm::vec2 Texcoords;

};

struct PSOut {
	glm::vec4 FragColor;
};


class IVertexShader
{
public:
	virtual VSOut Main(const VSIn& in) = 0;
};

class IFragmentShader
{
public:
	virtual PSOut Main(const PSIn& in) = 0;
};