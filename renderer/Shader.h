#pragma once
#include "Eigen3/Eigen"

struct VSOut
{
	Eigen::Vector3f Position;
};

struct VSIn
{
	Eigen::Vector3f Position;
};

struct PSIn {
	Eigen::Vector3f Position;
};

struct PSOut {
	Eigen::Vector4f FragColor;
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