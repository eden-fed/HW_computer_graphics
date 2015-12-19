#include "Triangle.h"

#define B0DX 0
#define B0DY 1
#define B1DX 2
#define B1DY 3
#define B2DX 4
#define B2DY 5
#define IS_FILLED 6

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

Vector4 Triangle::getNewBarycentricCrd(Vector4 bCrd, eScanConvMovement M)
{	
	//       V1
	//       *
	//      * *
	//     * D *
	//  V0*******V2

	Vector4 retVal = bCrd;

	//The gradient for barecentryc coordinates
	if (gradient[IS_FILLED] == 0.0) { //false=0.0
		gradient[B0DX] = ((*this)[2][1] - (*this)[1][1])*(1 / this->getArea());
		gradient[B0DY] = ((*this)[1][0] - (*this)[2][0])*(1 / this->getArea());

		gradient[B1DX] = ((*this)[0][1] - (*this)[2][1])*(1 / this->getArea());
		gradient[B1DY] = ((*this)[2][0] - (*this)[0][0])*(1 / this->getArea());

		gradient[B2DX] = ((*this)[1][1] - (*this)[0][1])*(1 / this->getArea());
		gradient[B2DY] = ((*this)[0][0] - (*this)[1][0])*(1 / this->getArea());

		gradient[IS_FILLED]=1.0; //true=1.0
	}


	switch (M) {

	case UP:
		retVal[0] += gradient[B0DY];
		retVal[1] += gradient[B1DY];
		retVal[2] += gradient[B2DY];
			break;
	case RIGHT:
		retVal[0] += gradient[B0DX];
		retVal[1] += gradient[B1DX];
		retVal[2] += gradient[B2DX];
		break;
	case DOWN:
		retVal[0] -= gradient[B0DY];
		retVal[1] -= gradient[B1DY];
		retVal[2] -= gradient[B2DY];
			break;
	case LEFT:
		retVal[0] -= gradient[B0DX];
		retVal[1] -= gradient[B1DX];
		retVal[2] -= gradient[B2DX];
		break;
	}

	return retVal;
	
}

void Triangle::triangleScanConversion(std::vector<Coordinate>& crdVec)
{
	crdVec.clear();

	//bounding rectangle parameters
	double minX, maxX, minY, maxY;

	//find bounding rectangle
	{
		minX = maxX = vertices[0][0];
		minY = maxY = vertices[0][0];
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
	Vector4 V1D;
	Vector4 V1V0= vertices[0]- vertices[1];
	Vector4 V1V2=vertices[0] - vertices[2];
	V1V0[2] = V1V2[2] = 0; //3d to 2d
	Vector4 bCrd;
	Coordinate crdToDraw;

	V1D.setVlaues(minX - vertices[0][0], minY - vertices[0][1], 0, 1);
	bCrd[0] = (((V1D^V1V2).getSize())/2) / this->getArea(); // V1V2D / V1V2V3
	bCrd[1] = ((V1D^V1V0).getSize() / 2) / this->getArea(); // V1V0D / V1V2V3
	bCrd[2] = bCrd[0] + bCrd[1];


	for (int x = minX; x <= maxX; x++) {
		getNewBarycentricCrd(bCrd, RIGHT);
		for (int y = minY; y <= maxY; y++) {
			getNewBarycentricCrd(bCrd, UP);
			if (bCrd[0] > 0 && bCrd[1] > 0 && bCrd[2] <1) {
				crdToDraw.setX(x);
				crdToDraw.setY(y);
				crdVec.push_back(crdToDraw);
			}
		}
	}

}

Color Triangle::getColorOfPoint(int x, int y)
{
	return Color(0xff0000ff);
}


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
