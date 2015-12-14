#pragma once
#include "Vector4.h"
#include "Coordinate.h"
#include "Color.h"

class Triangle
{
	Vector4 vertices[3];
	double area;
	Vector4 normal;

public:
	//constructors
	Triangle();
	Triangle(Vector4& v1, Vector4& v2, Vector4& v3);
	Triangle(int* v1, int* v2, int* v3);
	Triangle(Vector4& v1, Vector4& v2, Vector4& v3, Vector4& n);

	//destructor
	~Triangle();

	//get functions
	double getArea();
	Vector4 getNormal();

	inline Vector4& operator[](int index) { return vertices[index]; }
	inline const Vector4& operator[](int index) const { return vertices[index]; }

	//class methods
	Vector4 calcNormal();
	const bool isVertexInTriangle(Vector4& V);
	void triangleScanConversion(std::vector<Coordinate>& crdVec);
	Color getColorOfPoint(int x, int y);

};