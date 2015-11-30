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

void Object::setMtrx(Matrix4x4 mdlMtrx)
{
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

void Object::drawObjectCrdSystem(Matrix4x4 transMtrx)
{
	Line ln;
	Vector4 _XX_YY_ZZ[6];
	int Scl = 50;
	_XX_YY_ZZ[0].setVlaues(-Scl,0,0,1);
	_XX_YY_ZZ[1].setVlaues( Scl, 0, 0, 1);
	_XX_YY_ZZ[2].setVlaues(0,-Scl, 0, 1);
	_XX_YY_ZZ[3].setVlaues(0, Scl, 0, 1);
	_XX_YY_ZZ[4].setVlaues(0, 0,-Scl, 1);
	_XX_YY_ZZ[5].setVlaues(0, 0, Scl, 1);

	for (int i = 0; i < 6; i+=2) {
		_XX_YY_ZZ[i] *= transMtrx;
		_XX_YY_ZZ[i + 1] *= transMtrx;
		ln.setStartCrd(_XX_YY_ZZ[i], 0x000000ff + (0xff000000 >> (4 * i)));
		ln.setEndCrd(_XX_YY_ZZ[i+1], 0x000000ff + (0xff000000 >> (4 * i)));
		ln.drawline();
	}




	ln.drawline();
}
