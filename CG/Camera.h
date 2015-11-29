#ifndef CAMERA_H
#define CAMERA_H
#include "MeshModel.h"
#include "Matrix4x4.h"
#include "Vector4.h"

typedef enum {
	PERSPECTIVE,
	ORTHOGRAPHIC,
} eProjectionType;

class Camera
{
	Matrix4x4 viewMtrx;
	Matrix4x4 perspectiveMtrx;
	float flFovy, flNear, flFar, flAspectRatio;
	eProjectionType pType;

public:
	//constructors
	Camera();

	//destructor
	~Camera();

	//get functions
	Matrix4x4 getViewMtrx() { return viewMtrx; };
	Matrix4x4 getPerspectiveMtrx() { return perspectiveMtrx; };

	//set functions
	void setViewMtrx(Vector4 vPosition, Vector4 vDirection, Vector4 vUp, float screenResolution[2]);
	void setProjectionType(eProjectionType pType);
	void setProjectionMatrix(float flFovy, float flNear, float flFar, float fAspectRatio, eProjectionType pType);

	//operators



};

#endif