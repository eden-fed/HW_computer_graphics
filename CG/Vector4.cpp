#include "Vector4.h"

Vector4::Vector4() :vectorValues{0,0,0,1}, columnVector(true)
{
}

Vector4::Vector4(const int valX, const int valY, const int valZ, const int valW, bool isColumnVector)
{
	this->vectorValues[0] = valX;
	this->vectorValues[1] = valY;
	this->vectorValues[2] = valZ;
	this->vectorValues[3] = valW;
	this->columnVector = isColumnVector;

}

Vector4::Vector4(const Vector4 & V)
{

	for (int i = 0; i <= 3; i++) {
		this->vectorValues[i] = V.vectorValues[i];
	}
	this->columnVector = V.columnVector;

}

Vector4::~Vector4()
{
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
	this->columnVector = V.columnVector;
	return *this;
}

const bool Vector4::operator==(const Vector4 & V) const
{
	bool isEqual = true; 
	for (int i = 0; i <= 3; i++) {
		isEqual&=this->vectorValues[i] == V.vectorValues[i];
	}
	isEqual &= this->columnVector == V.columnVector;

	return isEqual;
}

const bool Vector4::operator!=(const Vector4 & V) const
{
	return !(*this==V);
}

const Vector4 & Vector4::operator+(const Vector4 & V) 
{
	if (this->columnVector == V.columnVector) {
		for (int i = 0; i <= 3; i++) {
			this->vectorValues[i] += V.vectorValues[i];
		}
	}
	else {
		throw new std::string("Trying to add vectors of differant types\n");
	}
	return *this;

}

const Vector4 & Vector4::operator-(const Vector4 & V)
{
	if (this->columnVector == V.columnVector) {
		for (int i = 0; i <= 3; i++) {
			this->vectorValues[i] -= V.vectorValues[i];
		}
	}
	else {
		throw new std::string("Trying to subtract vectors of differant types\n");
	}
	return *this;
}

const Vector4 & Vector4::operator*(const Matrix4x4 & M)
{
	if (this->columnVector == false) {
		Vector4 V = *this;
		memset(this, 0, sizeof(Vector4));
		for (int i = 0; i <= 3; i++) {
			for (int j = 0; j <= 3; j++) {
				(*this)[i] += V[j] * M[j][i];
			}
		}
		this->columnVector = false;
	}
	else {
		throw new std::string("Trying to multiply a column vector with a matrix\n");
	}
	return *this;
}

