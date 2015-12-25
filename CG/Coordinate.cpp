#include "Coordinate.h"

Coordinate::Coordinate():X(0),Y(0),color(0xff0000ff){
}

Coordinate::Coordinate(const int valX, const int valY, unsigned int valColor)
{
	this->setX(valX);
	this->setY(valY);
	this->setColor(valColor);
}

Coordinate::Coordinate(const Coordinate & coordinate)
{
	*this = coordinate;
}

Coordinate::~Coordinate()
{
}

void Coordinate::setX(const int valX)
{
	this->X = valX;
}

void Coordinate::setY(const int valY)
{
	this->Y = valY;
}

void Coordinate::setColor(const int valColor)
{
	this->color = valColor;
}

const Coordinate & Coordinate::operator=(const Coordinate & coordinate)
{
	memcpy(this, &coordinate, sizeof(Coordinate));
	return *this;
}

const bool Coordinate::operator==(const Coordinate & coordinate) const
{
	if (memcmp(this, &coordinate, sizeof(Coordinate)) == 0)
		return true;
	else
		return false;
}

const bool Coordinate::operator!=(const Coordinate & coordinate) const
{
	if (memcmp(this, &coordinate, sizeof(Coordinate)) == 0)
		return false;
	else
		return true;
}

Coordinate::operator Pixel()
{
	Pixel pxl;
	pxl.x = this->getX();
	pxl.y = this->getY();
	pxl.color = this->getColor();
	return pxl;
}

void Coordinate::swapXY()
{
	int tmp;
	tmp = this->getX();
	this->setX(this->getY());
	this->setY(tmp);
}
