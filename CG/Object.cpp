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

void Object::drawObjectCrdSystem(Matrix4x4 objMtrx, Vector4 objCentroid,double x,double y)
{
	Line ln;
	Matrix4x4 TwMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x , y , 0, 1);
	Matrix4x4 ToMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, objCentroid[0], objCentroid[1], 0, 1);
	Vector4 _XX_YY_ZZ1[6];
	Vector4 _XX_YY_ZZ2[6];
	int Scl = 50;
	_XX_YY_ZZ1[0].setVlaues(-Scl,0,0,1);
	_XX_YY_ZZ1[1].setVlaues( Scl, 0, 0, 1);
	_XX_YY_ZZ1[2].setVlaues(0,-Scl, 0, 1);
	_XX_YY_ZZ1[3].setVlaues(0, Scl, 0, 1);
	_XX_YY_ZZ1[4].setVlaues(0, 0,-Scl, 1);
	_XX_YY_ZZ1[5].setVlaues(0, 0, Scl, 1);

	for (int j = 0; j < 6; j++) {
		_XX_YY_ZZ2[j] = _XX_YY_ZZ1[j];
	}

	//world coordinates
	for (int i = 0; i < 6; i+=2) {
		_XX_YY_ZZ1[i] *= TwMatrix;
		_XX_YY_ZZ1[i + 1] *= TwMatrix;
		ln.setStartCrd(_XX_YY_ZZ1[i], 0x000000ff + (0xff000000 >> (4 * i)));
		ln.setEndCrd(_XX_YY_ZZ1[i+1], 0x000000ff + (0xff000000 >> (4 * i)));
		ln.drawline();
	}

	//object coordinates
	for (int i = 0; i < 6; i += 2) {
		_XX_YY_ZZ2[i] *= (objMtrx*ToMatrix);
		_XX_YY_ZZ2[i + 1] *= (objMtrx*ToMatrix);
		ln.setStartCrd(_XX_YY_ZZ2[i], 0x000000ff + (0xff000000 >> (4 * i)));
		ln.setEndCrd(_XX_YY_ZZ2[i + 1], 0x000000ff + (0xff000000 >> (4 * i)));
		ln.drawline();
	}
}
