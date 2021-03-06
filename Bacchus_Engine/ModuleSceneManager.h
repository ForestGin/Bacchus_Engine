#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Module.h"
#include <vector>
#include "Math.h"
#include "Color.h"
#include "Quadtree.h"

class GameObject;
class ComponentMaterial;
struct aiScene;
struct ImportMaterialData;
class ResourceMesh;

class ModuleSceneManager : public Module
{
public:

	ModuleSceneManager(bool start_enabled = true);
	~ModuleSceneManager();

	bool Init(json file);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	GameObject* CreateEmptyGameObject();
	ComponentMaterial* CreateEmptyMaterial();

	void Draw();
	GameObject* GetRootGO() const;
	void RedoOctree();
	void SetStatic(GameObject* go);
	void RecursiveDrawQuadtree(QuadtreeNode* node) const;
	void SelectFromRay(LineSegment& ray);

	void SaveStatus(json& file) const override;
	void LoadStatus(const json& file) override;
	void SaveScene();
	void LoadScene();
	void RecursiveFreeScene(GameObject* go);

	template <typename Box>
	static void DrawWire(const Box& box, Color color)
	{
		float3 corners[8];
		box.GetCornerPoints(corners);
		DrawWireFromVertices(corners, color);
	};
	static void DrawWireFromVertices(const float3* corners, Color color);
	
	GameObject* LoadTetrahedron();
	GameObject* LoadCube();
	GameObject* LoadOctahedron();
	GameObject* LoadIcosahedron();
	GameObject* LoadSphere();
	GameObject* LoadCubeSphere();
	GameObject* LoadIcoSphere();
	GameObject* LoadPyramid();
	GameObject* LoadCylinder();
	GameObject* LoadCone();

	void CreateGrid() const;

	GameObject* GetSelectedGameObjects() const;
	void SetSelectedGameObject(GameObject* go);
	void SetTextureToSelectedGO(uint id);
	void DestroyGameObject(GameObject* go);

private:
	void LoadPrimitiveArrays(ResourceMesh* new_mesh,
	uint vertices_size, const float* vertices,
	uint indices_size, const uint* indices,
	uint normals_size, const float* normals,
	uint texCoords_size, const float* texCoords) const;
	
	ResourceMesh* CreateSphere(float Radius = 1.0f, int sectors = 36, int stackCount = 18, bool smooth = false);
	ResourceMesh* CreateCubeSphere(float Radius = 1.0f, int sub = 3, bool smooth = false);
	ResourceMesh* CreateIcoSphere(float radius = 1.0f, int subdivision = 1, bool smooth = false);
	ResourceMesh* CreateCylinder(float baseRadius = 1.0f, float topRadius = 1.0f, float height = 1.0f, int sectorCount = 36, int stackCount = 1, bool smooth = false);
	
	void GatherGameObjects(std::vector<GameObject*>& scene_gos, GameObject* go);
	GameObject* CreateRootGameObject();
	void DrawScene();

public:
	ComponentMaterial* CheckersMaterial = nullptr;
	ComponentMaterial* DefaultMaterial = nullptr;

	Quadtree tree;//Octree*

	std::vector<GameObject*> NoStaticGo;

	bool display_tree = false;
	bool display_boundingboxes = false;

private:
	uint go_count = 0;
	GameObject* SelectedGameObject = nullptr;
	GameObject* root = nullptr;
	std::vector<ComponentMaterial*> Materials;
	std::vector<AABB> aabb;

	ResourceMesh* tetrahedron = nullptr;
	ResourceMesh* cube = nullptr;
	ResourceMesh* octahedron = nullptr;
	ResourceMesh* icosahedron = nullptr;
	ResourceMesh* sphere = nullptr;
	ResourceMesh* cubesphere = nullptr;
	ResourceMesh* icosphere = nullptr;
	ResourceMesh* pyramid = nullptr;
	ResourceMesh* cylinder = nullptr;
	ResourceMesh* cone = nullptr;

};

#endif 
