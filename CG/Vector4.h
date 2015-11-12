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
	bool isColumnVector;

public:
	//constructors
	Vector4();
	Vector4(const int valX, const int valY, const int valZ, const int valW, bool isCVector);

	//copy constructor
	Vector4(const Vector4& V);

	//destructor
	~Vector4();

	//get functions
	const bool getColumnVector()const { return isColumnVector; }

	//set functions
	void setColumnVector(const bool val);

	//class methods
	//const Vector4& dot(const Vector4& V);
	//const Vector4& cross(const Vector4& V);

	//operators
	int &operator[](int i);
	const Vector4& operator = (const Vector4& V);
	const bool operator == (const Vector4& V)const;
	const bool operator != (const Vector4& V)const;
	const Vector4& operator += (const Vector4& V) throw(std::string);
	const Vector4 operator + (const Vector4& V) throw(std::string);
	const Vector4& operator -= (const Vector4& V) throw(std::string);
	const Vector4 operator - (const Vector4& V) throw(std::string);
	const Vector4& operator *= (const Matrix4x4& M) throw(std::string);
	const Vector4 operator * (const Matrix4x4& M) throw(std::string);

};

#endif