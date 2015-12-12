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
	//       A0
	//       *
	//      * *
	//     * D *
	//  B1*******C2
	Vector4 D;
	Vector4 AB= vertices[0]- vertices[1];
	Vector4 AC=vertices[0] - vertices[2];
	AB[2] = AC[2] = 0; //3d to 2d
	double B1, B2; //barycentric coordinates
	double areaparallelogramArea = (AB^AC).getSize();
	Coordinate crdToDraw;


	for (int x = minX; x <= maxX; x++) {
		for (int y = minY; y <= maxY; y++) {
			D.setVlaues(x - vertices[0][0], y - vertices[0][1], 0, 1);
			B1 = ((D^AC).getSize()) / areaparallelogramArea;
			B2 = ((D^AB).getSize()) / areaparallelogramArea;

			if (B1 > 0 && B2 > 0 && (B1+B2) <1) {
				crdToDraw.setX(x);
				crdToDraw.setY(y);
				crdVec.push_back(crdToDraw);
			}
		}
	}

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
