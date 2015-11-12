#include "Vector4.h"

Vector4::Vector4() :vectorValues{0,0,0,1}, isColumnVector(true)
{
}

Vector4::Vector4(const int valX, const int valY, const int valZ, const int valW, bool isCVector)
{
	this->vectorValues[0] = valX;
	this->vectorValues[1] = valY;
	this->vectorValues[2] = valZ;
	this->vectorValues[3] = valW;
	this->isColumnVector = isCVector;

}

Vector4::Vector4(const Vector4 & V)
{

	for (int i = 0; i <= 3; i++) {
		this->vectorValues[i] = V.vectorValues[i];
	}
	this->isColumnVector = V.isColumnVector;

}

Vector4::~Vector4()
{
}

void Vector4::setColumnVector(const bool val)
{
	this->isColumnVector = val;
}

int & Vector4::operator[](int i)
{
	return this->vectorValues[i];
}

const Vector4 & Vector4::operator=(const Vector4 & V)
{
	for (int i = 0; i <= 3; i++) {
		this->vectorValues[i] = V.vectorValues[i];
	}
	this->isColumnVector = V.isColumnVector;
	return *this;
}

const bool Vector4::operator==(const Vector4 & V) const
{
	bool isEqual = true; 
	for (int i = 0; i <= 3; i++) {
		isEqual&=this->vectorValues[i] == V.vectorValues[i];
	}
	isEqual &= this->isColumnVector == V.isColumnVector;

	return isEqual;
}

const bool Vector4::operator!=(const Vector4 & V) const
{
	return !(*this==V);
}

const Vector4 & Vector4::operator+=(const Vector4 & V) 
{
	if (this->isColumnVector == V.isColumnVector) {
		for (int i = 0; i <= 3; i++) {
			this->vectorValues[i] += V.vectorValues[i];
		}
	}
	else {
		throw new std::string("Trying to add vectors of differant types\n");
	}
	return *this;

}

const Vector4 Vector4::operator+(const Vector4 & V) throw(std::string)
{
	Vector4 retV = *this;
	if (retV.isColumnVector == V.isColumnVector) {
		for (int i = 0; i <= 3; i++) {
			retV.vectorValues[i] += V.vectorValues[i];
		}
	}
	else {
		throw new std::string("Trying to add vectors of differant types\n");
	}
	return retV;
}

const Vector4 & Vector4::operator-=(const Vector4 & V)
{
	if (this->isColumnVector == V.isColumnVector) {
		for (int i = 0; i <= 3; i++) {
			this->vectorValues[i] -= V.vectorValues[i];
		}
	}
	else {
		throw new std::string("Trying to subtract vectors of differant types\n");
	}
	return *this;
}

const Vector4 Vector4::operator-(const Vector4 & V) throw(std::string)
{
	Vector4 retV = *this;
	if (retV.isColumnVector == V.isColumnVector) {
		for (int i = 0; i <= 3; i++) {
			retV.vectorValues[i] -= V.vectorValues[i];
		}
	}
	else {
		throw new std::string("Trying to add vectors of differant types\n");
	}
	return retV;
}

const Vector4 & Vector4::operator*=(const Matrix4x4 & M)
{
	if (this->isColumnVector == false) {
		Vector4 V = *this;
		memset(this, 0, sizeof(Vector4));
		for (int i = 0; i <= 3; i++) {
			for (int j = 0; j <= 3; j++) {
				(*this)[i] += V[j] * M[j][i];
			}
		}
		this->isColumnVector = false;
	}
	else {
		throw new std::string("Trying to multiply a column vector with a matrix\n");
	}
	return *this;
}

const Vector4 Vector4::operator*(const Matrix4x4 & M) throw(std::string)
{
	Vector4 V(0,0,0,0,false);
	if (this->isColumnVector == false) {
		for (int i = 0; i <= 3; i++) {
			for (int j = 0; j <= 3; j++) {
				V[i] += (*this)[j] * M[j][i];
			}
		}
	}
	else {
		throw new std::string("Trying to multiply a column vector with a matrix\n");
	}
	return V;
}

