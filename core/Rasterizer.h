#pragma once
#include <vector>
#include "Eigen3/Eigen"

enum class EBuffer {
	Color = 1 << 0,
	Depth = 1 << 1
};

inline EBuffer operator|(EBuffer a, EBuffer b)
{
	return EBuffer((int)a | (int)b);
}
inline EBuffer operator&(EBuffer a, EBuffer b)
{
	return EBuffer((int)a & (int)b);
}

enum class EPrimitive {
	Line,
	Triangle
};



class Rasterizer
{
public:
	Rasterizer(int width, int height);
	void SetPositions (const std::vector<Eigen::Vector3f>& positions);
	void SetIndices(const std::vector<Eigen::Vector3i>& indices);

	void SetModel(const Eigen::Matrix4f& m);
	void SetView(const Eigen::Matrix4f& m);
	void SetProjection(const Eigen::Matrix4f& m);

	void Clear(EBuffer buff);

	void Draw(EPrimitive type);

	void SaveFrameBufferToTGA(const std::string& filename) const;




private:
	void DrawLine(int x0, int y0, int x1, int y1,const Eigen::Vector3f& color);
	void SetPixel(int x, int y, const Eigen::Vector3f& color);



private:
	Eigen::Matrix4f _model;
	Eigen::Matrix4f _view;
	Eigen::Matrix4f _projection;

	std::vector<Eigen::Vector3f> _positions;
	std::vector<Eigen::Vector3i> _indices;

	std::vector<Eigen::Vector3f> _framebuf;
	std::vector<float> _depthbuf;

	int _width, _height;
};