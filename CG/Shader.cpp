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

void Shader::draw(MeshModel& mesh, Color ambientLight, Light& light1, Light& light2, Z_Buffer& zBuffer, Vector4& eyePosition)
{
	switch (ShadingType) {
	case FLAT:
		flatShading(mesh, ambientLight, light1, light2, zBuffer, eyePosition);
		break;
	case GOURAUD:
		gouraudShading(mesh, ambientLight, light1, light2, zBuffer, eyePosition);
		break;
	case PHONG:
		phongShading(mesh, ambientLight, light1, light2, zBuffer, eyePosition);
		break;
	}
}

void Shader::flatShading(MeshModel& mesh, Color ambientLight, Light& light1, Light& light2, Z_Buffer& zBuffer, Vector4& eyePosition)
{
		for (int i = 0; i < mesh.getAllFaces().size(); i++) {
			//clr.setColor(clr.getColor() + 0x05050500);
			Triangle& T = mesh.getAllFaces()[i];

			Color clr = getFlatColor(T, mesh.material, ambientLight, light1, light2, eyePosition);

			//bounding rectangle parameters
			double minX, maxX, minY, maxY;

			//find bounding rectangle
			T.calcBoundingRectangle(minX, maxX, minY, maxY);

			//       V1
			//       *
			//      * *
			//     * D *
			//  V0*******V2

			Vector4 bCrd, firstBaryCrd;
			stZbufferInfo crdInfo;

			//get the firs barycentric coordinit (low left)
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

void Shader::gouraudShading(MeshModel& mesh, Color ambientLight, Light& light1, Light& light2, Z_Buffer & zBuffer, Vector4& eyePosition)
{
	for (int i = 0; i < mesh.getAllFaces().size(); i++) {
		Triangle& T = mesh.getAllFaces()[i];

		//calculate Trinagle vertex colors
		Color clr0 = getVertxColor(T.getVertexInfo(0), mesh.material, ambientLight, light1, light2, eyePosition);
		Color clr1 = getVertxColor(T.getVertexInfo(1), mesh.material, ambientLight, light1, light2, eyePosition);
		Color clr2 = getVertxColor(T.getVertexInfo(2), mesh.material, ambientLight, light1, light2, eyePosition);


		//bounding rectangle parameters
		double minX, maxX, minY, maxY;

		//find bounding rectangle
		T.calcBoundingRectangle(minX, maxX, minY, maxY);

		//       V1
		//       *
		//      * *
		//     * D *
		//  V0*******V2

		Vector4 bCrd, firstBaryCrd;
		stZbufferInfo crdInfo;

		//get the firs barycentric coordinit (low left)
		firstBaryCrd[0] = helpGNBC(T, 1, 2, (int)minX, (int)minY) / helpGNBC(T, 1, 2, T[0][X], T[0][Y]); // V1V2D / V1V2V0
		firstBaryCrd[1] = helpGNBC(T, 2, 0, (int)minX, (int)minY) / helpGNBC(T, 2, 0, T[1][X], T[1][Y]); // V1V0D / V1V2V0
		firstBaryCrd[2] = 1.0 - (firstBaryCrd[0] + firstBaryCrd[1]);

		for (int x = minX; x <= maxX; x++) {
			bCrd = getNewBarycentricCrd(T, firstBaryCrd, RIGHT, x - (int)minX);
			for (int y = minY; y <= maxY; y++) {
				if (bCrd[0] > 0 && bCrd[1] > 0 && bCrd[2] > 0) {
					crdInfo.cartCrd.setX(x);
					crdInfo.cartCrd.setY(y);
					crdInfo.baryCrd = bCrd;

					//calculate interpulated color
					Color finalColor(clr0.getRedPortion()*bCrd[0] + clr1.getRedPortion()*bCrd[1] + clr2.getRedPortion()*bCrd[2],
									clr0.getGreenPortion()*bCrd[0] + clr1.getGreenPortion()*bCrd[1] + clr2.getGreenPortion()*bCrd[2],
									clr0.getBluePortion()*bCrd[0] + clr1.getBluePortion()*bCrd[1] + clr2.getBluePortion()*bCrd[2]);

					crdInfo.clr.setColor(finalColor.getColor());
					crdInfo.trl = T;

					zBuffer.FillPixelInBuffer(crdInfo);
				}
				bCrd = getNewBarycentricCrd(T, bCrd, UP, 1);
			}
		}
	}
}

void Shader::phongShading(MeshModel& mesh, Color ambientLight, Light& light1, Light& light2, Z_Buffer & zBuffer, Vector4& eyePosition)
{
	//loop through all the triangles
	for (int i = 0; i < mesh.getAllFaces().size(); i++) {

		Triangle& T = mesh.getAllFaces()[i];

		//bounding rectangle parameters
		double minX, maxX, minY, maxY;

		//find bounding rectangle
		T.calcBoundingRectangle(minX, maxX, minY, maxY);

		//       V1
		//       *
		//      * *
		//     * D *
		//  V0*******V2

		Vector4 bCrd, firstBaryCrd;
		stZbufferInfo crdInfo;

		//get the firs barycentric coordinit (low left)
		firstBaryCrd[0] = helpGNBC(T, 1, 2, (int)minX, (int)minY) / helpGNBC(T, 1, 2, T[0][X], T[0][Y]); // V1V2D / V1V2V0
		firstBaryCrd[1] = helpGNBC(T, 2, 0, (int)minX, (int)minY) / helpGNBC(T, 2, 0, T[1][X], T[1][Y]); // V1V0D / V1V2V0
		firstBaryCrd[2] = 1.0 - (firstBaryCrd[0] + firstBaryCrd[1]);

		for (int x = minX; x <= maxX; x++) {
			bCrd = getNewBarycentricCrd(T, firstBaryCrd, RIGHT, x - (int)minX);
			for (int y = minY; y <= maxY; y++) {
				if (bCrd[0] > 0 && bCrd[1] > 0 && bCrd[2] > 0) {
					crdInfo.cartCrd.setX(x);
					crdInfo.cartCrd.setY(y);
					crdInfo.baryCrd = bCrd;

					vertexInfo vInfo;
					//interpolated point
					vInfo.vertex = T[0] * bCrd[0] + T[1] * bCrd[1] + T[2] * bCrd[2];

					//interpolated normal and normilization
					vInfo.normal = T.getNormal(0) * bCrd[0] + T.getNormal(1) * bCrd[1] + T.getNormal(2) * bCrd[2];
					vInfo.normal = vInfo.normal.normalize();

					//claculate the color
					Color clr = getVertxColor(vInfo, mesh.material, ambientLight, light1, light2, eyePosition);

					crdInfo.clr.setColor(clr.getColor());
					crdInfo.trl = T;

					zBuffer.FillPixelInBuffer(crdInfo);
				}
				bCrd = getNewBarycentricCrd(T, bCrd, UP, 1);
			}
		}
	}
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

double Shader::helpGNBC(Triangle& T, int a, int b, double x, double y)
{
	return ((T[a][Y] - T[b][Y])*x) + ((T[b][X] - T[a][X])*y) + T[a][X] * T[b][Y] - T[b][X] * T[a][Y];
}

Color Shader::getFlatColor(Triangle & T, Material& M, Color& ambientLight, Light & light1, Light & light2, Vector4& eyePosition)
{
	vertexInfo vInfo;

	vInfo.vertex = ((T[0] + T[1]) + T[2]) / 3.0; //tCentroid
	vInfo.normal = T.getNormal();
	return getVertxColor(vInfo, M, ambientLight, light1, light2, eyePosition);
}

Color Shader::getVertxColor(vertexInfo& vInfo, Material & M, Color & ambientLight, Light & light1, Light & light2, Vector4& eyePosition)
{
	double A = M.getAmbient();
	Vector4& P = vInfo.vertex;
	Vector4& N = vInfo.normal.normalize();

	Color ambient(ambientLight.getRedPortion()*A, ambientLight.getGreenPortion()*A, ambientLight.getBluePortion()*A);

	Color diffuse1  = clacDiffuseLight(P, N, light1, M.getDiffuse());
	Color diffuse2  = clacDiffuseLight(P, N, light2, M.getDiffuse());

	Color specular1 = clacSpecularLight(P, N, light1, M.getSpecular(), M.getspecularExp(), eyePosition);
	Color specular2 = clacSpecularLight(P, N, light2, M.getSpecular(), M.getspecularExp(), eyePosition);

	Color color((ambient.getRedPortion() + diffuse1.getRedPortion() + diffuse2.getRedPortion() + specular1.getRedPortion() + specular2.getRedPortion()),
		(ambient.getGreenPortion() + diffuse1.getGreenPortion() + diffuse2.getGreenPortion() + specular1.getGreenPortion() + specular2.getGreenPortion()),
		(ambient.getBluePortion() + diffuse1.getBluePortion() + diffuse2.getBluePortion() + specular1.getBluePortion() + specular2.getBluePortion()));
	return color;
}

Color Shader::clacDiffuseLight(Vector4& point, Vector4 & normal, Light& light, double Kd)
{

	Vector4& N =normal;
	Vector4 L;

	//get light direction
	if (light.getType() == _DIRECTION)
		L = light.getPosition() - light.getDirection();
	else
		L = light.getPosition() - point;
	//normalize light
	L = L.normalize();

	//calc NL
	double NL = (N*L);
	NL = (NL) < 0 ? (0) : (NL);

	//equation from class
	Color& Ip = light.getIntensity();
	Color retColor(Kd*NL*Ip.getRedPortion(), Kd*NL*Ip.getGreenPortion(), Kd*NL*Ip.getBluePortion());
	return retColor;
}

Color Shader::clacSpecularLight(Vector4& point, Vector4& normal, Light& light, double Ks, double specularExp, Vector4& eyePosition)
{
	Vector4 N = normal;
	Vector4 L;
	//get light direction
	if (light.getType() == _DIRECTION)
		L = light.getPosition() - light.getDirection();
	else
		L = light.getPosition() - point;

	//normalize light
	L = L.normalize();

	double NL = (N*L);
	NL = (NL) < 0 ? (0) : (NL);

	//calculate R
	Vector4 R = (N * (NL*2)) - L;
	R = R.normalize();

	//calculate V
	Vector4 V = eyePosition-point; 
	V = V.normalize();

	double RV = (R*V);
	RV = pow(RV, specularExp);

	Color& Ip = light.getIntensity();

	Color retColor(Ks*RV*Ip.getRedPortion(), Ks*RV*Ip.getGreenPortion(), Ks*RV*Ip.getBluePortion());
	return retColor;
}
