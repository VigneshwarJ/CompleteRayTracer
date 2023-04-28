#include "CheckerBoardTexture.h"
#include <bitset>

CheckerBoardTexture::CheckerBoardTexture(glm::vec3 color1, glm::vec3 color2, float boxSize):
	color1(color1),
	color2(color2),
	boxResolution(boxSize)
{
}

glm::vec3 CheckerBoardTexture::value(float u, float v)
{
	std::bitset<2> checkBits;
	checkBits.set(0, ((int)floor(u * boxResolution)) % 2 == 0);
	checkBits.set(1, ((int)floor(v * boxResolution)) % 2 == 0);

	//basically checking if they are both even or both odd
	if (!(checkBits[0] ^ checkBits[1])) {
		return color1;
	}

	return color2;
}
