#pragma once
#ifndef Z_BUFFER_H
#define Z_BUFFER_H
#include <vector>
#include "Triangle.h"
#include "Color.h"

class Z_Buffer
{
	typedef struct {
		double zValue;
		Color color;
	}Zpixel;
	std::vector<std::vector<Zpixel>> buffer;

public:
	//constructors
	Z_Buffer();

	//destructor
	~Z_Buffer();

	//class methods
	double Depth(Triangle Q, double X, double Y);
};


#endif