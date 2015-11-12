#pragma once
#include <string>
#include "Matrix4x4.h"

#ifndef VECTOR4_H
#define VECTOR4_H

class Vector4
{
private:
	//assighning coordinate variables
	int vectorValues[4];
	bool columnVector;

public:
	//constructors
	Vector4();
	Vector4(const int valX, const int valY, const int valZ, const int valW, bool isColumnVector);

	//copy constructor
	Vector4(const Vector4& V);

	//destructor
	~Vector4();

	//class methods
	//const Vector4& dot(const Vector4& V);
	//const Vector4& cross(const Vector4& V);

	//operators
	int &operator[](int i);
	const Vector4& operator = (const Vector4& V);
	const bool operator == (const Vector4& V)const;
	const bool operator != (const Vector4& V)const;
	const Vector4& operator + (const Vector4& V) throw(std::string);
	const Vector4& operator - (const Vector4& V);
	const Vector4& operator * (const Matrix4x4& M);

	//const Vector4& operator * (const Vector4& V);

};

#endif
