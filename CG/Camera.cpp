#include "Camera.h"
#include <math.h> 

# define PI           3.14159265358979323846  /* pi */
# define ABS(x) ((x)<0 ? (-(x)) : (x))


Camera::Camera(Vector4 vPosition, Vector4 vDirection, Vector4 vUp)
{
	this->flAspectRatio = 1;
	this->vPosition = vPosition;
	this->vDirection = vDirection;
	this->vUp = vUp;
	setViewMtrx(this->vPosition, this->vDirection, this->vUp);
}

Camera::Camera()
{
	this->flAspectRatio = 1;
	setViewMtrx({ 0 , 0 , 100,0 }, { 0, 0, 0,0 }, { 0,1,0,0 });
	//setViewMtrx({ 0,200,0,0 }, { 0,0,0,0 }, { -1,0,0,0 });
	setProjectionMatrix(60, 0.01, 10000, ORTHOGRAPHIC, 1);
}

Camera::~Camera()
{
}

void Camera::setCamPosition(Vector4 vPosition)
{
	this->vPosition = vPosition;
	setViewMtrx(this->vPosition, this->vDirection, this->vUp);
}

void Camera::setCamDirection(Vector4 vDirection)
{
	this->vPosition = vPosition;
	setViewMtrx(this->vPosition, this->vDirection, this->vUp);

}

void Camera::setCamUp(Vector4 vUp)
{
	this->vUp = vUp;
	setViewMtrx(this->vPosition, this->vDirection, this->vUp);
}

void Camera::setNear(double flNear)
{
	this->flNear = flNear;
	setProjectionMatrix(this->flFovy, this->flNear, this->flFar, this->pType, this->flAspectRatio);
}

void Camera::setFar(double flFar)
{
	this->flFar = flFar;
	setProjectionMatrix(this->flFovy, this->flNear, this->flFar, this->pType, this->flAspectRatio);
}

void Camera::setFovy(double flFovy)
{
	this->flFovy = flFovy;
	setProjectionMatrix(this->flFovy, this->flNear, this->flFar, this->pType, this->flAspectRatio);
}

void Camera::setProjectionType(eProjectionType pType)
{
	this->pType = pType;
	setProjectionMatrix(this->flFovy, this->flNear, this->flFar, this->pType, this->flAspectRatio);
}

void Camera::setViewMtrx(Vector4 vEye, Vector4 vAt, Vector4 vUp)
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

	viewMtrx[3][0] = -(vEye[0]); //-(xaxis*vEye);//-(vEye[0]);
	viewMtrx[3][1] = -(vEye[1]); //-(yaxis*vEye);//-(vEye[1]);
	viewMtrx[3][2] = -(vEye[2]); //-(zaxis*vEye);//-(vEye[2]);

}


void Camera::setProjectionMatrix(double flFovy = 60, double flNear = 0.01, double flFar = 10000, eProjectionType pType = PERSPECTIVE, double flAspectRatio = 1)
{
	Matrix4x4 matTest;
	this->flFovy = flFovy;
	this->flFar = flFar;
	this->flNear = flNear;
	this->flAspectRatio = flAspectRatio;
	this->pType = pType;


	double t = flNear * tan((flFovy / 2)*(PI / 180));
	double b = -t;
	double l = b * flAspectRatio;
	double r = t * flAspectRatio;

	switch (pType) {
	case PERSPECTIVE:
		projectionMtrx.setAllValues(((2 * flNear) / (r - l)), 0, 0, 0, 0, ((2 * flNear) / (t - b)), 0, 0, ((r + l) / (r - l)), ((t + b) / (t - b)), (flFar + flNear) / (flNear - flFar), -1, 0, 0, (2 * flFar*flNear) / (flNear - flFar), 0);
		matTest.setAllValues(100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 1);
		projectionMtrx = projectionMtrx * matTest;
		break;
	case ORTHOGRAPHIC:
		projectionMtrx.setAllValues((2 / (r - l)), 0, 0, 0, 0, (2 / (t - b)), 0, 0, 0, 0, (2 / (flNear - flFar)), 0, ((r + l) / (l - r)), ((t + b) / (b - t)), ((flFar + flNear) / (flNear - flFar)), 1);
		matTest.setAllValues(0.3, 0, 0, 0, 0, 0.3, 0, 0, 0, 0, 0.3, 0, 0, 0, 0, 1);
		projectionMtrx = projectionMtrx * matTest;
		break;
	}

}


/*void Camera::setPerspectiveMatrix(double a, double d)
{
perspectiveMtrx.setAllValues(1,0,0,0,0,1,0,0,0,0,(d/(d-a)),(1/d),0,0,(-a*d/(d-a)),0);
}*/
