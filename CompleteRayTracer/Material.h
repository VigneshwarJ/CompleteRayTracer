#pragma once
#include <glm/glm.hpp>

class Material
{
public:
	Material(glm::vec3 objectColor, glm::vec3 specColor, float roughness);

	~Material(){}
	glm::vec3 Albedo{ 1.0f };
	glm::vec3 Specular{ 1.0f };
	float Roughness = 1.0f;
	float Metallic = 0.0f;
};

