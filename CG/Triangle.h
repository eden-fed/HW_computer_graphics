#pragma once
#include "Vector4.h"

class Triangle
{
	Vector4 vertices[3];
	double area;
	Vector4 normal;

public:
	//constructors
	Triangle();
	Triangle(Vector4& v1, Vector4& v2, Vector4& v3);
	Triangle(Vector4& v1, Vector4& v2, Vector4& v3, Vector4& n);

	//destructor
	~Triangle();

	//get functions
	double getArea();
	Vector4 getNormal();

	inline Vector4& operator[](int index) { return vertices[index]; }
	inline const Vector4& operator[](int index) const { return vertices[index]; }

};