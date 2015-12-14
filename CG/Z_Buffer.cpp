#include "Z_Buffer.h"

Z_Buffer::Z_Buffer()
{
	for (int i = 0; i < buffer.size(); i++) {
		for (int j = 0; j = buffer[i].size(); j++) {
			buffer[i][j].zValue = -DBL_MAX;
			buffer[i][j].color.setColor(0, 0, 0);
			buffer[i][j].to_print = false;
		}
	}
}

Z_Buffer::~Z_Buffer()
{
}

double Z_Buffer::Depth(Triangle Q, double X, double Y)
{
	return 0.0;
}

void Z_Buffer::FillBuffer(std::vector<Triangle> sceneTriangles)
{
	std::vector<Coordinate> TriangleCrd;

	for (int i = 0; i < sceneTriangles.size(); i++) {
		sceneTriangles[i].triangleScanConversion(TriangleCrd);

		for (int j = 0; j < TriangleCrd.size(); j++) {
			int x = TriangleCrd[j].getX();
			int y = TriangleCrd[j].getY();
			double z = Depth(sceneTriangles[i], (double)x, (double)y);
			if (z > this->buffer[x][y].zValue) {
				buffer[x][y].zValue = z;
				buffer[x][y].color = sceneTriangles[i].getColorOfPoint(x, y);
				buffer[x][y].to_print = true;
			}
		}
	}
}
