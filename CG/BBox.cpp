#include "BBox.h"

BBox::BBox(MeshModel model)
{
	//Vector4 box_vertices[8];
	//Vector4 box_center;
	box_center = model.getCentroid();
	double maxX , minX, maxY, minY, maxZ, minZ;
	model.getMinMaxValues(maxX, minX, maxY, minY, maxZ, minZ);
	box_vertices[0] = Vector4(maxX, minY, maxZ, 1);
	box_vertices[1] = Vector4(maxX, maxY, minZ, 1);
	box_vertices[2] = Vector4(minX, minY, minZ, 1);
	box_vertices[3] = Vector4(minX, maxY, minZ, 1);
	box_vertices[4] = Vector4(maxX, minY, maxZ, 1);
	box_vertices[5] = Vector4(maxX, maxY, maxZ, 1);
	box_vertices[6] = Vector4(minX, minY, maxZ, 1);
	box_vertices[7] = Vector4(minX, maxY, maxZ, 1);
}

BBox::~BBox()
{
}

void BBox::drawObject()
{
}
