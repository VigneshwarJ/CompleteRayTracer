#pragma once

#include "Image.h"

#include <memory>
#include <glm/glm.hpp>
#include "Shapes.h"
#include "Camera.h"

class Renderer
{
public:
	struct Settings
	{
		bool Accumulate = true;
	};
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render(const Scene& shapes, const Camera& camera);

	void ResetFrameIndex() { m_FrameIndex = 1; }
	std::shared_ptr<Image> GetFinalImage() const { return m_FinalImage; }
private:

	glm::vec4 PerPixel(uint32_t x, uint32_t y);
	glm::vec4 TraceRay(const Ray& ray);
	glm::vec4 ClosestHit(const Ray& ray,RayHitInfo& info);

	glm::vec4 Miss(const Ray& ray);

	//glm::vec3 CalculateLighting(const RayHitInfo& record);
private:
	Settings m_Settings;
	std::shared_ptr<Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
	glm::vec4* m_AccumulationData = nullptr;
	const Scene* m_scene = nullptr;
	const Camera* m_Camera = nullptr;
	uint32_t m_FrameIndex = 1;
};