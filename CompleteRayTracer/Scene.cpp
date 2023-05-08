#include "Scene.h"
#include <chrono>
#include <iostream>

Scene::Scene(Camera* camera):
	camera(camera)
{
	
}

bool Scene::Intersects(Ray ray, RayHitInfo& hitInfo) const
{
	if (kdTree)
	{
		return KdTreeIntersects(KDTree, ray, hitInfo);
	}
	bool intersects = false;
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
	if (intersects)
	{
		hitInfo.WorldPosition = ray.worldPositionAtT(hitDistance);
		hitInfo.hitObject->fillNormal(hitInfo.WorldPosition, hitInfo.WorldNormal);
		hitInfo.HitDistance = hitDistance;
	}
	hitInfo.ray = ray;
	return intersects;
}

bool Scene::KdTreeIntersects(KdNode* N, Ray r, RayHitInfo& hitInfo) const
{
	bool intersects = false;
	hitInfo.hitObject = nullptr;
	float hitDistance = std::numeric_limits<float>::max();
	if (N->isLeaf)
	{
		KdLeaf* leaf = static_cast<KdLeaf*>(N);

		for (int i = 0; i < leaf->primitives.size(); i++)
		{
			float closestT = leaf->primitives[i].o->OnIntersect(r);
			if (closestT > 0 && closestT < hitDistance)
			{
				hitDistance = closestT;
				hitInfo.hitObject = leaf->primitives[i].o;
				intersects = true;

			}
		}
		hitInfo.ray = r;
		if (intersects)
		{
			hitInfo.WorldPosition = r.worldPositionAtT(hitDistance);
			hitInfo.hitObject->fillNormal(hitInfo.WorldPosition, hitInfo.WorldNormal);
			hitInfo.HitDistance = hitDistance;
		}
		return intersects;
	}
	else
	{
		KdInterior* node = static_cast<KdInterior*>(N);
		Voxel::intersectVoxel interData = node->v.intersect(r);
		float a;
		float b;
		float P;

		// Check to see what is the changing coordinate between the voxel and seperating plane
		// take the coordinate that changes which is all is need for case analysis
		for (int i=0;i<3;i++){
		switch (i)
		{
		case 0:
			a = interData.enter.x;
			b = interData.exit.x;
			P = node->s_plane.point.x;
			break;
		case 1:
			a = interData.enter.y;
			b = interData.exit.y;
			P = node->s_plane.point.y;
			break;
		case 2:
			a = interData.enter.z;
			b = interData.exit.z;
			P = node->s_plane.point.z;
			break;
		}
		// case 1: Only crosses node1 voxel (a and b below P)
		if (a <= P && b <= P)
			return KdTreeIntersects(node->node1, r, hitInfo);
		// case 2: Only crosses node2 voxel (a and b above P)
		else if (a > P && b > P)
			return KdTreeIntersects(node->node2, r, hitInfo);
		// case 3: Starts node1, goes to node2 (P between a and b, where a < b)
		else if (a < b)
		{
			bool intersection = KdTreeIntersects(node->node1, r, hitInfo);
			if (!intersection)
				return KdTreeIntersects(node->node2, r, hitInfo);
			else
				return intersection;
		}
		// case 4: Starts node2, goes to node1 (P between a and b, where a > b)
		else if (a > b)
		{
			bool intersection = KdTreeIntersects(node->node2, r, hitInfo);
			if (!intersection)
				return KdTreeIntersects(node->node1, r, hitInfo);
			else
				return intersection;
		}
	}
	}
}

void Scene::buildkdTree()
{

		auto start = std::chrono::high_resolution_clock::now();
		treeBuilder = KdTree();
		KDTree = treeBuilder.getNode(totalBound, voxelObjectList, 0);
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
		std::cout << "\nK-D TREE BUILD TIME: " << duration.count() << " milliseconds\n\n";
	
}

void Scene::calculateBoundingBoxVoxels()
{
	glm::vec3 min = { 0,0,0 };
	glm::vec3 max = { 0,0,0 };

	for (auto& o : Shapes)
	{
		if (dynamic_cast<Triangle*>(o.get()))
		{
			Triangle* t = dynamic_cast<Triangle*>(o.get());

			// Calculate voxel dimensions of Triangle
			min = t->V0;

			std::vector<glm::vec3> vertices = { t->V0, t->V1, t->V2};

			for (int i = 0; i < 3; i++)
			{
				if (vertices[i].x < min.x) min.x = vertices[i].x;
				if (vertices[i].y < min.y) min.y = vertices[i].y;
				if (vertices[i].z < min.z) min.z = vertices[i].z;
				if (vertices[i].x > max.x) max.x = vertices[i].x;
				if (vertices[i].y > max.y) max.y = vertices[i].y;
				if (vertices[i].z > max.z) max.z = vertices[i].z;
			}
		}
		else if (dynamic_cast<Sphere*>(o.get()))
		{
			Sphere* s = dynamic_cast<Sphere*>(o.get());

			// Calculate voxel dimensions of Sphere
			min.x = s->Position.x - s->Radius;
			min.y = s->Position.y - s->Radius;
			min.z = s->Position.z - s->Radius;
									   
			max.x = s->Position.x + s->Radius;
			max.y = s->Position.y + s->Radius;
			max.z = s->Position.z + s->Radius;
		}
		Voxel v = Voxel(min, max);
		voxelObjectWrapper vow = voxelObjectWrapper(o.get(), v);

		this->voxelObjectList.push_back(vow);
	}
	// calculate the min and max points for the bounding voxel of the entire world
	min = voxelObjectList[0].v.min;
	max = voxelObjectList[0].v.max;

	for (int i = 0; i < voxelObjectList.size(); i++)
	{
		if (voxelObjectList[i].v.min.x < min.x) min.x = voxelObjectList[i].v.min.x;
		if (voxelObjectList[i].v.min.y < min.y) min.y = voxelObjectList[i].v.min.y;
		if (voxelObjectList[i].v.min.z < min.z) min.z = voxelObjectList[i].v.min.z;
		if (voxelObjectList[i].v.max.x > max.x) max.x = voxelObjectList[i].v.max.x;
		if (voxelObjectList[i].v.max.y > max.y) max.y = voxelObjectList[i].v.max.y;
		if (voxelObjectList[i].v.max.z > max.z) max.z = voxelObjectList[i].v.max.z;
	}
	min.x -= 0.5f;
	min.y -= 0.5f;
	min.z -= 0.5f;
	max.x += 0.5f;
	max.y += 0.5f;
	max.z += 0.5f;
	totalBound = Voxel(min, max);
}
