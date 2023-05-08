#include "Voxel.h"
#include "Shapes.h"


struct KdPlane
{
	glm::vec3 point;
	glm::vec3 normal;
	KdPlane(glm::vec3 point, glm::vec3 normal)
	{
		this->point = point;
		this->normal = normal;
	}
};

class KdNode
{
public:
	KdNode() {}
	~KdNode() {}
	bool isLeaf = false;
};



class KdInterior : public KdNode
{
public:
	int changingAxis;

	KdPlane s_plane;
	Voxel v;

	KdNode* node1;
	KdNode* node2;

	KdInterior(KdPlane p, int changingAxis, Voxel v, KdNode* node1, KdNode* node2);

};

class KdLeaf : public KdNode
{
public:
	std::vector<voxelObjectWrapper> primitives;

	KdLeaf(std::vector<voxelObjectWrapper> p);


};

