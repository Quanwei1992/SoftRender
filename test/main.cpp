
#include "Eigen3/Core"
#include "Eigen3/Dense"
#include "Rasterizer.h"
#include "TGAImage.h"
#include  <iostream>


using namespace Eigen;


int main(int argc, char** argv)
{
	Rasterizer rst(800, 600);

	std::vector<Vector3f> positions = {
		{-0.2f,-0.2f,0},
		{0.2f,-0.2f,0},
		{.0f,0.2f,0}
	};

	std::vector<Vector3i> indices = {
		{0,1,2}
	};

	rst.SetPositions(positions);
	rst.SetIndices(indices);
	rst.Draw(EPrimitive::Triangle);
	rst.SaveFrameBufferToTGA("output.tga");
	return 0;
}