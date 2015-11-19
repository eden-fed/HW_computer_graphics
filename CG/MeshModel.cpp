#include "MeshModel.h"

MeshModel::MeshModel()
{
}

MeshModel::MeshModel(Wavefront_obj & J)
{
	vertexInfo v;
	bool has_normals;
	if (J.m_normals.size() > 0) {
		has_normals = true;
	}
	for (int i = 0; i < J.m_points.size(); i++) {
		v.vertex = J.m_points[i];
		if (has_normals)
			v.normal = J.m_normals[i];
		else
			calcNormals();
		vertices.push_back(v);
	}
}

MeshModel::~MeshModel()
{
}

Vector4 MeshModel::getCentroid()
{
	return this->centroid;
}

void MeshModel::calcNormals()
{
}

Vector4 MeshModel::calcCentroid()
{
	Vector4 sum;
	for (int i = 0; i < vertices.size();i++) {
		sum += vertices[i].vertex;
	}
	return sum *(1 / vertices.size());
}
