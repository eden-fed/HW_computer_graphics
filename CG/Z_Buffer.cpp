#include "Z_Buffer.h"

Z_Buffer::Z_Buffer()
{
	for (int i = 0; i < buffer.size(); i++) {
		for (int j = 0; j = buffer[i].size(); j++) {
			buffer[i][j].zValue = -DBL_MAX;
		}
	}
}

Z_Buffer::~Z_Buffer()
{
}

double Z_Buffer::Depth(Triangle Q, double X, double Y)
{
	//Ax + By +Cz + D = 0 Normal Vector: N=(A, B, C)
	Vector4 N = Q.getNormal();

	double A = N[0];
	double B = N[1];
	double C = N[2];
	double D = -(A*Q[1][0] + B*Q[1][0] + C*Q[1][0]);


	//Z = (-AX -BY -D)/C
	return -((A*X) + (B*Y) + D) / C;
}
