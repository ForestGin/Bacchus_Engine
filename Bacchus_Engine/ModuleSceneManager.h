#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Module.h"
#include <vector>

class GameObject;
class ResourceMaterial;
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
	ResourceMaterial* CreateEmptyMaterial();
	uint GetNumGameObjects() const;
	void Draw() const;

	GameObject* CreateCube(float sizeX, float sizeY, float sizeZ, bool checkers = false);
	GameObject* CreateSphere(float Radius = 1.0f, int sectors = 36, int stackCount = 18, bool smooth = false);
	GameObject* CreateCubeSphere(float Radius = 1.0f, int sub = 3, bool smooth = false);
	GameObject* CreateCylinder(float baseRadius = 1.0f, float topRadius = 1.0f, float height = 1.0f, int sectorCount = 36, int stackCount = 1, bool smooth = false);

	void CreateGrid() const;

	uint GetSelectedGameObjects();
	std::vector<GameObject*>& GetGameObjects();
	void SetSelectedGameObject(uint index);
	void SetTextureToSelectedGO(uint id);

private:


public:
	ResourceMaterial* CheckersMaterial = nullptr;
	ResourceMaterial* DefaultMaterial = nullptr;

private:

	std::vector<GameObject*> game_objects;
	std::vector<ResourceMaterial*> Materials;
	uint SelectedGameObject = 0;
};

#endif 
