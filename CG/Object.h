#pragma once
#include "MeshModel.h"
#include "Matrix4x4.h"


class Object
{
	MeshModel mshMdl;
	Matrix4x4 mdlMtrx;

public:
	//constructors
	Object(MeshModel& mshMdl, Matrix4x4 mdlMtrx);

	//destructor
	~Object();


};