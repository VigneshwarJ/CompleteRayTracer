#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
class Voxel
{
public:
	struct intersectVoxel
	{
		bool intersect;
		glm::vec3 enter;
		glm::vec3 exit;

		intersectVoxel(bool inter)
		{
			intersect = inter;
		};

		intersectVoxel(bool inter, glm::vec3 ent, glm::vec3 exi)
		{
			intersect = inter;
			enter = ent;
			exit = exi;
		};
	};
	Voxel() {}
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 bounds[2];

	Voxel(glm::vec3 min, glm::vec3 max);

	intersectVoxel intersect(Ray r);
	bool inVoxel(Voxel v);

};

