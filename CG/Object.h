#pragma once
#include "MeshModel.h"
#include "Matrix4x4.h"


class Object
{
	MeshModel mshMdl;
	Matrix4x4 mdlMtrx;
	bool show_normals;

public:
	//constructors
	Object(MeshModel& mshMdl, Matrix4x4 mdlMtrx);

	//destructor
	~Object();

	void setShowNormals(bool norm);
	bool getShowNormals() { return show_normals; };

};