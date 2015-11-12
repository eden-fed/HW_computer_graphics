#include "Matrix4x4.h"

Matrix4x4::Matrix4x4():v1{ {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,1} }
{

}

Matrix4x4::Matrix4x4(int x1, int x2, int x3, int x4, int y1, int y2, int y3, int y4, int z1, int z2, int z3, int z4, int w1, int w2, int w3, int w4)
{
	v1[0][0] = x1;
	v1[0][1] = x2;
	v1[0][2] = x3;
	v1[0][2] = x3;
	v1[1][0] = y1;
	v1[1][1] = y2;
	v1[1][2] = y3;
	v1[1][2] = y3;
	v1[2][0] = z1;
	v1[2][1] = z2;
	v1[2][2] = z3;
	v1[2][2] = z3;
	v1[3][0] = w1;
	v1[3][1] = w2;
	v1[3][2] = w3;
	v1[3][2] = w3;
}

Matrix4x4::Matrix4x4(const Matrix4x4 & m)
{
}

const int*  Matrix4x4::operator[](int i) const
{
	return this->v1[i];
}

int * Matrix4x4::operator[](int i)
{
	return this->v1[i];
}

const Matrix4x4 & Matrix4x4::operator=(const Matrix4x4 & V)
{
	
}
