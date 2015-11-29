#include "Camera.h"
#include <math.h> 

Camera::Camera()
{
	//setViewMtrx({ 0,0,200,0 }, { 0,0,0,0 }, {0,1,0,0 });
	setViewMtrx({ 0,200,0,0 }, { 0,0,0,0 }, { -1,0,0,0 });
	setPerspectiveMatrix(60, 0.01, 10000, PERSPECTIVE);
}

Camera::~Camera()
{
}

void Camera::setViewMtrx(Vector4 vEye, Vector4 vAt, Vector4 vUp)
{
	Vector4 zaxis = (vEye - vAt).normalize();
	Vector4 xaxis = (vUp ^ zaxis).normalize();
	Vector4 yaxis = (zaxis ^ xaxis)*(-1);


	viewMtrx[0][0] = xaxis[0];
	viewMtrx[1][0] = xaxis[1];
	viewMtrx[2][0] = xaxis[2];
	viewMtrx[3][0] = 0;

	viewMtrx[0][1] = yaxis[0];
	viewMtrx[1][1] = yaxis[1];
	viewMtrx[2][1] = yaxis[2];
	viewMtrx[3][1] = 0;

	viewMtrx[0][2] = zaxis[0];
	viewMtrx[1][2] = zaxis[1];
	viewMtrx[2][2] = zaxis[2];
	viewMtrx[3][2] = 0;

	viewMtrx[0][3] = 0;
	viewMtrx[1][3] = 0;
	viewMtrx[2][3] = 0;
	viewMtrx[3][3] = 1;

	viewMtrx[3][0] = -(xaxis*vEye);//-(vEye[0]);
	viewMtrx[3][1] = -(yaxis*vEye);//-(vEye[1]);
	viewMtrx[3][2] = -(zaxis*vEye);//-(vEye[2]);
	
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
