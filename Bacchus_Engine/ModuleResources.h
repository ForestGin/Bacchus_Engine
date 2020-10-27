#ifndef __MODULE_RESOURCES_H__
#define __MODULE_RESOURCES_H__

#include "Module.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <vector>

#include <vector>


class GameObject;
class ResourceMaterial;


class ModuleResources : public Module
{
public:

	ModuleResources(bool start_enabled = true);
	~ModuleResources();

	bool Init(json file);
	bool Start();
	void Draw() const;
	bool CleanUp();
	bool LoadFBX(const char* path);
	uint GetNumGameObjects() const;
	

private:

	std::vector<GameObject*> game_objects;
	std::vector<ResourceMaterial*> Materials;
};

#endif
