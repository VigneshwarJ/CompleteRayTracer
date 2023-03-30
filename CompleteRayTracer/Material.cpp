#include "Material.h"

Material::Material(glm::vec3 objectColor, glm::vec3 specColor, float roughness):
	Albedo(objectColor),
	Specular(specColor),
	Roughness(roughness)
{
}
