#include "Object.h"

Object::Object(MeshModel& mshMdl, Matrix4x4 mdlMtrx) {
	//this->mshMdl = mshMdl;
	this->mdlMtrx = mdlMtrx;
}

Object::~Object()
{
}
