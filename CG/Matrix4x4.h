#include <iostream>
#include "Vector4.h"

#ifndef MATRIX4X4_H
#define MATRIX4X4_H

class Matrix4x4
{
private:
	int v1[4][4];

public:
	Matrix4x4();
	Matrix4x4(int x1, int x2, int x3, int x4, int y1, int y2,int y3,int y4, int z1, int z2, int z3, int z4, int w1, int w2, int w3, int w4);
	Matrix4x4(const Matrix4x4& m);

	const int* operator[] (int i) const;
	int* operator[](int i);

	/*int& operator() (int row,int line);
	int operator() (unsigned row, unsigned col) const;*/

	const Matrix4x4& operator = (const Matrix4x4& V);
	const bool operator == (const Matrix4x4& V)const;
	const bool operator != (const Matrix4x4& V)const;
	const Matrix4x4& operator + (const Matrix4x4& V);
	const Matrix4x4& operator - (const Matrix4x4& V);
	const Matrix4x4& operator * (const Matrix4x4& V);

};
#endif
