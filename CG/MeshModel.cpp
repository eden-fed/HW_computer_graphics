#include "MeshModel.h"
#include "Matrix4x4.h"

MeshModel::MeshModel()
{
}
MeshModel::MeshModel(Wavefront_obj & J)
{

	bool has_normals=false;
	if (J.m_normals.size() != 0) {
		has_normals = true;
	}
	for (int i = 0; i < J.m_points.size(); i++) {
		vertexInfo v;
		v.vertex = J.m_points[i];
		if (has_normals)
			v.normal = J.m_normals[i];
		vertices.push_back(v);
	}

	this->moveCentroidToOrigin();
	this->scale10units();

	/*for (int i = 0; i < J.m_points.size();i++) {
		J.m_points[i] = vertices[i].vertex;
	}*/

	for (int i = 0; i < J.m_faces.size(); i++) {
		Triangle t(vertices[J.m_faces[i].v[0]].vertex, vertices[J.m_faces[i].v[1]].vertex, vertices[J.m_faces[i].v[2]].vertex);
		faces.push_back(t);
	}

	if (!has_normals)
		this->calcNormals();
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
	double sumArea;
	for (int i = 0; i < vertices.size(); i++) {
		sumVector.setVlaues(0, 0, 0, 1);
		sumArea = 0;
		for (int j = 0; j < faces.size(); j++) {
			if (faces[j].isVertexInTriangle(vertices[i].vertex)) {
				sumVector += faces[j].getNormal()*faces[j].getArea();
				sumArea += faces[j].getArea();
			}
		}
		vertices[i].normal = sumVector * (1.0 / sumArea);
	}
}

void MeshModel::calcCentroid()
{
	Vector4 sum;
	for (int i = 0; i < vertices.size(); i++) {
		sum += vertices[i].vertex;
	}
	this->centroid = sum * (1.0 / vertices.size());
}

void MeshModel::transformMshMdl(Matrix4x4 &M)
{
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].vertex = vertices[i].vertex*M;
		vertices[i].normal = vertices[i].normal*M;
	}
	for (int j = 0; j < faces.size(); j++) {
		for (int k = 0; k < 3; k++) {
			faces[j][k] = faces[j][k] * M;
		}
	}
}

void MeshModel::transformMshMdlonlyVertices(Matrix4x4 &M)
{
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].vertex = vertices[i].vertex*M;
		vertices[i].normal = vertices[i].normal*M;
	}
}



void MeshModel::moveCentroidToOrigin()
{
	this->calcCentroid();
	Matrix4x4 M(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, (-1 * this->centroid[0]), (-1 * this->centroid[1]), (-1 * this->centroid[2]), 1);
	transformMshMdlonlyVertices(M);
}

void MeshModel::scale10units()
{
	double maxVal = 0.0;
	for (int i = 0; i < vertices.size(); i++){
		for (int j = 0; j < 3; j++) 
				if ((vertices[i].vertex[j])>maxVal) {
					maxVal = vertices[i].vertex[j];
				}
		/*if (vertices[i].vertex[0]>maxVal)
			maxVal = vertices[i].vertex[0];*/
		}
	double scaleValue = 10.0 / maxVal;
	Matrix4x4 M(scaleValue, 0, 0, 0, 0, scaleValue, 0, 0, 0, 0, scaleValue, 0, 0, 0, 0, 1);
	transformMshMdlonlyVertices(M);
}
