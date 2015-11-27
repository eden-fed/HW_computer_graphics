#include "Camera.h"

Camera::Camera()
{
	setPerspectiveMatrix(100,10);
	/*setPerspective(100, 10);
	//Setting orthographic matrix
	Vector4 a(1, 0, 0), b(0, 1, 0), c(0, 0, 0);
	o_mat.setRow0(a); o_mat.setRow1(b); o_mat.setRow2(c);
	o_mat.setType(5);
	//Oter initialized to ID
	Vector4 v5(1, 0, 0), v6(0, 1, 0), v7(0, 0, 1);
	model1_mat.setRow0(v5); model1_mat.setRow1(v6); model1_mat.setRow2(v7);
	model2_mat.setRow0(v5); model2_mat.setRow1(v6); model2_mat.setRow2(v7);
	cam1_mat.setRow0(v5); cam1_mat.setRow1(v6); cam1_mat.setRow2(v7);
	cam2_mat.setRow0(v5); cam2_mat.setRow1(v6); cam2_mat.setRow2(v7);*/
}

void Camera::setProjectionType(eProjectionType pType)
{
	this->pType = pType;
}

void Camera::setPerspectiveMatrix(float a, float d)
{
	perspectiveMtrx.setAllValues(1,0,0,0,0,1,0,0,0,0,(d/(d-a)),(1/d),0,0,(-a*d/(d-a)),0);
}
