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
	void draw(MeshModel& mesh,Color ambientLight, Light& light1, Light& light2,Z_Buffer& zBuffer,Vector4& eyePosition, Matrix4x4& pMtrx);

	void flatShading(MeshModel& mesh, Color ambientLight,Light& light1, Light& light2, Z_Buffer& zBuffer, Vector4& eyePosition, Matrix4x4& pMtrx);
	void gouraudShading(MeshModel& mesh, Color ambientLight, Light& light1, Light& light2, Z_Buffer& zBuffer, Vector4& eyePosition, Matrix4x4& pMtrx);
	void phongShading(MeshModel& mesh, Color ambientLight, Light& light1, Light& light2, Z_Buffer& zBuffer, Vector4& eyePosition, Matrix4x4& pMtrx);

	Vector4 getNewBarycentricCrd(Triangle& T,Vector4& bCrd, eScanConvMovement M, unsigned int numOfMoves);
	double helpGNBC(Triangle& T, int a, int b, double x, double y);

	Color getFlatColor(Triangle& T,Material& M, Color& ambientLight, Light& light1, Light& light2, Vector4& eyePosition);
	Color getVertxColor(vertexInfo& vInfo, Material& M, Color& ambientLight, Light& light1, Light& light2, Vector4& eyePosition);
	

	Color clacDiffuseLight(Vector4& point, Vector4 & normal, Light& light, double Kd);
	Color clacSpecularLight(Vector4& point, Vector4& normal, Light& light, double Ks, double specularExp, Vector4& eyePosition);
};


#endif

