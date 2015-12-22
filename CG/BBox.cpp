#include "BBox.h"

BBox::BBox(MeshModel model)
{
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

BBox::BBox()
{
}

BBox::~BBox()
{
}

void BBox::setVertices(MeshModel& model) {
	box_center = model.getCentroid();
	double maxX, minX, maxY, minY, maxZ, minZ;
	model.getMinMaxValues(maxX, minX, maxY, minY, maxZ, minZ);
	box_vertices[0] = Vector4(maxX, minY, minZ, 1);
	box_vertices[1] = Vector4(maxX, maxY, minZ, 1);
	box_vertices[2] = Vector4(minX, minY, minZ, 1);
	box_vertices[3] = Vector4(minX, maxY, minZ, 1);
	box_vertices[4] = Vector4(maxX, minY, maxZ, 1);
	box_vertices[5] = Vector4(maxX, maxY, maxZ, 1);
	box_vertices[6] = Vector4(minX, minY, maxZ, 1);
	box_vertices[7] = Vector4(minX, maxY, maxZ, 1);
}
void BBox::transformBox(Matrix4x4& m)
{
	for (int i = 0; i < 8; i++) {
		box_vertices[i] =box_vertices[i]*m;
	}
	box_center *= m;
}
void BBox::drawBox()
{
	Line ln;
	ln.setStartCrd(box_vertices[0], 0xff0000);
	ln.setEndCrd(box_vertices[1], 0xff0000);
	ln.drawline();
	ln.setStartCrd(box_vertices[1], 0xff0000);
	ln.setEndCrd(box_vertices[3], 0xff0000);
	ln.drawline();
	ln.setStartCrd(box_vertices[2], 0xff0000);
	ln.setEndCrd(box_vertices[3], 0xff0000);
	ln.drawline();
	ln.setStartCrd(box_vertices[0], 0xff0000);
	ln.setEndCrd(box_vertices[2], 0xff0000);
	ln.drawline();
	ln.setStartCrd(box_vertices[4], 0xff0000);
	ln.setEndCrd(box_vertices[5], 0xff0000);
	ln.drawline();
	ln.setStartCrd(box_vertices[5], 0xff0000);
	ln.setEndCrd(box_vertices[7], 0xff0000);
	ln.drawline();
	ln.setStartCrd(box_vertices[6], 0xff0000);
	ln.setEndCrd(box_vertices[7], 0xff0000);
	ln.drawline();
	ln.setStartCrd(box_vertices[4], 0xff0000);
	ln.setEndCrd(box_vertices[6], 0xff0000);
	ln.drawline();
	ln.setStartCrd(box_vertices[0], 0xff0000);
	ln.setEndCrd(box_vertices[4], 0xff0000);
	ln.drawline();
	ln.setStartCrd(box_vertices[1], 0xff0000);
	ln.setEndCrd(box_vertices[5], 0xff0000);
	ln.drawline();
	ln.setStartCrd(box_vertices[3], 0xff0000);
	ln.setEndCrd(box_vertices[7], 0xff0000);
	ln.drawline();
	ln.setStartCrd(box_vertices[2], 0xff0000);
	ln.setEndCrd(box_vertices[6], 0xff0000);
	ln.drawline();
}
