#pragma once
#include <Eigen3/Eigen>

class Texture;
class Sampler2D
{
public:
	Sampler2D(const Texture& texture);
	Eigen::Vector4f Sample(Eigen::Vector2f coord) const;

private:
	const Texture& _Texture;
};