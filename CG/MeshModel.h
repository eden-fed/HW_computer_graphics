#pragma once
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Triangle.h"
#include "Obj Parser\wavefront_obj.h"
#include <set>
#include "Line.h"
#include "Material.h"

class MeshModel
{
	typedef struct {
		Vector4 vertex;
		Vector4 normal;
		//bool normalIsSet;
	}vertexInfo;

	//Wavefront_obj obj;
	std::vector<vertexInfo>  vertices;
	std::vector<Triangle> faces;
	Vector4 centroid;//****temp**** may not need it 
	std::set<Line> linesToDraw;
	

public:
	Material material;

	//constructors
	MeshModel();
	MeshModel(Wavefront_obj& J);

	//destructor
	~MeshModel();

	//get functions
	Vector4 getCentroid();
	inline int getNumberOfFaces() { return faces.size(); };//get number of triangles
	inline int getNumberOfVertices() { return vertices.size(); };//get number of vertices
	inline int getNumberOfNormals() { return vertices.size(); };//get number of normals
	inline Vector4 getVertex(int index) { return vertices[index].vertex; };//get vertex by index
	inline Vector4 getNormal(int index) { return vertices[index].normal; }; // get normal by index
	inline Triangle getFace(int index) { return faces[index]; }; // get normal by index
	inline std::vector<Triangle>& getAllFaces() { return faces; }; // get normal by index


	//class methods 
	void calcNormals();
	void calcCentroid();
	void transformMshMdl(Matrix4x4 &M);
	void transformMshMdlonlyVertices(Matrix4x4 &M);
	void transformNormals(Matrix4x4 &M);
	void getMinMaxValues(double& maxXpos, double& maxXneg, double& maxYpos, double& maxYneg, double& maxZpos, double& maxZneg);
	void scale10units();
	void moveCentroidToOrigin();
	void populatelinesToDraw();
	void drawModelEdges();
	void drawNormals(double normSize);

};