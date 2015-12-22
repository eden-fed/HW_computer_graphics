#pragma once
#ifndef Z_BUFFER_H
#define Z_BUFFER_H
#include <vector>
#include "Triangle.h"
#include "Color.h"

typedef struct {
	Vector4 baryCrd;
	Coordinate cartCrd;
	Color clr;
	Triangle trl;
} stZbufferInfo;
class Z_Buffer
{
	//screen resolution is 1600/900
	typedef struct {
		double zValue;
		Color color;
		bool to_print;
	}Zpixel;
	//std::vector<std::vector<Zpixel>> buffer;
	Zpixel **buffer;
	int zWidth, zHeight;


public:
	//constructors
	Z_Buffer(int width, int height);

	//destructor
	~Z_Buffer();

	//class methods
	//double Depth(Triangle Q, double X, double Y);
	double Depth(Triangle& Q, Vector4& baryCrd);
	//void FillBuffer(std::vector<Triangle> sceneTriangles);
	void FillPixelInBuffer(stZbufferInfo& pixel);
	void drawBuffer();
	void reshape(int width, int height);
	void emptyBuffer();
};


#endif