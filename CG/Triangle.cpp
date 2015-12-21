#include "Triangle.h"

#define B0DX 0
#define B0DY 1
#define B1DX 2
#define B1DY 3
#define B2DX 4
#define B2DY 5
#define IS_FILLED 6

#define X 0
#define Y 1
#define Z 3


Triangle::Triangle()
{
}

Triangle::Triangle(Vector4 & v1, Vector4 & v2, Vector4 & v3)
{
	this->vertices[0] = v1;
	this->vertices[1] = v2;
	this->vertices[2] = v3;
	this->normal = calcNormal();
	this->area = 0;
	memset(gradient, 0.0, sizeof(gradient));
}

Triangle::Triangle(int * v1, int * v2, int * v3)
{
	vertices[0] = Vector4(v1[0], v1[1], v1[2], 1);
	vertices[1] = Vector4(v2[0], v2[1], v2[2], 1);
	vertices[2] = Vector4(v3[0], v3[1], v3[2], 1);
	this->normal = calcNormal();
	memset(gradient, 0.0, sizeof(gradient));
}

Triangle::Triangle(Vector4 & v1, Vector4 & v2, Vector4 & v3, Vector4 & n)
{
	this->vertices[0] = v1;
	this->vertices[1] = v2;
	this->vertices[2] = v3;
	this->normal = n;
	memset(gradient, 0.0, sizeof(gradient));
}

Triangle::~Triangle()
{
}

double Triangle::getArea()
{
	if (this->area == 0) {
		this->area = 0.5*((vertices[0] - vertices[1]).getSize())*((vertices[0] - vertices[2]).getSize());
	}
	return this->area;
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
/*
Vector4 Triangle::getNewBarycentricCrd(Vector4 bCrd, eScanConvMovement M, unsigned int numOfMoves)
{
	//       V1
	//       *
	//      * *
	//     * D *
	//  V0*******V2

	Vector4 retVal = bCrd;

	//The gradient for barecentryc coordinates
	if (gradient[IS_FILLED] == 0.0) { //false=0.0

		gradient[B0DX] = ((*this)[1][Y] - (*this)[2][Y]) / helpGNBC(1, 2, (*this)[0][X], (*this)[0][Y]);
		gradient[B0DY] = ((*this)[2][X] - (*this)[1][X]) / helpGNBC(1, 2, (*this)[0][X], (*this)[0][Y]);

		gradient[B1DX] = ((*this)[2][Y] - (*this)[0][Y]) / helpGNBC(2, 0, (*this)[1][X], (*this)[1][Y]);
		gradient[B1DY] = ((*this)[0][X] - (*this)[2][X]) / helpGNBC(2, 0, (*this)[1][X], (*this)[1][Y]);

		gradient[B2DX] = ((*this)[0][Y] - (*this)[1][Y]) / helpGNBC(0, 1, (*this)[2][X], (*this)[2][Y]);
		gradient[B2DY] = ((*this)[1][X] - (*this)[0][X]) / helpGNBC(0, 1, (*this)[2][X], (*this)[2][Y]);

		gradient[IS_FILLED] = 1.0; //true=1.0
	}


	switch (M) {

	case UP:
		retVal[0] += (gradient[B0DY] * numOfMoves);
		retVal[1] += (gradient[B1DY] * numOfMoves);
		retVal[2] += (gradient[B2DY] * numOfMoves);
		break;
	case RIGHT:
		retVal[0] += (gradient[B0DX] * numOfMoves);
		retVal[1] += (gradient[B1DX] * numOfMoves);
		retVal[2] += (gradient[B2DX] * numOfMoves);
		break;
	}
		return retVal;
}*/

/*
void Triangle::triangleScanConversion(std::vector<stZbufferInfo>& crdVec)
{
	crdVec.clear();

	//bounding rectangle parameters
	float minX, maxX, minY, maxY;

	//find bounding rectangle
	{
		minX = maxX = vertices[0][0];
		minY = maxY = vertices[0][1];
		for (int i = 1; i < 3; i++) {
			if (vertices[i][0] < minX) {
				minX = vertices[i][0];
			}
			if (vertices[i][0] > maxX) {
				maxX = vertices[i][0];
			}
			if (vertices[i][1] < minY) {
				minY = vertices[i][1];
			}
			if (vertices[i][1] > maxY) {
				maxY = vertices[i][1];
			}
		}
	}

	//       V1
	//       *
	//      * *
	//     * D *
	//  V0*******V2


	Vector4 bCrd,firstBaryCrd;
	stZbufferInfo crdInfo;

	firstBaryCrd[0] = helpGNBC(1, 2, (int)minX, (int)minY) / helpGNBC(1, 2, (*this)[0][X], (*this)[0][Y]); // V1V2D / V1V2V0
	firstBaryCrd[1] = helpGNBC(2, 0, (int)minX, (int)minY) / helpGNBC(2, 0, (*this)[1][X], (*this)[1][Y]); // V1V0D / V1V2V0
	firstBaryCrd[2] = 1.0 - (firstBaryCrd[0] + firstBaryCrd[1]);

	for (int x = minX; x <= maxX; x++) {
		bCrd = getNewBarycentricCrd(firstBaryCrd, RIGHT ,x-(int)minX);
		for (int y = minY; y <= maxY; y++) {
			if (bCrd[0] > 0 && bCrd[1] > 0 && bCrd[2] > 0) {
				crdInfo.cartCrd.setX(x);
				crdInfo.cartCrd.setY(y);
				crdInfo.baryCrd = bCrd;
				crdVec.push_back(crdInfo);
			}
			bCrd = getNewBarycentricCrd(bCrd, UP,1);
		}
	}

}
*/
Color Triangle::getColorOfPoint(int x, int y)
{
	return Color(0x00ff00);
}
/*
float Triangle::helpGNBC(int a, int b, float x, float y) 
{
	return (((*this)[a][Y] - (*this)[b][Y])*x) + (((*this)[b][X] - (*this)[a][X])*y) + (*this)[a][X] * (*this)[b][Y] - (*this)[b][X] * (*this)[a][Y];
}*/


/*Vector4 Triangle::getBarycentricCrd(Vector4 CartesianCrd)
{
	//       A0
	//       *
	//      * *
	//     * D *
	//  B1*******C2
	Vector4 retVal;

	//get trangle in 2d
	Vector4 A = vertices[0];
	A[2] = 0;
	Vector4 B = vertices[1];
	B[2] = 0;
	Vector4 C = vertices[2];
	C[2] = 0;

	//calc area
	double areaBAD = 0.5*((A - B).getSize())*((A - CartesianCrd).getSize());
	double areaCAD = 0.5*((A - C).getSize())*((A - CartesianCrd).getSize());
	double areaBDC = 0.5*((B - CartesianCrd).getSize())*((CartesianCrd - C).getSize());
	double areaSum = areaBAD + areaCAD + areaBDC;

	retVal[0] = areaBDC / areaSum;
	retVal[1] = areaCAD / areaSum;
	retVal[2] = areaBAD / areaSum;

	return retVal;
}*/
