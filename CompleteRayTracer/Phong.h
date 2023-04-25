#pragma once
#include "Material.h"
#include <glm/glm.hpp>

class Phong : public Material
{
public:
	Phong(glm::vec3 objectColor, glm::vec3 specColor, float ambient, float diffuse, float specular, float specHighlight);

	glm::vec4 GetColor(RayHitInfo record, const Scene& scene) override;

private:
	float k_a;	// ambient
	float k_d;	// diffuse
	float k_s;	// specular
	float k_e;	// spec highlight
	glm::vec3 C_o;  // object color
	glm::vec3 C_s;  // specular color

};

