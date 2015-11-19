
#ifndef VECTOR4_H
#define VECTOR4_H
#include <string>
#include "Matrix4x4.h"

//row vector
class Vector4
{
private:
	//assighning coordinate variables
	double vectorValues[4];

public:
	//constructors
	Vector4();
	Vector4(const double valX, const double valY, const double valZ, const double valW);

	//copy constructor
	Vector4(const Vector4& V);

	//destructor
	~Vector4();

	//class methods
	//const Vector4& dot(const Vector4& V);
	//const Vector4& cross(const Vector4& V);

	//operators
	double &operator[](int i);
	const double &operator[](int i) const;
	const Vector4& operator = (const Vector4& V);
	const bool operator == (const Vector4& V)const;
	const bool operator != (const Vector4& V)const;
	const Vector4& operator += (const Vector4& V);
	const Vector4 operator + (const Vector4& V) ;
	const Vector4& operator -= (const Vector4& V) ;
	const Vector4 operator - (const Vector4& V) ;

	double operator*(const Vector4& V); //Dot product 
	Vector4 operator^(const Vector4& V); //Cross product
	Vector4 operator*(const double num); //scalar*vector

	const Vector4& operator *= (const Matrix4x4& M) ;//vector*matrix
	const Vector4 operator * (const Matrix4x4& M);

};

#endif
