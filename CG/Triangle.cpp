#include "Triangle.h"

Triangle::Triangle()
{
}

Triangle::Triangle(Vector4 & v1, Vector4 & v2, Vector4 & v3)
{
	this->vertices[0] = v1;
	this->vertices[1] = v2;
	this->vertices[2] = v3;
	this->normal = calcNormal();
}

Triangle::Triangle(int * v1, int * v2, int * v3)
{
	vertices[0] = Vector4(v1[0], v1[1], v1[2], 1);
	vertices[1] = Vector4(v2[0], v2[1], v2[2], 1);
	vertices[2] = Vector4(v3[0], v3[1], v3[2], 1);
	this->normal = calcNormal();
}

Triangle::Triangle(Vector4 & v1, Vector4 & v2, Vector4 & v3, Vector4 & n)
{
	this->vertices[0] = v1;
	this->vertices[1] = v2;
	this->vertices[2] = v3;
	this->normal = n;
}

Triangle::~Triangle()
{
}

double Triangle::getArea()
{
	return 0.5*((vertices[0] - vertices[1]).getSize())*((vertices[0] - vertices[2]).getSize());
}

Vector4 Triangle::getNormal()
{
	return this->normal;
}

Vector4 Triangle::calcNormal()
{
	return (vertices[1] - vertices[0])^(vertices[2] - vertices[0]);
}

const bool Triangle::isVertexInTriangle(Vector4 & V)
{
	for (int i = 0; i < 3; i++) {
		if (V == vertices[i]) {
			return true;
		}
	}
	return false;
}
