#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "Material.h"
#include "Ray.h"
#include <vector>
#include "Camera.h"
#include "Texture.h"
#include "Voxel.h"





class Shape{
public:
	 virtual float  OnIntersect(const Ray& ray) const = 0;
	 virtual void fillNormal(glm::vec3& position, glm::vec3& normal) const = 0;
	 virtual glm::vec2 getUV(glm::vec3& position) const { return { 0,0 }; }
	 Material* GetMaterial() const { return m_Material; }
	 virtual bool inVoxel(Voxel v)
	 {
		 return false;
	 }

protected:
	 Material* m_Material = nullptr;
};

struct Light {
	glm::vec3 Position;
	glm::vec3 color;
	float intensity;
};


class Sphere : public Shape
{
public:
	Sphere();
	Sphere(glm::vec3 position, float radius, Material* material);
	float  OnIntersect(const Ray& ray) const override;
	void fillNormal(glm::vec3& position, glm::vec3& normal) const override;
	bool inVoxel(Voxel v) override;

	glm::vec3 Position{ 0.0f };
	float Radius = 0.5f;
private:

};

class Plane : public Shape
{
public:
	Plane(std::vector<glm::vec3> v_list, Material* mat);

	float  OnIntersect(const Ray& ray) const override;
	void fillNormal(glm::vec3& position, glm::vec3& normal) const override;
	glm::vec2 getUV(glm::vec3& position) const override;
	bool inVoxel(Voxel v) override;
	glm::vec3 Normal;
private:
	std::vector<glm::vec3> Vertices;
	float F;
	glm::vec3 Position;

};

class Triangle : public Shape
{
public:

	Triangle (glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, Material* mat);
	void fillNormal(glm::vec3& position, glm::vec3& normal) const override;

	float OnIntersect(const Ray& ray) const override;
	bool inVoxel(Voxel v) override;


	glm::vec3 Position;
	glm::vec3 V0,V1,V2;
private:
	glm::vec3 Normal;
	float F;
};

struct voxelObjectWrapper
{
	Shape* o;
	Voxel v;

	voxelObjectWrapper(Shape* object, Voxel voxel)
	{
		this->o = object;
		this->v = voxel;
	};
};