#include "Line.h"

Line::Line():startCrd(0,0,255), endCrd(0, 0,255){
}

Line::Line(const Coordinate valStart, const Coordinate valEnd)
{
	this->startCrd=valStart;
	this->endCrd = valEnd;
}

Line::Line(Vector4 & V1, Vector4 & V2, int color)
{
	Vector4 roundV1, roundV2;
	roundV1 = V1.roundVectorValues();
	roundV2 = V2.roundVectorValues();
	this->startCrd.setX(roundV1[0]);
	this->startCrd.setY(roundV1[1]);
	this->startCrd.setColor(color);
	this->endCrd.setX(roundV2[0]);
	this->endCrd.setY(roundV2[1]);
	this->endCrd.setColor(color);

}

Line::Line(const Line & line)
{
	*this = line;
}

Line::~Line()
{
}

void Line::setStartCrd(Vector4 & V1, int color)
{
	Vector4 roundV1;
	roundV1 = V1.roundVectorValues();
	this->startCrd.setX(roundV1[0]);
	this->startCrd.setY(roundV1[1]);
	this->startCrd.setColor(color);
}

void Line::setStartCrd(const Coordinate startCrd)
{
	this->startCrd = startCrd;
}

void Line::setStartCrd(const int valX, const int valY, unsigned int valColor)
{
	this->startCrd.setX(valX);
	this->startCrd.setY(valY);
	this->startCrd.setColor(valColor);
}

void Line::setEndCrd(Vector4 & V2, int color)
{
	Vector4 roundV2;
	roundV2 = V2.roundVectorValues();
	this->endCrd.setX(roundV2[0]);
	this->endCrd.setY(roundV2[1]);
	this->endCrd.setColor(color);
}

void Line::setEndCrd(const Coordinate endCrd)
{
	this->endCrd = endCrd;
}

void Line::setEndCrd(const int valX, const int valY, unsigned int valColor)
{
	this->endCrd.setX(valX);
	this->endCrd.setY(valY);
	this->endCrd.setColor(valColor);
}


const Line & Line::operator=(const Line & line)
{
	this->startCrd.setX(line.startCrd.getX());
	this->startCrd.setY(line.startCrd.getY());
	this->endCrd.setX(line.endCrd.getX());
	this->endCrd.setY(line.endCrd.getY());
	return *this;
}

const bool Line::operator==(const Line & line) const
{
	if ((this->startCrd == line.startCrd && this->endCrd== line.endCrd)|| (this->startCrd == line.endCrd  && this->endCrd == line.startCrd))
		return true;
	else
		return false;
}

const bool Line::operator!=(const Line & line) const
{
		return !(*this==line);
}

const bool Line::operator<(const Line & line) const
{
	//if they are equal then false
	if (*this == line) {
		return false;
	}
	//if the start coordinates are equal -> determine using the end coordinates
	else if (this->startCrd == line.startCrd) {
		//if x is equal -> determine using y
		if (this->endCrd.getX() == line.endCrd.getX()) {
			return this->endCrd.getY() < line.endCrd.getY();
		}
		else {
			return this->endCrd.getX() < line.endCrd.getX();
		}
	}
	//if the start coordinates are differant ->determine using the start coordinates
	else {
		//if x is equal -> determine using y
		if (this->startCrd.getX() == line.startCrd.getX()) {
			return this->startCrd.getY() < line.startCrd.getY();
		}
		else {
			return this->startCrd.getX() < line.startCrd.getX();
		}
	}

}

void Line::drawline() 
{
	Line::eLineType elineType;
	Coordinate C1=startCrd, C2=endCrd;
	int deltaX, deltaY, d, deltaE, deltaNE;
	Pixel pxl,pxlToPush;
	Renderer renderer;
	std::vector<Pixel> pixels;

	elineType=HelpGetLineType(C1, C2);
	HelpHandleActionOnInput(elineType, C1, C2);

	pxl.color = C1.getColor();
	pxl.x = C1.getX();
	pxl.y = C1.getY();

	deltaX = C2.getX() - C1.getX();
	deltaY = C2.getY() - C1.getY();

	d = (deltaY - deltaX)<<1;

	deltaE = deltaY << 1;
	deltaNE = (deltaY - deltaX) << 1;

	pxlToPush = pxl;
	HelpHandleActionOnOutput(elineType, pxlToPush);
	pixels.push_back(pxlToPush);

	while (pxl.x < C2.getX()) {
		if (d < 0) {
			d += deltaE;
			pxl.x++;
		}
		else {
			d += deltaNE;
			pxl.x++;
			pxl.y++;
		}
		pxlToPush = pxl;
		HelpHandleActionOnOutput(elineType, pxlToPush);
		pixels.push_back(pxlToPush);
	}
	renderer.drawPixels(pixels);
}

Line::eLineType Line::HelpGetLineType(Coordinate & C1, Coordinate & C2) 
{
	bool lowSlope;

	if (C1.getX() > C2.getX()) {
		HelpSwapCoordinates(C1,C2);
	}
	else if (C1.getX() == C2.getX() && C1.getY() <= C2.getY()) {
		return HIGH_POSITIVE_SLOPE;
	}
	else if (C1.getX() == C2.getX() && C1.getY() > C2.getY()) {
		return HIGH_NEGATIVE_SLOPE;
	}

	lowSlope = (ABS(C2.getY() - C1.getY()) <= ABS(C2.getX() - C1.getX()));

	if (C1.getY() <= C2.getY() && lowSlope) {
		return LOW_POSITIVE_SLOPE;
	}
	else if (C1.getY() < C2.getY() && !lowSlope) {
		return HIGH_POSITIVE_SLOPE;
	}
	else if (C1.getY() > C2.getY() && lowSlope) {
		return LOW_NEGATIVE_SLOPE;
	}
	else if (C1.getY() > C2.getY() && !lowSlope) {
		return HIGH_NEGATIVE_SLOPE;
	}
}

void Line::HelpSwapCoordinates(Coordinate & C1, Coordinate & C2)
{
	Coordinate tmpCoordinate;
	tmpCoordinate = C1;
	C1 = C2;
	C2 = tmpCoordinate;
}



void Line::HelpHandleActionOnInput(Line::eLineType& lineType, Coordinate & C1, Coordinate & C2)
{
	switch (lineType) {
     case LOW_POSITIVE_SLOPE:
		 break;
	 case HIGH_POSITIVE_SLOPE:
		 C1.swapXY();
		 C2.swapXY();
		 break;
	 case LOW_NEGATIVE_SLOPE:
		 C1.setY(-1 * C1.getY());
		 C2.setY(-1 * C2.getY());
		 break;
	 case HIGH_NEGATIVE_SLOPE:
		 C1.setY(-1 * C1.getY());
		 C2.setY(-1 * C2.getY());
		 C1.swapXY();
		 C2.swapXY();
		 break;
	}
}

void Line::HelpHandleActionOnOutput(Line::eLineType& lineType, Pixel& pxl)
{
	int tmp;
	switch (lineType) {
	case LOW_POSITIVE_SLOPE:
		break;
	case HIGH_POSITIVE_SLOPE:
		tmp = pxl.x;
		pxl.x= pxl.y;
		pxl.y = tmp;
		break;
	case LOW_NEGATIVE_SLOPE:
		pxl.y *= (-1);
		break;
	case HIGH_NEGATIVE_SLOPE:
		tmp = pxl.x;
		pxl.x = pxl.y;
		pxl.y = -tmp;
		break;
	}
}







