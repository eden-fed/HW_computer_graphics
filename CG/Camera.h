#ifndef CAMERA_H
#define CAMERA_H
#include "MeshModel.h"
#include "Matrix4x4.h"

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
	Camera(Matrix4x4 viewMtrx, eProjectionType pType);

	//destructor
	~Camera();

	//get functions


	//set functions
	void setProjectionType(eProjectionType pType);
	void setPerspectiveMatrix(float a,float d);

	//operators



};

#endif