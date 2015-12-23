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

void Shader::draw(std::vector<MeshModel>& meshVec, Color ambientLight, Light& light1, Light& light2, Z_Buffer& zBuffer)
{
	switch (ShadingType) {
	case FLAT:
		flatShading(meshVec, ambientLight, light1, light2, zBuffer);
		break;
	case GOURAUD:
		gouraudShading(meshVec, ambientLight, light1, light2, zBuffer);
		break;
	case PHONG:
		phongShading(meshVec, ambientLight, light1, light2, zBuffer);
		break;
	}
}

void Shader::flatShading(std::vector<MeshModel>& meshVec, Color ambientLight, Light& light1, Light& light2, Z_Buffer& zBuffer)
{
	for (int i = 0; i < meshVec.size(); i++) {
		for (int j = 0; j < meshVec[i].getAllFaces().size(); j++) {
			//clr.setColor(clr.getColor() + 0x05050500);
			Triangle& T = meshVec[i].getAllFaces()[j];

			Color clr = getFlatColor(T, meshVec[i].material, ambientLight, light1, light2);

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

void Shader::gouraudShading(std::vector<MeshModel>& meshVec, Color ambientLight, Light& light1, Light& light2, Z_Buffer & zBuffer)
{
}

void Shader::phongShading(std::vector<MeshModel>& meshVec, Color ambientLight, Light& light1, Light& light2, Z_Buffer & zBuffer)
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

Color Shader::getFlatColor(Triangle & T, Material& M, Color& ambientLight, Light & light1, Light & light2)
{
	
	double A = M.getAmbient();
	Vector4 tCentroid = (T[0] + T[1] + T[2])/3;

	Color ambient(ambientLight.getRedPortion()*A, ambientLight.getGreenPortion()*A, ambientLight.getBluePortion()*A);

	Color diffuse1 = clacDiffuseLight(tCentroid, T.getNormal(), light1, M.getDiffuse());
	Color specular1 = clacSpecularLight(tCentroid, T.getNormal(), light1, M.getSpecular(), M.getspecularExp());
	Color diffuse2 = clacDiffuseLight(tCentroid, T.getNormal(), light2, M.getDiffuse());
	Color specular2 = clacSpecularLight(tCentroid, T.getNormal(), light2, M.getSpecular(), M.getspecularExp());
	Color color((ambient.getRedPortion() + diffuse1.getRedPortion() + diffuse2.getRedPortion() + specular1.getRedPortion() + specular2.getRedPortion()),
				(ambient.getGreenPortion() + diffuse1.getGreenPortion() + diffuse2.getGreenPortion() + specular1.getGreenPortion() + specular2.getGreenPortion()),
				(ambient.getBluePortion() + diffuse1.getBluePortion() + diffuse2.getBluePortion() + specular1.getBluePortion() + specular2.getBluePortion()));
	return color;
}

Color Shader::clacDiffuseLight(Vector4 & point, Vector4 & norm, Light & light, double Kdiffuse)
{
	Color tmp_color;
	Vector4 N = norm;
	Vector4 L = light.getPosition() - point;
	if (light.getType() == DIRECTION)
		L = light.getPosition() - light.getDirection();
	L = L*(1 / L.getSize());
	N[3] = 0; L[3] = 0;
	double factor = (N*L);
	if (factor<0)factor = 0;
	tmp_color.setColor(Kdiffuse*factor*light.getIntensity().getRedPortion(), Kdiffuse*factor*light.getIntensity().getGreenPortion(), Kdiffuse*factor*light.getIntensity().getBluePortion());
	return tmp_color;
}

Color Shader::clacSpecularLight(Vector4 & point, Vector4 & norm, Light & light, double Kspecular, double specularExp)
{
	Color tmp_color;
	Vector4 L = light.getPosition() - point;
	if (light.getType() == DIRECTION)
		L = light.getPosition() - light.getDirection();
	L = L*(1 / L.getSize());
	Vector4 N = norm;
	N[3] = 0; L[3] = 0;
	double dotNL = (N*L);
	if (dotNL<0) dotNL = 0;
	Vector4 V = N*(2 * (dotNL)) - L;
	V = V*(1 / V.getSize());
	Vector4 R(0, 0, -1, 1);
	double factor = (R*V);
	factor = pow(factor, specularExp);
	tmp_color.setColor(Kspecular*factor*light.getIntensity().getRedPortion(), Kspecular*factor*light.getIntensity().getGreenPortion(), Kspecular*factor*light.getIntensity().getBluePortion());
	return tmp_color;
}