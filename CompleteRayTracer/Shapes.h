#pragma once
#include <glm/glm.hpp>
#include "Material.h"
#include "Ray.h"
#include <vector>

class Shape{
public:
	 virtual float  OnIntersect(const Ray& ray) const = 0;
	 Material* GetMaterial() const { return m_Material; }
protected:
	 Material* m_Material = nullptr;
};

class Sphere : public Shape
{
public:
	Sphere();
	Sphere(glm::vec3 position, float radius, Material* material);
	float  OnIntersect(const Ray& ray) const override;


private:
	glm::vec3 Position{ 0.0f };
	float Radius = 0.5f;

};

class Plane : public Shape
{
public:
	Plane(std::vector<glm::vec3> v_list, Material* mat);

	float  OnIntersect(const Ray& ray) const override;


private:
	std::vector<glm::vec3> Vertices;
	glm::vec3 Normal;
	float F;
	glm::vec3 Position;

};