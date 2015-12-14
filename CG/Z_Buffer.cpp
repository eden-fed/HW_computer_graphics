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
	return 0.0;
}
