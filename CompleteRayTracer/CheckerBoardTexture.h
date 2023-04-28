#pragma once
#include "Texture.h"
class CheckerBoardTexture : public Texture {
	float boxResolution;
	glm::vec3 color1;
	glm::vec3 color2;
public:
	CheckerBoardTexture(glm::vec3 color1, glm::vec3 color2, float boxSize);
	virtual glm::vec3 value(float u, float v) override;
};

