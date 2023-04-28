#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "Material.h"
#include "Ray.h"
#include <vector>
#include "Camera.h"
#include "Texture.h"





class Shape{
public:
	 virtual float  OnIntersect(const Ray& ray) const = 0;
	 virtual void fillNormal(glm::vec3& position, glm::vec3& normal) const = 0;
	 virtual glm::vec2 getUV(glm::vec3& position) const { return { 0,0 }; }
	 Material* GetMaterial() const { return m_Material; }

protected:
	 Material* m_Material = nullptr;
};

struct Light {
	glm::vec3 Position;
	glm::vec3 color;
	float intensity;
};

struct Scene {
	Scene():camera(nullptr){}
	std::vector<Light> pointLights;
	glm::vec4 SkyColor{ 0,0,1.0f,1.0f };
	const Camera*  camera;
	bool Intersects(Ray ray, RayHitInfo& info) const;
	std::vector<std::unique_ptr<Shape>> Shapes;
	std::vector<std::unique_ptr<Material>> materials;
	std::vector<std::unique_ptr<Texture>> textures;
};

class Sphere : public Shape
{
public:
	Sphere();
	Sphere(glm::vec3 position, float radius, Material* material);
	float  OnIntersect(const Ray& ray) const override;
	void fillNormal(glm::vec3& position, glm::vec3& normal) const override;

private:
	glm::vec3 Position{ 0.0f };
	float Radius = 0.5f;

};

class Plane : public Shape
{
public:
	Plane(std::vector<glm::vec3> v_list, Material* mat);

	float  OnIntersect(const Ray& ray) const override;
	void fillNormal(glm::vec3& position, glm::vec3& normal) const override;
	glm::vec2 getUV(glm::vec3& position) const override;
private:
	std::vector<glm::vec3> Vertices;
	glm::vec3 Normal;
	float F;
	glm::vec3 Position;

};