#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Module.h"
#include <vector>
#include "Math.h"
#include "Color.h"

class GameObject;
class ComponentMaterial;
struct aiScene;
struct ImportMaterialData;

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

	GameObject* CreateSphere(float Radius = 1.0f, int sectors = 36, int stackCount = 18, bool smooth = false);
	GameObject* CreateCubeSphere(float Radius = 1.0f, int sub = 3, bool smooth = false);
	GameObject* CreateIcoSphere(float radius = 1.0f, int subdivision = 1, bool smooth = false);
	GameObject* CreateCylinder(float baseRadius = 1.0f, float topRadius = 1.0f, float height = 1.0f, int sectorCount = 36, int stackCount = 1, bool smooth = false);

	void CreateGrid() const;

	GameObject* GetSelectedGameObjects() const;
	void SetSelectedGameObject(GameObject* go);
	void SetTextureToSelectedGO(uint id);
	void DestroyGameObject(GameObject* go);

private:
	void LoadPrimitiveArrays(GameObject& new_object,
	uint vertices_size, const float* vertices,
	uint indices_size, const uint* indices,
	uint normals_size, const float* normals,
	uint texCoords_size, const float* texCoords) const;
		
	void GatherGameObjects(std::vector<GameObject*>& scene_gos, GameObject* go);
	GameObject* CreateRootGameObject();
	void DrawRecursive(GameObject* go);

public:
	ComponentMaterial* CheckersMaterial = nullptr;
	ComponentMaterial* DefaultMaterial = nullptr;

private:

	uint go_count = 0;
	std::vector<ComponentMaterial*> Materials;
	GameObject* SelectedGameObject = nullptr;
	GameObject* root = nullptr;
};

#endif 
