#include <iostream>
#include "Renderer.h"

#ifndef COORDINATE_H
#define COORDINATE_H


class Coordinate
{
private:
	//assighning coordinate variables
	int X;
	int Y;
	unsigned int color;


public:
	//constructors
	Coordinate();
	Coordinate(const int valX, const int valY, unsigned int valColor);

	//copy constructor
	Coordinate(const Coordinate& coordinate);

	//destructor
	~Coordinate();

	//get functions
	const int getX()const { return X; }
	const int getY()const { return Y; }
	const int getColor()const { return color; }

	//set functions
	void setX(const int valX);
	void setY(const int valY);
	void setColor(const int valColor);

	//operators
	const Coordinate& operator = (const Coordinate& coordinate);
	const bool operator == (const Coordinate& coordinate)const;//the (==) sighn compares X and Y values
	const bool operator != (const Coordinate& coordinate)const;//the (!=) sighn compares X and Y values
	operator Pixel();
	void swapXY();
};

#endif