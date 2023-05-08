#pragma once

#include <glm/glm.hpp>
#include <vector>
class Shape;

struct Ray
{
	glm::vec3 Origin;
	glm::vec3 Direction;
	glm::vec3 invDir;
	glm::uvec3 invSign;

	glm::vec3 worldPositionAtT(float t)
	{
		return Origin + t * Direction;
	}

	void preprocessForVoxel()
	{
		invDir = glm::vec3(1.0f / Direction[0], 1.0f / Direction[1], 1.0f / Direction[2]);
		invSign[0] = (invDir[0] < 0);
		invSign[1] = (invDir[1] < 0);
		invSign[2] = (invDir[2] < 0);
	}
};

struct RayHitInfo
{
	float HitDistance = -1;
	glm::vec3 WorldPosition;
	glm::vec3 WorldNormal;
	glm::vec2 uv;
	std::vector<glm::vec3> Reflection;
	Ray ray;
	const Shape* hitObject;
};
