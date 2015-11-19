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
			CalcNormals();
		vertices.push_back(v);
	}
}

MeshModel::~MeshModel()
{
}

void MeshModel::CalcNormals()
{
}
