#include "Z_Buffer.h"
#include "Renderer.h"

Z_Buffer::Z_Buffer(int width, int height)
{
	this->zWidth = width;
	this->zHeight = height;
	buffer = new Zpixel*[width];
	for (int x = 0; x < width; x++) {
		buffer[x] = new Zpixel[height];
	}
}

Z_Buffer::~Z_Buffer()
{
	for (int x = 0; x < this->zWidth; ++x) {
		delete[] buffer[x];
	}
	delete[] buffer;
}
double Z_Buffer::Depth(Triangle Q, Vector4 baryCrd)
{
	return Q[0][2] * baryCrd[0] + Q[1][2] * baryCrd[1] + Q[2][2] * baryCrd[2];
}
/*
double Z_Buffer::Depth(Triangle Q, double X, double Y)
{
	//Ax + By +Cz + D = 0 Normal Vector: N=(A, B, C)
	Vector4 N = Q.getNormal();

	double A = N[0];
	double B = N[1];
	double C = N[2];
	double D = -(A*Q[1][0] + B*Q[1][0] + C*Q[1][0]);


	//Z = (-AX -BY -D)/C
	return -((A*X) + (B*Y) + D) / C;
}*/

void Z_Buffer::FillBuffer(std::vector<Triangle> sceneTriangles)
{
	emptyBuffer();
	std::vector<stBaryAndCartCrd> TriangleCrd;

	for (int i = 0; i < sceneTriangles.size(); i++) {
		sceneTriangles[i].triangleScanConversion(TriangleCrd);
		unsigned int rnd = rand() << 8;
		for (int j = 0; j < TriangleCrd.size(); j++) {
			int x = TriangleCrd[j].cartCrd.getX();
			int y = TriangleCrd[j].cartCrd.getY();
			if (x < 0 || y < 0 || x > this->zWidth || y > this->zHeight)
				continue;
			double z = Depth(sceneTriangles[i], TriangleCrd[j].baryCrd);
			if (z > this->buffer[x][y].zValue) {
				buffer[x][y].zValue = z;
				//buffer[x][y].color = sceneTriangles[i].getColorOfPoint(x, y);
				buffer[x][y].color.setColor(0x00ff00 + rnd);
				buffer[x][y].to_print = true;
			}
		}
	}
}

void Z_Buffer::drawBuffer()
{
	Renderer renderer;
	Pixel pxlToPush;
	std::vector<Pixel> pixels;

	for (int x = 0; x < this->zWidth; x++) {
		for (int y = 0; y < this->zHeight; y++) {
			if (buffer[x][y].to_print) {
				pxlToPush.x = x;
				pxlToPush.y = y;
				pxlToPush.color = buffer[x][y].color.getColor();
				pixels.push_back(pxlToPush);
			}
		}
	}

	renderer.drawPixels(pixels);

}

void Z_Buffer::reshape(int width, int height)
{


	for (int x = 0; x < this->zWidth; ++x) {
		delete[] buffer[x];
	}
	delete[] buffer;

	this->zWidth = width;
	this->zHeight = height;

	buffer = new Zpixel*[width];
	for (int x = 0; x < width; x++) {
		buffer[x] = new Zpixel[height];
	}
}

void Z_Buffer::emptyBuffer()
{
	for (int x = 0; x < this->zWidth; x++) {
		for (int y = 0; y < this->zHeight; y++) {
			buffer[x][y].zValue = -DBL_MAX;
			buffer[x][y].color.setColor(0, 0, 0);
			buffer[x][y].to_print = false;
		}
	}
}
