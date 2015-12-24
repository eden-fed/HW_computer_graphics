//#pragma once

#ifndef OBJECT_H
#define OBJECT_H
#include "MeshModel.h"
#include "Matrix4x4.h"
#include "Line.h"
#include <set>
#include "Material.h"

class Object
{
	MeshModel mshMdl;
	Matrix4x4 mdlMtrx;
	std::set<Line> linesToDraw;


public:
	Material material;

	//constructors
	Object(MeshModel& mshMdl, Matrix4x4& mdlMtrx);
	Object();

	//destructor
	~Object();

	//set functions
	void setModel(MeshModel& mshMdl, Matrix4x4& mdlMtrx);
	void setMtrx(Matrix4x4 mdlMtrx);


	//get functions
	MeshModel& getMshMdl() { return mshMdl; };
	std::set<Line>& getLinesToDraw() { return linesToDraw; };
	Matrix4x4& getMtrx() { return mdlMtrx; };
	

	//class methods
	void drawObjectCrdSystem(Matrix4x4& objMtrx,Vector4& objCentroid,double x, double y);
};
#endif