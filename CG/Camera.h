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
	eProjectionType pType;
	Matrix4x4 viewMtrx;
	Matrix4x4 perspectiveMtrx;

public:
	//constructors
	Camera();

	//destructor
	~Camera();

	//get functions
	Matrix4x4 getViewMtrx() { return viewMtrx; };

	//set functions
	void setViewMtrx(Vector4 vPosition, Vector4 vDirection, Vector4 vUp);
	void setProjectionType(eProjectionType pType);
	void setPerspectiveMatrix(float f,float n, float t, float b, float l, float r, eProjectionType ptype);

	//operators



};

#endif