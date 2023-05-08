#include "Shapes.h"
#include "Material.h"
#include <iostream>
#include "TriangleBoxIntersection.h"

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
	float neg = ( - b - glm::sqrt(discriminant)) / (2.0f * a);
	float closestT = (-b + glm::sqrt(discriminant)) / (2.0f * a) < neg ? (-b + glm::sqrt(discriminant)) / (2.0f * a):neg;
	return closestT;
}

void Sphere::fillNormal(glm::vec3& position, glm::vec3& normal) const
{
	normal = glm::normalize(Position - position);
}

bool Sphere::inVoxel(Voxel v)
{
	float d = 0.0f;

	for (size_t i = 0; i < 3; i++)
	{

	

		float e = Position[i] - v.min[i];

		if (e < 0)
		{
			if (e < -Radius)
			{
				return false;
			}
			d = d + (e * e);
		}
		else if ((e = Position[i] - v.max[i]) > 0)
		{
			if (e > Radius)
			{
				return false;
			}
			d = d + (e * e);
		}
	}

	if (d > Radius*Radius)
	{
		return false;
	}

	return true;
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
glm::vec2 Plane::getUV(glm::vec3& position) const
{
	//glm::vec3 e1 = glm::normalize(glm::cross(Normal, Vertices[0]));
	glm::vec3 e1 = glm::normalize(glm::vec3{Normal.y, Normal.x, 0});
	glm::vec3 e2 = glm::cross(Normal, e1);
	float u = glm::dot(position, e1);
	float v = glm::dot(position, e2);
	return { u,v };
}

void Plane::fillNormal(glm::vec3& position, glm::vec3& normal) const
{
	normal = Normal;
}

Triangle::Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, Material* mat) :
	V0(v0),
	V1(v1),
	V2(v2),
	F(0.0f),
	Position(0.0f)
{
	m_Material = mat;
	Normal = glm::normalize(glm::cross(v1 - v0, v2 -v0));
	F = -glm::dot(Normal,v0);
}

void Triangle::fillNormal(glm::vec3& position, glm::vec3& normal) const
{
	normal = Normal;
}
bool Triangle::inVoxel(Voxel v)
{
	glm::vec3 spatialMedian = (v.max + v.min) / 2.0f;

	glm::vec3  boxhalfsize = spatialMedian - v.min;

	return triBoxOverlap(spatialMedian, boxhalfsize, V0, V1, V2);

}

bool Plane::inVoxel(Voxel v)
{
	glm::vec3 spatialMedian = (v.max + v.min) / 2.0f;

	glm::vec3  boxhalfsize = spatialMedian - v.min;

	 if (!planeBoxOverlap(Normal, Vertices[0], boxhalfsize))
		return false; 
	 return true;

}

float Triangle::OnIntersect(const Ray& ray) const
{


	glm::vec3 e1 = V1 - V0;
	glm::vec3 e2 = V2 - V0;

	glm::vec3 N = glm::cross(e1, e2);
	N = glm::normalize(N);

	glm::vec3 pVec = glm::cross(ray.Direction, e2);
	float det = glm::dot(pVec, e1);

	if (det < 1e-8f) return false;

	float invDet = 1 / det;

	glm::vec3 tvec = ray.Origin - V0;
	float u = glm::dot(tvec, pVec) * invDet;
	if (u < 0 || u > 1) return false;

	glm::vec3 qvec = glm::cross(tvec, e1);
	float v = glm::dot(ray.Direction, qvec) * invDet;
	if (v < 0 || u + v > 1) return false;
	float t = glm::dot(e2, qvec) * invDet;

	float w = 1 - u - v;

	return t;

}