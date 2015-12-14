#pragma once
#ifndef Z_BUFFER_H
#define Z_BUFFER_H
#include <vector>
#include "Triangle.h"

class Z_Buffer
{
	std::vector<std::vector<double>> Pixels;

public:
	//constructors
	Z_Buffer();

	//destructor
	~Z_Buffer();

	//class methods
	double Depth(Triangle Q, double X, double Y);
};


#endif