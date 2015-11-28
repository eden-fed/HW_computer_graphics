#include "Camera.h"
#include <math.h> 

Camera::Camera()
{
	setViewMtrx({ 0,0,200,0 }, { 0,0,0,0 }, { 1,1,-1,0 });
	setPerspectiveMatrix(60, 0.01, 10000, PERSPECTIVE);
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

	//viewMtrx.setAllValues(xaxis[0], yaxis[0], zaxis[0],0, xaxis[1], yaxis[1], zaxis[1],0, xaxis[2], yaxis[2], zaxis[2], 0, -vPosition[0], -vPosition[1], -vPosition[2],1);

	viewMtrx.setAllValues(xaxis[0], xaxis[1], xaxis[2],0, yaxis[0], -yaxis[1], yaxis[2],0, zaxis[0], zaxis[1], zaxis[2], 0, -vPosition[0], -vPosition[1], -vPosition[2],1);


}

void Camera::setProjectionType(eProjectionType pType)
{
	setPerspectiveMatrix(this->flFovy, this->flNear, this->flFar, pType);
}

void Camera::setPerspectiveMatrix(float flFovy=60, float flNear=0.01, float flFar=10000, eProjectionType pType= PERSPECTIVE)
{
	this->flFovy = flFovy;
	this->flFar = flFar;
	this->flNear = flNear;
	this->pType = pType;


	float r_l = 2 * flNear * tan(flFovy / 2);
	float t_b = (r_l * 9) / 16;

	switch(pType) {
	case PERSPECTIVE:
		perspectiveMtrx.setAllValues((2* flNear) / r_l,0,0,0,0, (2 * flNear) / t_b,0,0,0,0,-(flFar + flNear)/(flFar - flNear),-1,0,0,(-2* flFar*flNear) / (flFar - flNear),0);
		break;
	case ORTHOGRAPHIC:
		perspectiveMtrx.setAllValues((2/ r_l),0,0,0,0,(2/ t_b),0,0,0,0,(-2/ (flFar - flNear)),0,0,0, (-(flFar + flNear) / (flFar - flNear)),1);
		break;
	}

}


/*void Camera::setPerspectiveMatrix(float a, float d)
{
	perspectiveMtrx.setAllValues(1,0,0,0,0,1,0,0,0,0,(d/(d-a)),(1/d),0,0,(-a*d/(d-a)),0);
}*/
