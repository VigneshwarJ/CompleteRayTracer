#include "Voxel.h"


Voxel::Voxel(glm::vec3 min, glm::vec3 max)
{
	this->min = min;
	this->max = max;
	this->bounds[0] = this->min;
	this->bounds[1] = this->max;
}

Voxel::intersectVoxel Voxel::intersect(Ray r)
{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	tmin = (bounds[r.invSign[0]].x - r.Origin.x) * r.invDir[0];
	tmax = (bounds[1 - r.invSign[0]].x - r.Origin.x) * r.invDir[0];
	tymin = (bounds[r.invSign[1]].y - r.Origin.y) * r.invDir[1];
	tymax = (bounds[1 - r.invSign[1]].y - r.Origin.y) * r.invDir[1];

	if ((tmin > tymax) || (tymin > tmax))
		return Voxel::intersectVoxel(false);
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	tzmin = (bounds[r.invSign[2]].z - r.Origin.z) * r.invDir[2];
	tzmax = (bounds[1 - r.invSign[2]].z - r.Origin.z) * r.invDir[2];

	if ((tmin > tzmax) || (tzmin > tmax))
		return Voxel::intersectVoxel(false);
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	// Ray intersects Voxel, return points a and b
	float xi, yi, zi;
	glm::vec3 a, b;

	if (tmin < 0 && tmax < 0)
	{
		return Voxel::intersectVoxel(false);
	}
	else if (tmin < 0)
	{
		xi = r.Origin.x + r.Direction[0] * tmax;
		yi = r.Origin.x + r.Direction[1] * tmax;
		zi = r.Origin.x + r.Direction[2] * tmax;
		b = glm::vec3(xi, yi, zi);
		a = r.Origin;
	}
	else if (tmax < 0)
	{
		xi = r.Origin.x + r.Direction[0] * tmin;
		yi = r.Origin.x + r.Direction[1] * tmin;
		zi = r.Origin.x + r.Direction[2] * tmin;
		b = glm::vec3(xi, yi, zi);
		a = r.Origin;
	}
	else
	{
		xi = r.Origin.x + r.Direction[0] * tmax;
		yi = r.Origin.x + r.Direction[1] * tmax;
		zi = r.Origin.x + r.Direction[2] * tmax;
		b = glm::vec3(xi, yi, zi);
		xi = r.Origin.x + r.Direction[0] * tmin;
		yi = r.Origin.x + r.Direction[1] * tmin;
		zi = r.Origin.x + r.Direction[2] * tmin;
		a = glm::vec3(xi, yi, zi);
	}
	return Voxel::intersectVoxel(true, a, b);
}

bool Voxel::inVoxel(Voxel v)
{
	if (v.min.x > v.max.x || v.min.y > v.max.y || v.min.z > v.max.z)
		return false;
	return true;
}

