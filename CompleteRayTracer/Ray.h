#pragma once

#include <glm/glm.hpp>
#include <vector>
class Shape;

struct Ray
{
	glm::vec3 Origin;
	glm::vec3 Direction;

	glm::vec3 worldPositionAtT(float t)
	{
		return Origin + t * Direction;
	}
};

struct RayHitInfo
{
	float HitDistance = -1;
	glm::vec3 WorldPosition;
	glm::vec3 WorldNormal;
	std::vector<glm::vec3> Reflection;
	Ray ray;
	const Shape* hitObject;
};
