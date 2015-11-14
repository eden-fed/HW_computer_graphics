#include <iostream>
#include "Coordinate.h"
#include "Renderer.h"


#ifndef LINE_H
#define LINE_H

#define ABS(x) ((x)<0 ? (-(x)) : (x))

class Line
{
private:
	Coordinate startCrd;
	Coordinate endCrd;

	typedef enum { 
		LOW_POSITIVE_SLOPE,
		HIGH_POSITIVE_SLOPE,
		LOW_NEGATIVE_SLOPE,
		HIGH_NEGATIVE_SLOPE
	} eLineType;

public:

	//constructors
	Line();
	Line(const Coordinate valStartCrd, const Coordinate valEndCrd);

	//copy constructor
	Line(const Line& line);

	//destructor
	~Line();

	//get functions
	inline const Coordinate getStartCrd()const { return startCrd; }
	inline const Coordinate getEndCrd()const { return endCrd; }

	//set functions
	void setStartCrd(const Coordinate startCrd);
	void setStartCrd(const int valX, const int valY, unsigned int valColor);
	void setEndCrd(const Coordinate endCrd);
	void setEndCrd(const int valX, const int valY, unsigned int valColor);
	
	//operators
	const Line& operator = (const Line& line);
	const bool operator == (const Line& line)const;//the (==) sighn compares X and Y values of startCrd and endCrd
	const bool operator != (const Line& line)const;//the (!=) sighn compares X and Y values of startCrd and endCrd

	//line methods
	void drawline();
	

private:
	//help functions
	Line::eLineType HelpGetLineType(Coordinate & C1, Coordinate & C2);
	void HelpSwapCoordinates(Coordinate & C1, Coordinate & C2);
	void HelpHandleActionOnInput(Line::eLineType& lineType, Coordinate& C1, Coordinate& C2);
	void HelpHandleActionOnOutput(Line::eLineType& lineType, Pixel& pxl);
};

#endif