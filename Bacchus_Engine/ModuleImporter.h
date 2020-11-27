#ifndef __MODULE_IMPORTER_H__
#define __MODULE_IMPORTER_H__

#include "Module.h"



class ImporterScene;
class Blockhead;
class BlockheadImporter;

class ModuleImporter : public Module
{
public:

	ModuleImporter(bool start_enabled = true);
	~ModuleImporter();

	bool Init(json file);
	bool Start();
	bool CleanUp();

	bool LoadFromPath(const char* path);

	ImporterScene* GetImporterScene() const;

private:


	ImporterScene* IScene = nullptr;
	
};

#endif
