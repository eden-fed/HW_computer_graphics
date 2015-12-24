//#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

class Material
{
	double ambient;
	double diffuse;
	double specular;
	double specularExp;

public:
	//constructors
	Material(double A,double D, double S, double SE);
	Material();

	//destructor
	~Material();

	//set functions
	void setAmbient(double A);
	void setDiffuse(double D);
	void setSpecular(double S);
	void setspecularExp(double SE);
	void setAll(double A, double D, double S, double SE);

	//get functions
	double getAmbient() { return this->ambient; };
	double getDiffuse() { return this->diffuse; };
	double getSpecular() { return this->specular; };
	double getspecularExp() { return this->specularExp; };
	//class methods

};
#endif
