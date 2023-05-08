#include "Renderer.h"
#include "Ray.h"
#include "Scene.h"

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
	delete[] m_AccumulationData;
	m_AccumulationData = new glm::vec4[width * height];
}

void Renderer::Render(const Scene& scene, const Camera& camera)
{
	bounces = 0;
	m_scene = &scene;
	m_Camera = &camera;
	luminanceBuffer = new float[m_FinalImage->GetWidth() * m_FinalImage->GetHeight()];
	maxLuminance = 1000;
	if (m_FrameIndex == 1)
		memset(m_AccumulationData, 0, m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(glm::vec4));
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
	
			//m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(x,y);
			glm::vec4 color = PerPixel(x, y);
			m_AccumulationData[x + y * m_FinalImage->GetWidth()] += color;

			glm::vec4 accumulatedColor = m_AccumulationData[x + y * m_FinalImage->GetWidth()];
			accumulatedColor /= (float)m_FrameIndex;
			accumulatedColor = glm::vec4(sqrt(accumulatedColor.x), sqrt(accumulatedColor.y), sqrt(accumulatedColor.z),1);
			luminanceBuffer[x + y * m_FinalImage->GetWidth()] = (0.27 * accumulatedColor.x) + (0.67 * accumulatedColor.y) + (0.06 * accumulatedColor.z);
			if (luminanceBuffer[x + y * m_FinalImage->GetWidth()] > maxLuminance) {
				maxLuminance = luminanceBuffer[x + y * m_FinalImage->GetWidth()];
			}
			accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = ConvertToRGBA(accumulatedColor);
		}
	}
	float logAverageLuminance = calculateLogAverageLuminance(luminanceBuffer, m_FinalImage->GetWidth(), m_FinalImage->GetHeight());
	//applyReinhard(logAverageLuminance, m_FinalImage->GetWidth(), m_FinalImage->GetHeight(), m_AccumulationData);
	//applyWard(logAverageLuminance, m_FinalImage->GetWidth(), m_FinalImage->GetHeight(), m_AccumulationData);
	m_FinalImage->SetData(m_ImageData);

	//if (m_Settings.Accumulate)
	//	m_FrameIndex++;
	//else
		m_FrameIndex = 1;
}

glm::vec4 Renderer::PerPixel(uint32_t x,uint32_t y)
{
	Ray ray;
	ray.Origin = m_Camera->GetPosition();
	ray.Direction = m_Camera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];
	ray.preprocessForVoxel();
	return TraceRay(ray, 10);
}

glm::vec4 Renderer::TraceRay(const Ray& ray,int depth)
{
	RayHitInfo info;
	;
	if (!m_scene->Intersects(ray,info)) // Ray miss
	{
		return Miss(ray);
	}
	glm::vec4 color = ClosestHit(ray, info);
	glm::vec4 IndirecTcolor{ 0,0,0,0 };
	
	if (bounces < 10)
	{
		if (info.hitObject->GetMaterial()->ReflectiveConst > 0)
		{
			Ray reflected;
			reflected.Origin = info.WorldPosition + info.WorldNormal * 0.001f;
			reflected.Direction = glm::reflect(ray.Direction, info.WorldNormal);
			reflected.preprocessForVoxel();
			IndirecTcolor = IndirecTcolor + TraceRay(reflected, depth + 1) * info.hitObject->GetMaterial()->ReflectiveConst;
		}
		if (info.hitObject->GetMaterial()->TransperancyConst > 0)
		{
			glm::vec3 normal = info.WorldNormal;
			float refractionIndex = 1.0f / info.hitObject->GetMaterial()->TransperancyConst;
			if (glm::dot(normal, -ray.Direction) < 0)
			{
				normal *= -1;
				refractionIndex = info.hitObject->GetMaterial()->TransperancyConst / 1.0f;

			}
			glm::vec3 refractedVector = glm::refract(ray.Direction, normal, refractionIndex);

			if (refractedVector.x == 0 && refractedVector.y == 0 && refractedVector.z == 0)
			{
				//total internal reflection
				refractedVector = glm::reflect(ray.Direction, normal);
			}

			Ray refractedRay;
			refractedRay.Origin = info.WorldPosition + 0.01f * refractedVector;
			refractedRay.Direction = refractedVector;
			refractedRay.preprocessForVoxel();

			glm::vec3 refractedColor = TraceRay(refractedRay, depth + 1);
			IndirecTcolor += glm::vec4(refractedColor, 1.0) * info.hitObject->GetMaterial()->TransperancyConst;
		}
	}
	return color + IndirecTcolor;
}

glm::vec4 Renderer::ClosestHit(const Ray& ray, RayHitInfo& info)
{	
	int bounces = 10;
	return info.hitObject->GetMaterial()->GetColor(info, *m_scene);

	//return color;
}

glm::vec4 Renderer::Miss(const Ray& ray)
{
	return m_scene->SkyColor;
}

float Renderer::calculateLogAverageLuminance(float* luminanceBuffer, int width, int height)
{
	double logAverage = 0;
	double delta = 0.00001;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			logAverage += log10(luminanceBuffer[i * width + j] + delta);
		}
	}
	logAverage = logAverage / ((float)(width * height));
	logAverage = exp(logAverage);
	return logAverage;
}

void Renderer::applyReinhard(float logAverageLuminance, int width, int height, glm::vec4* frameBuffer)
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			glm::vec4 temp = frameBuffer[i * width + j];
			temp = temp * (0.18f / logAverageLuminance);
			glm::vec4 newValue(temp.x / (1 + temp.x), temp.y / (1 + temp.y), temp.z / (1 + temp.z),1.0);
			newValue = glm::clamp(newValue * maxLuminance, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[i * width + j] = ConvertToRGBA(newValue);
		}
	}
}

void Renderer::applyWard(float logAverageLuminance, int width, int height, glm::vec4* frameBuffer)
{
	float top = 1.219 + pow((maxLuminance / 2), 0.4);
	float bottom = 1.219 + pow(logAverageLuminance, 0.4);
	float sf = pow((top / bottom), 2.5);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			glm::vec4 newValue = glm::clamp(frameBuffer[i * width + j] * sf , glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[i * width + j] = ConvertToRGBA(newValue);
		}
	}
	
}

void Renderer::applyAdaptiveLogMapping(float logAverageLuminance, int width, int height, glm::vec3* frameBuffer, float* luminanceBuffer)
{
	float scaledMaxLuminance = maxLuminance / logAverageLuminance;
	float exponent = log(0.85) / log(0.5);
	float denom1 = log10(scaledMaxLuminance + 1);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			float scaledPixelLuminance = luminanceBuffer[i * width + j] / logAverageLuminance;
			float num2 = log(scaledPixelLuminance + 1);
			float luminanceRatio = scaledPixelLuminance / scaledMaxLuminance;
			float denom2 = log(2 + ((pow(luminanceRatio, exponent) * 8)));

			float Ld = (1 / denom1) * (num2 / denom2);
			frameBuffer[i * width + j] = frameBuffer[i * width + j] * (Ld);
		}
	}

}