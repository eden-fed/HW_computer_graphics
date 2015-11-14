#include "Matrix4x4.h"
#include "Vector4.h"

Matrix4x4::Matrix4x4():v1{ {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,1} }
{

}

Matrix4x4::Matrix4x4(double x1, double x2, double x3, double x4, double y1, double y2, double y3, double y4, double z1, double z2, double z3, double z4, double w1, double w2, double w3, double w4=1)
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
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->v1[i][j] = m.v1[i][j];
}

const double*  Matrix4x4::operator[](int i) const
{
	return this->v1[i];
}

double * Matrix4x4::operator[](int i)
{
	return this->v1[i];
}

const Matrix4x4 & Matrix4x4::operator=(const Matrix4x4 & m)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->v1[i][j] = m.v1[i][j];
	
	return *this;
}

const bool Matrix4x4::operator==(const Matrix4x4 & m) const
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (v1[i][j] != m.v1[i][j])
				return false;
	return true;
}

const bool Matrix4x4::operator!=(const Matrix4x4 & m) const
{
	return (!(*this == m));
}

const Matrix4x4  Matrix4x4::operator+(const Matrix4x4 & m) const
{
	Matrix4x4 newMat;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			newMat[i][j] = v1[i][j] + m[i][j];
	return newMat;
}

const Matrix4x4 Matrix4x4::operator-(const Matrix4x4 & m) const
{
	Matrix4x4 newMat;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			newMat[i][j] = v1[i][j] - m[i][j];
	return newMat;
}

const Matrix4x4 Matrix4x4::operator*(const Matrix4x4 & m) const
{
	Matrix4x4 newMat;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			newMat[i][j] = v1[i][0]*m[0][j]+v1[i][1]*m[1][j]+v1[i][2]*m[2][j]+v1[i][3]*m[3][j];
	return newMat;
}

const Vector4 Matrix4x4::operator*(const Vector4 & v) const
{
	Vector4 newV;
	for (int i = 0; i <= 3; i++) {
		for (int j = 0; j <= 3; j++) {
			newV[i] += v1[i][j] * v[j];
		}
	}
	return newV;
}


