#include "Vector4.h"

int & Vector4::operator[](int i)
{
	return vectorValues[i];
}
