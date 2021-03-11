#pragma once
#include "Eigen3/Eigen"

namespace MathHelper
{
	inline Eigen::Matrix4f ortho(float left,float right,float top,float bottom,float near,float far)
	{
		Eigen::Matrix4f T;
		T << 1, 0, 0, -(right + left) * 0.5f,
			 0, 1, 0, -(top + bottom) * 0.5f,
			 0, 0, 1, -(near + far) * 0.5f,
			 0, 0, 0, 1;

		Eigen::Matrix4f S;
		S << 2 / (right - left),                  0,                 0, 0,
							  0, 2 / (top - bottom),                 0, 0,
			                  0,                   0, 2 / (near - far), 0,
			                  0,                   0,                0, 1;
		return S * T;
	}

	inline Eigen::Matrix4f projection(float left, float right, float top, float bottom, float near, float far)
	{
		return Eigen::Matrix4f();
	}
}