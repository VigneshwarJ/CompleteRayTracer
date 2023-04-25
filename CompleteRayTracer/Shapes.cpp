#include "Shapes.h"
#include "Material.h"
#include <iostream>
Sphere::Sphere() {

}

Sphere::Sphere(glm::vec3 position, float radius, Material* material) :
	Position(position),
	Radius(radius)
{
	m_Material = material;
}

float Sphere::OnIntersect(const Ray& ray) const
{
	glm::vec3 origin = ray.Origin - Position;

	float a = glm::dot(ray.Direction, ray.Direction);
	float b = 2.0f * glm::dot(origin, ray.Direction);
	float c = glm::dot(origin, origin) - Radius * Radius;

	// Quadratic forumula discriminant:
	// b^2 - 4ac

	float discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f)
		return -1;

	// Quadratic formula:
	// (-b +- sqrt(discriminant)) / 2a

	// float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a); // Second hit distance (currently unused)
	float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
	return closestT;
}

void Sphere::fillNormal(glm::vec3& position, glm::vec3& normal) const
{
	normal = glm::normalize(position - Position);
}

Plane::Plane(std::vector<glm::vec3> v_list, Material* mat) :
	Vertices(v_list),
	F(0.0f),
	Position(0.0f)
{
	m_Material = mat;
	Normal = glm::normalize(glm::cross(v_list[1] - v_list[0], v_list[2] - v_list[0]));
	F = -glm::dot(Normal, Vertices[0]);
}

float Plane::OnIntersect(const Ray& ray) const
{
	//glm::vec3 origin = ray.Origin - glm::vec3(Vertices[3].x * .5 + Vertices[0].x * .5, Vertices[2].y * .5 + Vertices[1].y * .5, Vertices[1].z * .5 + Vertices[2].z * .5);
	glm::vec3 origin = ray.Origin - Vertices[0];


	float denom = glm::dot(Normal, ray.Direction);
	if (denom !=0)
	{
		
		float num = -1 * (glm::dot(Normal, origin) + F);
		float omega = num / denom;

		if (omega > 0.0001f)
		{
			
			glm::vec3 intersection =  origin + (ray.Direction * omega);
			glm::vec3 A = glm::normalize( Vertices[Vertices.size() - 1] - intersection);
			glm::vec3 B = glm::normalize(Vertices[0] - intersection);
			float totalAngle = glm::degrees(acos(glm::dot(A, B)));
			for (size_t i = 0; i < Vertices.size() - 1; i++)
			{
				A = glm::normalize( Vertices[i] - intersection);
				B = glm::normalize( Vertices[i + 1] - intersection);
				totalAngle += glm::degrees(acos(glm::dot(A, B)));
			}
			//std::cout << totalAngle<<"\n";
			if (totalAngle > 359.5f && totalAngle < 360.5f)
			{
				//std::cout << "here\n";
				return omega;
			}
		}
	}
	return -1.0f;
}

void Plane::fillNormal(glm::vec3& position, glm::vec3& normal) const
{

}

bool Scene::Intersects(Ray ray, RayHitInfo& hitInfo) const
{
	bool intersects=false;
	hitInfo.hitObject = nullptr;
	float hitDistance = std::numeric_limits<float>::max();
	for (auto& shape : Shapes)
	{
		float closestT = shape->OnIntersect(ray);
		if (closestT > 0 && closestT < hitDistance)
		{
			hitDistance = closestT;
			hitInfo.hitObject = shape.get();
			intersects = true;
			
		}
	}
	if(intersects)
	{
		hitInfo.WorldPosition = ray.worldPositionAtT(hitDistance);
		hitInfo.hitObject->fillNormal(hitInfo.WorldPosition, hitInfo.WorldNormal);
		hitInfo.HitDistance = hitDistance;
	}
	return intersects;
}
