#include "Shader.h"

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



Shader::Shader()
{
}

Shader::Shader(eShadingType sType)
{
	this->ShadingType = sType;
}

Shader::~Shader()
{
}

void Shader::draw(std::vector<MeshModel>& meshVec, Light& light1, Light& light2, Z_Buffer& zBuffer)
{
	switch (ShadingType) {
	case FLAT:
		flatShading(meshVec, light1, light2, zBuffer);
		break;
	case GOURAUD:
		gouraudShading(meshVec, light1, light2, zBuffer);
		break;
	case PHONG:
		phongShading(meshVec, light1, light2, zBuffer);
		break;
	}
}

void Shader::flatShading(std::vector<MeshModel>& meshVec, Light& light1, Light& light2, Z_Buffer& zBuffer)
{
	Color clr(0x0FFF00);
	for (int i = 0; i < meshVec.size(); i++) {
		for (int j = 0; j < meshVec[i].getAllFaces().size(); j++) {
			//clr.setColor(clr.getColor() + 0x05050500);
			Triangle& T = meshVec[i].getAllFaces()[j];

			//bounding rectangle parameters
			float minX, maxX, minY, maxY;

			//find bounding rectangle
			T.calcBoundingRectangle(minX, maxX, minY, maxY);

			//       V1
			//       *
			//      * *
			//     * D *
			//  V0*******V2

			Vector4 bCrd, firstBaryCrd;
			stZbufferInfo crdInfo;

			firstBaryCrd[0] = helpGNBC(T,1, 2, (int)minX, (int)minY) / helpGNBC(T, 1, 2, T[0][X], T[0][Y]); // V1V2D / V1V2V0
			firstBaryCrd[1] = helpGNBC(T, 2, 0, (int)minX, (int)minY) / helpGNBC(T, 2, 0, T[1][X], T[1][Y]); // V1V0D / V1V2V0
			firstBaryCrd[2] = 1.0 - (firstBaryCrd[0] + firstBaryCrd[1]);

			for (int x = minX; x <= maxX; x++) {
				bCrd = getNewBarycentricCrd(T,firstBaryCrd, RIGHT, x - (int)minX);
				for (int y = minY; y <= maxY; y++) {
					if (bCrd[0] > 0 && bCrd[1] > 0 && bCrd[2] > 0) {
						crdInfo.cartCrd.setX(x);
						crdInfo.cartCrd.setY(y);
						crdInfo.baryCrd = bCrd;
						crdInfo.clr.setColor(clr.getColor());
						crdInfo.trl = T;

						zBuffer.FillPixelInBuffer(crdInfo);
					}
					bCrd = getNewBarycentricCrd(T, bCrd, UP, 1);
				}
			}
		}
	}
}

void Shader::gouraudShading(std::vector<MeshModel>& meshVec, Light& light1, Light& light2, Z_Buffer & zBuffer)
{
}

void Shader::phongShading(std::vector<MeshModel>& meshVec, Light& light1, Light& light2, Z_Buffer & zBuffer)
{
}

Vector4 Shader::getNewBarycentricCrd(Triangle& T, Vector4& bCrd, eScanConvMovement M, unsigned int numOfMoves)
{
	//       V1
	//       *
	//      * *
	//     * D *
	//  V0*******V2

	Vector4 retVal = bCrd;

	//The gradient for barecentryc coordinates
	if (T.gradient[IS_FILLED] == 0.0) { //false=0.0

		T.gradient[B0DX] = (T[1][Y] - T[2][Y]) / helpGNBC(T,1, 2, T[0][X], T[0][Y]);
		T.gradient[B0DY] = (T[2][X] - T[1][X]) / helpGNBC(T, 1, 2, T[0][X], T[0][Y]);

		T.gradient[B1DX] = (T[2][Y] - T[0][Y]) / helpGNBC(T, 2, 0, T[1][X], T[1][Y]);
		T.gradient[B1DY] = (T[0][X] - T[2][X]) / helpGNBC(T, 2, 0, T[1][X], T[1][Y]);

		T.gradient[B2DX] = (T[0][Y] - T[1][Y]) / helpGNBC(T, 0, 1, T[2][X], T[2][Y]);
		T.gradient[B2DY] = (T[1][X] - T[0][X]) / helpGNBC(T, 0, 1, T[2][X], T[2][Y]);

		T.gradient[IS_FILLED] = 1.0; //true=1.0
	}


	switch (M) {

	case UP:
		retVal[0] += (T.gradient[B0DY] * numOfMoves);
		retVal[1] += (T.gradient[B1DY] * numOfMoves);
		retVal[2] += (T.gradient[B2DY] * numOfMoves);
		break;
	case RIGHT:
		retVal[0] += (T.gradient[B0DX] * numOfMoves);
		retVal[1] += (T.gradient[B1DX] * numOfMoves);
		retVal[2] += (T.gradient[B2DX] * numOfMoves);
		break;
	}
	return retVal;
}

float Shader::helpGNBC(Triangle& T, int a, int b, float x, float y)
{
	return ((T[a][Y] - T[b][Y])*x) + ((T[b][X] - T[a][X])*y) + T[a][X] * T[b][Y] - T[b][X] * T[a][Y];
}
