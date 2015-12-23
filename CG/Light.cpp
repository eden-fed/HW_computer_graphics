#include "Light.h"

Light::Light()
{
	intensity.setColor(0, 0, 0);
	position.setVlaues(0,0,0,1);
	direction.setVlaues(0, 0, 0, 1);;
	type=_POINT;
}

Light::~Light()
{
}

void Light::setIntensity(Color I)
{
	this->intensity = I;
}

void Light::setPosition(Vector4 P)
{
	this->position = P;
}

void Light::setDirection(Vector4 D)
{
	this->direction = D;
}

void Light::setType(eLightType T)
{
	this->type = T;
}
