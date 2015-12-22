#pragma once
#include "Vector4.h"
#include "MeshModel.h"
#include "Line.h"


class BBox
{
	Vector4 box_vertices[8];
	Vector4 box_center;

public:
	//constructors
	BBox(MeshModel model);
	BBox();

	//destructor
	~BBox();

	void setVertices(MeshModel& model);
	void transformBox(Matrix4x4& m);
	void drawBox();


};
