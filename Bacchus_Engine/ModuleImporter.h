#ifndef __MODULE_IMPORTER_H__
#define __MODULE_IMPORTER_H__

#include "Module.h"


class ImporterScene;

class ModuleImporter : public Module
{
public:

	ModuleImporter(bool start_enabled = true);
	~ModuleImporter();

	bool Init(json file);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	bool LoadFromPath(const char* path);

	Resource* GetResource(uint UID);
	Resource::ResourceType GetResourceTypeFromPath(const char* path);

private:


	ImporterScene* IScene = nullptr;
	
};

#endif
