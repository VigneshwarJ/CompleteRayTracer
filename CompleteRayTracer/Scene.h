#include <vector>
#include "Ray.h"
#include "Shapes.h"
#include "KDTree.h"

struct Scene {
	Scene(Camera* camera = nullptr);
	std::vector<Light> pointLights;
	glm::vec4 SkyColor{ 0,0,1.0f,1.0f };
	const Camera* camera;
	bool Intersects(Ray ray, RayHitInfo& info) const;
	bool KdTreeIntersects(KdNode* N, Ray ray, RayHitInfo& info) const;
	void buildkdTree();
	void calculateBoundingBoxVoxels();

	std::vector<std::unique_ptr<Shape>> Shapes;
	std::vector<std::unique_ptr<Material>> materials;
	std::vector<std::unique_ptr<Texture>> textures;
	bool kdTree = false;
	KdTree treeBuilder;
	KdNode* KDTree = nullptr;
	Voxel totalBound;
	std::vector<voxelObjectWrapper> voxelObjectList;
};
