#include "Camera.h"

Camera::Camera()
{
	setViewMtrx({ 0,0,200,0 }, { 0,0,0,0 }, { 0,1,0,0 });
	pType = PERSPECTIVE;
}

Camera::~Camera()
{
}

void Camera::setViewMtrx(Vector4 vPosition, Vector4 vDirection, Vector4 vUp)
{
	/*
	if C is the camera matrix which moves the camera to world space
	and V is the view matrix which move it to the origien
	then I=C*V
	*/

	// The "look-at" vector. 
	Vector4 zaxis = (vPosition - vDirection).normalize();
	// The "right" vector.
	Vector4 xaxis = (vUp ^ zaxis).normalize();
	// The "up" vector.
	Vector4 yaxis = (zaxis ^ xaxis).normalize();
	
	viewMtrx.setAllValues(xaxis[0], yaxis[0], zaxis[0],0, xaxis[1], yaxis[1], zaxis[1],0, xaxis[2], yaxis[2], zaxis[2], 0, -vPosition[0], -vPosition[1], -vPosition[2],1);

}

void Camera::setProjectionType(eProjectionType pType)
{
	this->pType = pType;
}

void Camera::setPerspectiveMatrix(float f, float n, float t, float b, float l, float r, eProjectionType ptype)
{
}



/*void Camera::setPerspectiveMatrix(float a, float d)
{
	perspectiveMtrx.setAllValues(1,0,0,0,0,1,0,0,0,0,(d/(d-a)),(1/d),0,0,(-a*d/(d-a)),0);
}*/
