#pragma once

#include "Image.h"

#include <memory>
#include <glm/glm.hpp>
#include "Shapes.h"
#include "Camera.h"

class Renderer
{
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render(const std::vector<std::unique_ptr<Shape>>& shapes, const Camera& camera);

	std::shared_ptr<Image> GetFinalImage() const { return m_FinalImage; }
private:
	uint32_t PerPixel(uint32_t x, uint32_t y);
private:
	std::shared_ptr<Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
	const std::vector<std::unique_ptr<Shape>>* m_Shapes = nullptr;
	const Camera* m_Camera = nullptr;
};