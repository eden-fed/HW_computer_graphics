
#ifndef MATRIX4X4_H
#define MATRIX4X4_H
#include <iostream>
class Vector4;

class Matrix4x4
{
private:
	double v1[4][4];

public:
	Matrix4x4();
	Matrix4x4(double x1, double x2, double x3, double x4, double y1, double y2, double y3, double y4, double z1, double z2, double z3, double z4, double w1, double w2, double w3, double w4);
	Matrix4x4(const Matrix4x4& m);

	const double* operator[] (int i) const;
	double* operator[](int i);

	const Matrix4x4& operator = (const Matrix4x4& m);
	const bool operator == (const Matrix4x4& m)const;
	const bool operator != (const Matrix4x4& m)const;
	const Matrix4x4 operator + (const Matrix4x4& m) const;
	const Matrix4x4 operator - (const Matrix4x4& m) const;
	const Matrix4x4 operator * (const Matrix4x4& m) const;
	/*const Vector4 operator * (const Vector4& v) const;*/
	


};
#endif
