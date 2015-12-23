#pragma once
#ifndef SHADER_H
#define SHADER_H
#include "Object.h"
#include "MeshModel.h"
#include "Light.h"
#include "Triangle.h"
#include "Z_Buffer.h"

typedef enum {
	FLAT,
	GOURAUD,
	PHONG
} eShadingType;

class Shader
{
	typedef enum {
		UP,
		RIGHT
	} eScanConvMovement;
public:
	eShadingType ShadingType;


	//constructors
	Shader();
	Shader(eShadingType sType);

	//destructor
	~Shader();

	//class methods
	void draw(std::vector<MeshModel>& meshVec,Color ambientLight, Light& light1, Light& light2,Z_Buffer& zBuffer);

	void flatShading(std::vector<MeshModel>& meshVec, Color ambientLight,Light& light1, Light& light2, Z_Buffer& zBuffer);
	void gouraudShading(std::vector<MeshModel>& meshVec, Color ambientLight, Light& light1, Light& light2, Z_Buffer& zBuffer);
	void phongShading(std::vector<MeshModel>& meshVec, Color ambientLight, Light& light1, Light& light2, Z_Buffer& zBuffer);

	Vector4 getNewBarycentricCrd(Triangle& T,Vector4& bCrd, eScanConvMovement M, unsigned int numOfMoves);
	float helpGNBC(Triangle& T, int a, int b, float x, float y);

	Color getFlatColor(Triangle& T,Material& M, Color& ambientLight, Light& light1, Light& light2);
	Color clacDiffuseLight(Vector4& point, Vector4 & normal, Light& light, double Kd);
	Color clacSpecularLight(Vector4& point, Vector4& normal, Light& light, double Ks, double specularExp);
};


#endif

