#pragma once
#include "MeshModel.h"
#include "Matrix4x4.h"
#include "Line.h"
#include <set>

class Object
{
	MeshModel mshMdl;
	Matrix4x4 mdlMtrx;
	bool show_normals;
	std::set<Line> linesToDraw;
	std::set<Line>::iterator linesToDrawIterator;
public:
	//constructors
	Object(MeshModel& mshMdl, Matrix4x4 mdlMtrx);

	//destructor
	~Object();

	void setShowNormals(bool norm);
	bool getShowNormals() { return show_normals; };

};	