#pragma once

class Color
{
	unsigned int color;
public:
	//constructors
	Color();
	Color(unsigned int C);
	Color(int r,int g,int b);

	//set functions
	void setColor(unsigned int C);

	//get functions
	unsigned int getColor()const { return this->color; };

	//class methods
	int getRedPortion();
	int getGreenPortion();
	int getBluePortion();
	void setColor(int r, int g, int b);

	//operators
	const Color& operator = (const Color& C);
};