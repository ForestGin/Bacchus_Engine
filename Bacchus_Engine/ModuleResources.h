#ifndef __MODULE_RESOURCES_H__
#define __MODULE_RESOURCES_H__

#include "Module.h"
#include "Resource.h"


class ImporterScene;
class Resource;

class ModuleResources : public Module
{
public:

	ModuleResources(bool start_enabled = true);
	~ModuleResources();

	bool Init(json file);
	bool Start();
	bool CleanUp();
	bool LoadFromPath(const char* path);
	ImporterScene* GetImporterScene() const;

private:
	std::map<uint, Resource*>  resources;
	ImporterScene* IScene = nullptr;
	
};

#endif
