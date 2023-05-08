#include "KdNode.h"

KdInterior::KdInterior(KdPlane p, int ca, Voxel v, KdNode* node1, KdNode* node2) : 
	KdNode(), 
	s_plane(KdPlane(p.point, p.normal))
{
	this->changingAxis = ca;
	this->v = v;
	this->node1 = node1;
	this->node2 = node2;
}

KdLeaf::KdLeaf(std::vector<voxelObjectWrapper> p) : KdNode()
{
	this->primitives = p;

	isLeaf = true;
}