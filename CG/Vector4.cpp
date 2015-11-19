#include "Vector4.h"

Vector4::Vector4() :vectorValues{0,0,0,1}
{
}

Vector4::Vector4(const double valX, const double valY, const double valZ, const double valW)
{
	this->vectorValues[0] = valX;
	this->vectorValues[1] = valY;
	this->vectorValues[2] = valZ;
	this->vectorValues[3] = valW;
}

Vector4::Vector4(const Vector4 & V)
{

	for (int i = 0; i <= 3; i++) {
		this->vectorValues[i] = V.vectorValues[i];
	}
}

Vector4::~Vector4()
{
}


double & Vector4::operator[](int i)
{
	return this->vectorValues[i];
}

const double & Vector4::operator[](int i) const
{
	return this->vectorValues[i];
}

const Vector4 & Vector4::operator=(const Vector4 & V)
{
	for (int i = 0; i <= 3; i++) {
		this->vectorValues[i] = V.vectorValues[i];
	}
	return *this;
}

const bool Vector4::operator==(const Vector4 & V) const
{
	bool isEqual = true; 
	for (int i = 0; i <= 3; i++) {
		isEqual&=this->vectorValues[i] == V.vectorValues[i];
	}

	return isEqual;
}

const bool Vector4::operator!=(const Vector4 & V) const
{
	return !(*this==V);
}

const Vector4 & Vector4::operator+=(const Vector4 & V) 
{
		for (int i = 0; i <= 2; i++) {
			this->vectorValues[i] += V.vectorValues[i];
		}
	return *this;

}

const Vector4 Vector4::operator+(const Vector4 & V) 
{
	Vector4 retV = *this;
		for (int i = 0; i <= 2; i++) {
			retV.vectorValues[i] += V.vectorValues[i];
		}
	
	return retV;
}

const Vector4 & Vector4::operator-=(const Vector4 & V)
{	
		for (int i = 0; i <= 2; i++) {
			this->vectorValues[i] -= V.vectorValues[i];
		}
	return *this;
}

const Vector4 Vector4::operator-(const Vector4 & V) 
{
	Vector4 retV = *this;
		for (int i = 0; i <= 2; i++) {
			retV.vectorValues[i] -= V.vectorValues[i];
		}
	return retV;
}

double Vector4::operator*(const Vector4 & V)
{
	double retVal;
		for (int i = 0; i <= 3; i++) {
			retVal += (*this)[i] * V[i];
			//retVal += (*this)[0] * V[0];
		}
	

	return retVal;
}

Vector4 Vector4::operator^(const Vector4 & V)
{
	Vector4 retV;
	retV[0] = (*this)[1] * V[2] - (*this)[2] * V[1];
	retV[1] = (*this)[0] * V[2] - (*this)[2] * V[0];
	retV[2] = (*this)[0] * V[1] - (*this)[1] * V[0];
	return retV;
}

Vector4 Vector4::operator*(const double num)
{
	Vector4 retV;
	for (int i = 0; i <= 2; i++) {
		retV.vectorValues[i] = num*this->vectorValues[i];
	}
	return retV;
}

const Vector4 & Vector4::operator*=(const Matrix4x4 & M)
{
		Vector4 V = *this;
		memset(this, 0, sizeof(Vector4));
		for (int i = 0; i <= 3; i++) {
			for (int j = 0; j <= 3; j++) {
				(*this)[i] += V[j] * M[j][i];
			}
		}
	return *this;
}

const Vector4 Vector4::operator*(const Matrix4x4 & M)
{
	Vector4 V(0,0,0,0);
	
		for (int i = 0; i <= 3; i++) {
			for (int j = 0; j <= 3; j++) {
				V[i] += (*this)[j] * M[j][i];
			}
		}
	return V;
}

