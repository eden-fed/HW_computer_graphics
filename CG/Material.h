//#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

class Material
{
	double ambient;
	double diffuse;
	double specular;

public:
	//constructors
	Material(double A,double D, double S);
	Material();

	//destructor
	~Material();

	//set functions
	void setAmbient(double A);
	void setDiffuse(double D);
	void setSpecular(double S);

	//get functions
	double getAmbient() { return this->ambient; };
	double getDiffuse() { return this->diffuse; };
	double getSpecular() { return this->specular; };
	//class methods

};
#endif
