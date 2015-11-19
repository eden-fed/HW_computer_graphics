#include "MeshModel.h"

MeshModel::MeshModel()
{
}

MeshModel::MeshModel(Wavefront_obj &)
{
}

MeshModel::~MeshModel()
{
}

Vector4 MeshModel::getCentroid()
{
	return this->centroid;
}

Vector4 MeshModel::calcCentroid()
{
	Vector4 sum;
	for (int i = 0; i < vertices.size();i++) {
		sum += vertices[i].vertex;
	}
	return sum *(1 / vertices.size());
}
