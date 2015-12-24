#include "Object.h"

Object::Object(MeshModel& mshMdl, Matrix4x4& mdlMtrx ){
	this->mshMdl = mshMdl;
	this->mdlMtrx = mdlMtrx;
}

Object::Object()
{
}

Object::~Object()
{
}



void Object::setModel(MeshModel & mshMdl, Matrix4x4& mdlMtrx)
{
	this->mshMdl = mshMdl;
	this->mdlMtrx = mdlMtrx;
}

void Object::setMtrx(Matrix4x4 mdlMtrx)
{
	this->mdlMtrx = mdlMtrx;
}

void Object::drawObjectCrdSystem(Matrix4x4& objMtrx, Vector4& objCentroid,double x,double y)
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
