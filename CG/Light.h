//#pragma once

#ifndef LIGHT_H
#define LIGHT_H
#include "Vector4.h"
#include "Color.h"

typedef enum {
	POINT,
	DIRECTION,
} eLightType;


class Light
{
	Color intensity;
	Vector4 position, direction;
	eLightType type; 

public:
	//constructors
	Light();

	//destructor
	~Light();

	//set functions

	//get functions

	//class methods

};
#endif
