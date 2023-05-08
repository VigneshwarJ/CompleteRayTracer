#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
struct Scene;

class Material
{
public:
	Material(){}

	~Material(){}

	virtual glm::vec4 GetColor(RayHitInfo hitInfo, const Scene& scene) =0;

	float ReflectiveConst = 0.0f;
	float TransperancyConst = 0.0f;
};

