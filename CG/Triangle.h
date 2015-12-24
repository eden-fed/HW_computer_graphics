#pragma once
#include "Vector4.h"
#include "Coordinate.h"
#include "Color.h"

typedef struct {
	Vector4 vertex;
	Vector4 normal;
}vertexInfo;

class Triangle
{
	vertexInfo vertices[3];
	double area;
	Vector4 normal;

public:
	float gradient[7];
	//constructors
	Triangle();
	Triangle(Vector4& v1, Vector4& v2, Vector4& v3);
	Triangle(Vector4& v1, Vector4& v2, Vector4& v3, Vector4& n1, Vector4& n2, Vector4& n3);
	Triangle(int* v1, int* v2, int* v3, int * n1, int * n2, int * n3);
	Triangle(int* v1, int* v2, int* v3);

	//Triangle(Vector4& v1, Vector4& v2, Vector4& v3, Vector4& n);

	const Triangle & operator=(const Triangle & V);

	//destructor
	~Triangle();

	//get functions
	double getArea();
	Vector4 getNormal();
	Vector4 getCrossNormal();
	vertexInfo& getVertexInfo(unsigned int index);


	inline Vector4& operator[](int index) { return vertices[index].vertex; }
	inline const Vector4& operator[](int index) const { return vertices[index].vertex; }
	inline Vector4& getNormal(int index) { return vertices[index].normal; }
	void setNormal(Vector4 n);

	//class methods
	Vector4 calcNormal();
	const int isVertexInTriangle(Vector4& V);
	//Vector4 getNewBarycentricCrd(Vector4 bCrd, eScanConvMovement M, unsigned int numOfMoves);
	//void triangleScanConversion(std::vector<stZbufferInfo>& crdVec);
	Color getColorOfPoint(int x, int y);
	void calcBoundingRectangle(float& minX, float& maxX, float& minY, float& maxY);
	//float helpGNBC(int a, int b, float x, float y);

};