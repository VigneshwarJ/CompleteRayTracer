#include "KDTree.h"

std::vector<voxelObjectWrapper> KdTree::getObjectsInVoxel(Voxel v, std::vector<voxelObjectWrapper> primitives)
{
	std::vector<voxelObjectWrapper> newList;
	for (int i = 0; i < primitives.size(); i++)
	{
		if (primitives[i].o->inVoxel(v))
		{
			newList.push_back(primitives[i]);
		}
	}
	return newList;
}

KdPlane KdTree::getPartitionPlane(Voxel v)
{
	// Alternate axis
	this->i++;

	glm::vec3 normal;
	switch (this->i % 3)
	{
	case 0:
		normal = glm::vec3(1, 0, 0);
		break;
	case 1:
		normal = glm::vec3(0, 1, 0);
		break;
	case 2:
		normal = glm::vec3(0, 0, 1);
		break;
	default:
		normal = glm::vec3(1, 0, 0);
		break;
	}

	glm::vec3 spatialMedian = (v.max + v.min) / 2.0f;
	return KdPlane(spatialMedian, normal);
}

KdNode* KdTree::getNode(Voxel v, std::vector<voxelObjectWrapper> primitives, int depth)
{
	if (primitives.size() <= 1 || depth == MAX_DEPTH)
	{
		if (depth == MAX_DEPTH)
		{
			depth = MAX_DEPTH;
		}
		KdNode* retNode = new KdLeaf(primitives);
		return retNode;
	}
	else
	{
		// Create a partitioning plane
		KdPlane P = getPartitionPlane(v);
		int partitionAxis = i % 3;

		// Create new voxels based on partitioning plane
		Voxel v1 = Voxel(v.min, v.max);
		Voxel v2 = Voxel(v.min, v.max);
		float seperateValue;

		switch (this->i % 3)
		{
		case 0:
			seperateValue = (v.max.x + v.min.x) / 2.0f;
			v1.max.x = seperateValue;
			v2.min.x = seperateValue;
			break;
		case 1:
			seperateValue = (v.max.y + v.min.y) / 2.0f;
			v1.max.y = seperateValue;
			v2.min.y = seperateValue;
			break;
		case 2:
			seperateValue = (v.max.z + v.min.z) / 2.0f;
			v1.max.z = seperateValue;
			v2.min.z = seperateValue;
			break;
		}

		// Split list of objects based on the partitioning plane
		std::vector<voxelObjectWrapper> L_1 = getObjectsInVoxel(v1, primitives);
		std::vector<voxelObjectWrapper> L_2 = getObjectsInVoxel(v2, primitives);

		KdNode* node1 = getNode(v1, L_1, depth + 1);
		KdNode* node2 = getNode(v2, L_2, depth + 1);

		KdNode* retNode = new KdInterior(P, partitionAxis, v, node1, node2);
		return retNode;
	}
}
