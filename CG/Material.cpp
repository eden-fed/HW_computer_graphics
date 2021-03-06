#include "Material.h"

Material::Material(double A, double D, double S, double SE)
{
	this->ambient = A;
	this->diffuse = D;
	this->specular = S;
	this->specularExp = SE;

}

Material::Material()
{
}

Material::~Material()
{
}

void Material::setAmbient(double A)
{
	this->ambient = A;
}

void Material::setDiffuse(double D)
{
	this->diffuse = D;
}

void Material::setSpecular(double S)
{
	this->specular = S;
}

void Material::setspecularExp(double SE)
{
	this->specularExp = SE;
}

void Material::setAll(double A, double D, double S, double SE)
{
	this->ambient = A;
	this->diffuse = D;
	this->specular = S;
	this->specularExp = SE;
}
