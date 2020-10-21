#ifndef __MODULE_RESOURCES_H__
#define __MODULE_RESOURCES_H__

#include "Module.h"
#include "Math.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <vector>

#include <vector>

class ResourceMesh;

class ModuleResources : public Module
{
public:

	ModuleResources(bool start_enabled = true);
	~ModuleResources();

	bool Init(/*json file*/);
	bool Start();
	//update_status PreUpdate(float dt);
	//update_status Update(float dt);
	//update_status PostUpdate(float dt);
	void Draw();
	bool CleanUp();
	bool LoadFBX(const char* path);
	uint GetNumMeshes() const;

private:

	std::vector<ResourceMesh*> meshes;
	uint HouseTexID = 0;

};

#endif
