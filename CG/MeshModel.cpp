#include "MeshModel.h"
#include "Matrix4x4.h"

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
	////////////////////////////////////
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

void MeshModel::calcCentroid()
{
	Vector4 sum;
	for (int i = 0; i < vertices.size(); i++) {
		sum += vertices[i].vertex;
	}
	this->centroid = sum * (1 / vertices.size());
}

void MeshModel::transformMshMdl(Matrix4x4 &M)
{
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].vertex = vertices[i].vertex*M;
		vertices[i].normal = vertices[i].normal*M;
	}
	for (int j = 0; j < model.size(); j++) {
		for (int k = 0; k < 3; k++) {
			model[j][k] = model[j][k] * M;
		}
	}
}

void MeshModel::moveCentroidToOrigin()
{
	Matrix4x4 M(1,0,0,0,0,1,0,0,0,0,1,0,(-1 * this->centroid[0]), (-1 * this->centroid[1]), (-1 * this->centroid[2]),1);
	transformMshMdl(M);


}
