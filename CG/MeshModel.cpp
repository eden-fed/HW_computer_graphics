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
