#pragma once
#include "Vector4.h"
#include "Triangle.h"
#include "Obj Parser\wavefront_obj.h"

class MeshModel
{
	typedef struct {
		Vector4 vertex;
		Vector4 normal;
		//bool normalIsSet;
	}vertexInfo;

	Wavefront_obj obj;
	std::vector<vertexInfo>  vertices;
	std::vector<Triangle> model;
	Vector4 center;//****temp**** may not need it 

public:
	//constructors
	MeshModel();
	MeshModel(Wavefront_obj&);

	//destructor
	~MeshModel();

	//get functions
	Vector4 getCenterOfModel();
	inline int getNumberOfTriangles() { return model.size(); };//get number of triangles
	inline int getNumberOfVertices() { return vertices.size(); };//get number of vertices
	inline int getNumberOfNormals() { return vertices.size(); };//get number of normals
	inline Vector4 getVertex(int index) { return vertices[index].vertex; };//get vertex by index
	inline Vector4 getNormal(int index) { return vertices[index].normal; }; // get normal by index
	inline Triangle getTriangle(int index) { return model[index]; }; // get normal by index


	//call methods 
	void CalcNormals();
};