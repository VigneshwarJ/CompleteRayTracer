#include "Renderer.h"
#include "Ray.h"

static uint32_t ConvertToRGBA(const glm::vec4& color)
{
	uint8_t r = (uint8_t)(color.r * 255.0f);
	uint8_t g = (uint8_t)(color.g * 255.0f);
	uint8_t b = (uint8_t)(color.b * 255.0f);
	uint8_t a = (uint8_t)(color.a * 255.0f);

	uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
	return result;
}


void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		// No resize necessary
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Image>(width, height, ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

void Renderer::Render(const std::vector<std::unique_ptr<Shape>>& shapes, const Camera& camera)
{
	m_Shapes = &shapes;
	m_Camera = &camera;
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
	
			m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(x,y);
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(uint32_t x,uint32_t y)
{
	Ray ray;
	ray.Origin = m_Camera->GetPosition();
	ray.Direction = m_Camera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];
	const Shape* closestShape = nullptr;
	float hitDistance = std::numeric_limits<float>::max();
	for ( auto& shape : *m_Shapes)
	{
		float closestT = shape->OnIntersect(ray);
		if (closestT > 0 && closestT < hitDistance)
		{
			hitDistance = closestT;
			closestShape = shape.get();
		}

	}
	if (!closestShape)
	{
		return ConvertToRGBA(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	return ConvertToRGBA(glm::vec4(closestShape->GetMaterial()->Albedo, 1.0f));
	
}