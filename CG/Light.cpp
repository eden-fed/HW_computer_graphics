#include "Light.h"

Light::Light()
{
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
