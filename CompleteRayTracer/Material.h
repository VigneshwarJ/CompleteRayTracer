#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
class Scene;
class Material
{
public:
	Material(){}

	~Material(){}

	virtual glm::vec4 GetColor(RayHitInfo hitInfo, const Scene& scene) =0;

};

