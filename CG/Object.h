//#pragma once

#ifndef OBJECT_H
#define OBJECT_H
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
public:
	//constructors
	Object(MeshModel& mshMdl, Matrix4x4 mdlMtrx);

	//destructor
	~Object();

	//set functions
	void setShowNormals(bool norm);

	//get functions
	bool getShowNormals() { return show_normals; };
	std::set<Line>& getLinesToDraw() { return linesToDraw; };
	//class methods
	void populatelinesToDraw();
	void drawObjectTriangles();
	void drawObject();
};
#endif