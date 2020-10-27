#ifndef __MODULE_RESOURCES_H__
#define __MODULE_RESOURCES_H__

#include "Module.h"


class ImporterScene;


class ModuleResources : public Module
{
public:

	ModuleResources(bool start_enabled = true);
	~ModuleResources();

	bool Init(json file);
	bool Start();
	bool CleanUp();
	bool LoadFBX(const char* path);

private:

	ImporterScene* IScene = nullptr;
	
};

#endif
