#include "Object.h"

Object::Object(MeshModel& mshMdl, Matrix4x4 mdlMtrx ){
	this->mshMdl = mshMdl;
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

void Object::populatelinesToDraw()
{
	linesToDraw.clear();
	Line ln;
	for (int i = 0; i < mshMdl.getNumberOfTriangles(); i++) {
		for (int j = 0; j < 3; j++) {
			ln.setStartCrd(mshMdl.getTriangle(i)[j],0xff0000ff);
			ln.setEndCrd(mshMdl.getTriangle(i)[(j+1)%3], 0xff0000ff);
			linesToDraw.insert(ln);
		}
	}
}

void Object::drawObjectTriangles()
{
	Line ln;
	for (int i = 0; i < mshMdl.getNumberOfTriangles(); i++) {
		for (int j = 0; j < 3; j++) {
			ln.setStartCrd(mshMdl.getTriangle(i)[j], 0xff0000ff);
			ln.setEndCrd(mshMdl.getTriangle(i)[(j + 1) % 3], 0xff0000ff);
			ln.drawline();
		}
	}
}

void Object::drawObject()
{
	populatelinesToDraw();
	std::cout << "in drawObject!! "<< std::endl;
	Line ln;
	for (std::set<Line>::iterator it = this->linesToDraw.begin(); it != this->linesToDraw.end(); ++it) {
		ln = *it;
		ln.drawline();
	}
}
