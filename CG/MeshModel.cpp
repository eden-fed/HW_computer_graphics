#include "MeshModel.h"

MeshModel::MeshModel()
{
}
MeshModel::MeshModel(Wavefront_obj & J)
{

	bool has_normals;
	if (J.m_normals.size() > 0) {
		has_normals = true;
	}
	for (int i = 0; i < J.m_points.size(); i++) {
		vertexInfo v;
		v.vertex = J.m_points[i];
		if (has_normals)
			v.normal = J.m_normals[i];
		vertices.push_back(v);
	}
	if (!has_normals)
		this->calcNormals();

	for (int i = 0; i < J.m_faces.size(); i++) {
		Triangle t;
		t = (Triangle)J.m_faces[i];
		model.push_back(t);
	}

	this->calcCentroid();
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
	Vector4 sumVector;
	int sumArea;
	for (int i = 0; i < vertices.size(); i++) {
		sumVector.setVlaues(0, 0, 0, 1);
		sumArea = 0;
		for (int j = 0; j < model.size(); j++) {
			if (model[j].isVertexInTriangle(vertices[i].vertex)) {
				sumVector += model[j].getNormal()*model[j].getArea();
				sumArea += model[j].getArea();
			}
		}
		vertices[i].normal = sumVector * (1 / sumArea);
	}
}

Vector4 MeshModel::calcCentroid()
{
	Vector4 sum;
	for (int i = 0; i < vertices.size(); i++) {
		sum += vertices[i].vertex;
	}
	return sum *(1 / vertices.size());
}
