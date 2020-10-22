#ifndef __MODULE_RESOURCES_H__
#define __MODULE_RESOURCES_H__

#include "Module.h"
#include "Math.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <vector>

#include <vector>

class ResourceMesh;
class aiScene;

#define NORMAL_LENGTH 1

class ModuleResources : public Module
{
public:

	ModuleResources(bool start_enabled = true);
	~ModuleResources();

	bool Init(/*json file*/);
	bool Start();
	void Draw() const;
	bool CleanUp();
	bool LoadFBX(const char* path);
	uint GetNumMeshes() const;
	void GetTextureIDFromSceneMaterial(const aiScene& scene, uint& texture_ID, std::string& directory);

private:
	void DrawMesh(const ResourceMesh* mesh) const;
	void DrawNormals(const ResourceMesh* mesh) const;

private:

	std::vector<ResourceMesh*> meshes;

};

#endif
