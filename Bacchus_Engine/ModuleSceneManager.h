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
	GameObject* CreateSphere(float Radius, int slices, int slacks, bool checkers = false);
	void CreateGrid() const;

	uint GetSelectedGameObjects();
	std::vector<GameObject*>& GetGameObjects();
	void SetSelectedGameObject(uint index);
	void SetTextureToSelectedGO(uint id);



public:
	ResourceMaterial* CheckersMaterial = nullptr;

private:

	std::vector<GameObject*> game_objects;
	std::vector<ResourceMaterial*> Materials;
	uint SelectedGameObject = 0;
};

#endif 
