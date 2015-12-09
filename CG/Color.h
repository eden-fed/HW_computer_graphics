#pragma once

class Color
{
	unsigned int color;
public:
	//constructors
	Color();
	Color(int r,int g,int b);

	//set functions
	void setColor(unsigned int C);

	//get functions
	unsigned int getColor() {return this->color};

	//class methods
	int getRedPortion();
	int getGreenPortion();
	int getBluePortion();
	void setColor(int r, int g, int b);
};