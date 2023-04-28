#pragma once
#include <glm/glm.hpp>

class Texture {
public:
	virtual glm::vec3 value(float u, float v) = 0;
	virtual ~Texture() = default;
};

