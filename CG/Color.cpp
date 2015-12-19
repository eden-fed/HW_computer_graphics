#include "Color.h"

#define CUT_TO_FIT_UBYTE(x) ((x)<0 ? (x=0) : (((x)>255 ? (x = 255) : (x=x))))


Color::Color()
{
	color=0;
}
Color::Color(unsigned int C)
{
	this->color = C;
}
Color::Color(int r,int g,int b)
{
	setColor(r,g,b);
}

void Color::setColor(unsigned int C)
{
	this->color = C;
}

int Color::getRedPortion()
{
	return color&0x0000ff;
}

int Color::getGreenPortion()
{
	return (color&0x00ff00)>>8;
}

int Color::getBluePortion()
{
	return (color&0xff0000)>>16;
}

void Color::setColor(int r, int g, int b)
{
	CUT_TO_FIT_UBYTE(r);
	CUT_TO_FIT_UBYTE(g);
	CUT_TO_FIT_UBYTE(b);
	color = (b << 16) | (g << 8) | r;
}

const Color& Color::operator=(const Color & C)
{
	this->color = C.getColor();
	return *this;
}
