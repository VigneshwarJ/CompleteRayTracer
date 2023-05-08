#pragma once
#define MAX_DEPTH 30
#include "Voxel.h"
#include "Shapes.h"
#include "KdNode.h"


class KdTree {
private:
	int i = 0;

public:
	KdTree(){}

	KdPlane getPartitionPlane(Voxel v);
	std::vector<voxelObjectWrapper> getObjectsInVoxel(Voxel v, std::vector<voxelObjectWrapper> primitives);
	KdNode* getNode(Voxel v, std::vector<voxelObjectWrapper> primitives, int depth);

};


