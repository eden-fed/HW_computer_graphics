#include "Camera.h"
#include <math.h> 

# define PI           3.14159265358979323846  /* pi */

Camera::Camera()
{
	float screenResolution[] = { 1366,768 };
	setViewMtrx({ 0 , 0 , 100,0 }, { 0, 0, 0,0 }, { 0,1,0,0 }, screenResolution);
	//setViewMtrx({ 0,200,0,0 }, { 0,0,0,0 }, { -1,0,0,0 });
	setProjectionMatrix(60, 0.01, 10000, 1, ORTHOGRAPHIC);
}

Camera::~Camera()
{
}

void Camera::setViewMtrx(Vector4 vEye, Vector4 vAt, Vector4 vUp, float screenResolution[2])
{
	Vector4 zaxis = (vEye - vAt).normalize();
	Vector4 xaxis = (vUp ^ zaxis).normalize();
	Vector4 yaxis = (zaxis ^ xaxis)*(-1);


	viewMtrx[0][0] = xaxis[0];// *(1 / (vEye - vAt).getSize());
	viewMtrx[1][0] = xaxis[1];
	viewMtrx[2][0] = xaxis[2];
	viewMtrx[3][0] = 0;

	viewMtrx[0][1] = yaxis[0];
	viewMtrx[1][1] = yaxis[1];// *(1 / (vEye - vAt).getSize());
	viewMtrx[2][1] = yaxis[2];
	viewMtrx[3][1] = 0;

	viewMtrx[0][2] = zaxis[0];
	viewMtrx[1][2] = zaxis[1];
	viewMtrx[2][2] = zaxis[2];// *(1 / (vEye - vAt).getSize());
	viewMtrx[3][2] = 0;

	viewMtrx[0][3] = 0;
	viewMtrx[1][3] = 0;
	viewMtrx[2][3] = 0;
	viewMtrx[3][3] = 1;

	viewMtrx[3][0] = -(vEye[0]);//+ (screenResolution[0] / 2); //-(xaxis*vEye);//-(vEye[0]);
	viewMtrx[3][1] = -(vEye[1]);//+ (screenResolution[1] / 2); //-(yaxis*vEye);//-(vEye[1]);
	viewMtrx[3][2] = -(vEye[2]); //-(zaxis*vEye);//-(vEye[2]);

}

void Camera::setProjectionType(eProjectionType pType)
{
	setProjectionMatrix(this->flFovy, this->flNear, this->flFar, this->flAspectRatio, pType);
}

void Camera::setProjectionMatrix(float flFovy = 60, float flNear = 0.01, float flFar = 10000, float flAspectRatio = 1, eProjectionType pType = PERSPECTIVE)
{
	this->flFovy = flFovy;
	this->flFar = flFar;
	this->flNear = flNear;
	this->flAspectRatio = flAspectRatio;
	this->pType = pType;


	float t = flNear * tan((flFovy / 2)*(PI / 180));
	float b = -t;
	float l = b * flAspectRatio;
	float r = t * flAspectRatio;

	switch (pType) {
	case PERSPECTIVE:
		perspectiveMtrx.setAllValues(((2 * flNear) / (r - l)), 0, 0, 0, 0, ((2 * flNear) / (t - b)), 0, 0, ((r + l) / (r - l)), ((t + b) / (t - b)), (flFar + flNear) / (flNear - flFar), -1, 0, 0, (2 * flFar*flNear) / (flNear - flFar), 0);
		break;
	case ORTHOGRAPHIC:
		perspectiveMtrx.setAllValues((2 / (r - l)), 0, 0, 0, 0, (2 / (t - b)), 0, 0, 0, 0, (2 / (flNear - flFar)), 0, ((r + l) / (l - r)), ((t + b) / (b - t)), ((flFar + flNear) / (flNear - flFar)), 1);
		break;
	}

}


/*void Camera::setPerspectiveMatrix(float a, float d)
{
perspectiveMtrx.setAllValues(1,0,0,0,0,1,0,0,0,0,(d/(d-a)),(1/d),0,0,(-a*d/(d-a)),0);
}*/
