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
	std::set<Line> linesToDraw;
public:
	//constructors
	Object(MeshModel& mshMdl, Matrix4x4 mdlMtrx);
	Object();

	//destructor
	~Object();

	//set functions
	void setModel(MeshModel& mshMdl, Matrix4x4 mdlMtrx);

	//get functions
	MeshModel& getMshMdl() { return mshMdl; };
	std::set<Line>& getLinesToDraw() { return linesToDraw; };
	//class methods
	void populatelinesToDraw();
	void drawObjectTriangles();
	void drawObject();
	void drawNormals(double normSize);
};
#endif