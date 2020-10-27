#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Module.h"
#include <vector>

class GameObject;
class ResourceMaterial;
struct aiScene;

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
	ResourceMaterial* CreateMaterialFromScene(const aiScene& scene, const char& file_path);
	uint GetNumGameObjects() const;
	void Draw() const;

private:

	std::vector<GameObject*> game_objects;
	std::vector<ResourceMaterial*> Materials;
};

#endif 
