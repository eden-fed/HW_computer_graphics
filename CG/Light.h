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
	void setIntensity(Color I);
	void setPosition(Vector4 P);
	void setDirection(Vector4 D);
	void setType(eLightType T);

	//get functions
	Color getIntensity() { return this->intensity;};
	Vector4 getPosition() { return this->position;};
	Vector4 getDirection() { return this->direction;};
	eLightType getType() { return this->type;};

	//class methods

};
#endif
