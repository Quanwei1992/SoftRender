
#include "Eigen3/Core"
#include "Eigen3/Dense"
#include "Rasterizer.h"
#include "TGAImage.h"
#include "OBJLoader/OBJ_Loader.h"
#include  <iostream>


using namespace Eigen;


struct AABB
{
	Vector3f min, max;
};


Matrix4f MakeOrthoProjectionMatrix(float l,float r,float b,float t,float n,float f)
{
	Matrix4f mt;
	mt << 1, 0, 0, -(r + l) / 2,
		 0, 1, 0, -(t + b) / 2,
		 0, 0, 1, -(n + f) / 2,
		 0, 0, 0, 1;
	Matrix4f ms;
	ms << 2.0f/(r-l), 0, 0,0,
		0, 2.0f/(t-b), 0, 0,
		0,0, 2.0f/(f-n), 0,
		0, 0, 0, 1;
	return ms * mt;		 
}

AABB CalculateAABB(const std::vector<Vector3f>& positions)
{
	AABB aabb;
	aabb.max = Vector3f(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
	aabb.min = Vector3f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	for (auto& pos : positions)
	{
		for (int i =0;i<3;i++)
		{
			if (pos[i] < aabb.min[i]) aabb.min[i] = pos[i];
			if (pos[i] > aabb.max[i]) aabb.max[i] = pos[i];
		}


	}
	return aabb;
}

int main(int argc, char** argv)
{
	Rasterizer rst(800, 600);


	objl::Loader objLoader;
	assert(objLoader.LoadFile("./data/models/bunny/bunny.obj"));

	auto vertes = objLoader.LoadedVertices;
	std::vector<Vector3f> positions;
	for (auto& vert : vertes)
	{
		positions.push_back(Vector3f(vert.Position.X, vert.Position.Y, vert.Position.Z));
	}

	AABB aabb = CalculateAABB(positions);

	Matrix4f proj = MakeOrthoProjectionMatrix(aabb.min.x(),aabb.max.x(), aabb.min.y(), aabb.max.y(), aabb.min.z(), aabb.max.z());
	rst.SetProjection(proj);

	rst.SetPositions(positions);
	rst.SetIndices(objLoader.LoadedIndices);
	rst.Draw(EPrimitive::Triangle);
	rst.SaveFrameBufferToTGA("output.tga");
	return 0;
}