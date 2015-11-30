#include "Object.h"

Object::Object(MeshModel& mshMdl, Matrix4x4 mdlMtrx ){
	this->mshMdl = mshMdl;
	this->mdlMtrx = mdlMtrx;
}

Object::Object()
{
}

Object::~Object()
{
}



void Object::setModel(MeshModel & mshMdl, Matrix4x4 mdlMtrx)
{
	this->mshMdl = mshMdl;
	this->mdlMtrx = mdlMtrx;
}

void Object::populatelinesToDraw()
{
	linesToDraw.clear();
	Line ln;
	for (int i = 0; i < mshMdl.getNumberOfFaces(); i++) {
		for (int j = 0; j < 3; j++) {
			ln.setStartCrd(mshMdl.getFace(i)[j],0xff0000ff);
			ln.setEndCrd(mshMdl.getFace(i)[(j+1)%3], 0xff0000ff);
			linesToDraw.insert(ln);
		}
	}
}

void Object::drawObjectTriangles()
{
	Line ln;
	for (int i = 0; i < mshMdl.getNumberOfFaces(); i++) {
		for (int j = 0; j < 3; j++) {
			ln.setStartCrd(mshMdl.getFace(i)[j], 0xff0000ff);
			ln.setEndCrd(mshMdl.getFace(i)[(j + 1) % 3], 0xff0000ff);
			ln.drawline();
		}
	}
}

void Object::drawObject()
{
	populatelinesToDraw();
	Line ln;
	for (std::set<Line>::iterator it = this->linesToDraw.begin(); it != this->linesToDraw.end(); ++it) {
		ln = *it;
		ln.drawline();
	}
}

void Object::drawNormals(double normSize)
{
	Line ln; Vector4 end;
	for (int i = 0; i < mshMdl.getNumberOfNormals(); i++) {
			ln.setStartCrd(mshMdl.getVertex(i), 0xff0000);
			end = mshMdl.getVertex(i) + ((mshMdl.getNormal(i).normalize())*normSize);
			ln.setEndCrd(end, 0xff0000);
			ln.drawline();
		
	}
}
