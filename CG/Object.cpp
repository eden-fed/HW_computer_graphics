#include "Object.h"

Object::Object(MeshModel& mshMdl, Matrix4x4 mdlMtrx) {
	//this->mshMdl = mshMdl;
	this->mdlMtrx = mdlMtrx;
	show_normals = false;
}

Object::~Object()
{
	show_normals = false;
}

void Object::setShowNormals(bool norm)
{
	show_normals = norm;
}
