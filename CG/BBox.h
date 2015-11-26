#pragma once
#include "Vector4.h"


class BBox
{

	Vector4 box_vertices[8];
	Vector4 box_center;
	int color;

public:
	//constructors
	BBox();

	//destructor
	~BBox();

	

};
