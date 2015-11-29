#pragma once
#include "Vector4.h"
#include "MeshModel.h"


class BBox
{
	Vector4 box_vertices[8];
	Vector4 box_center;

public:
	//constructors
	BBox(MeshModel model);

	//destructor
	~BBox();

	void drawObject();


};
