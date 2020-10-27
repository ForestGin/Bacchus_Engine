#include "Application.h"
#include "ModuleResources.h"


#include "Importer.h"
#include "ImporterScene.h"
#include "Assimp/include/cimport.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "mmgr/mmgr.h"

void MyAssimpCallback(const char* msg, char* userData)
{
	LOG("[Assimp]: %s", msg);
}

ModuleResources::ModuleResources(bool start_enabled) : Module(start_enabled)
{
	IScene = new ImporterScene;
}

ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Init(json file)
{
	//Stream LOG messages to MyAssimpCallback, that sends them to console
	struct aiLogStream stream;
	stream.callback = MyAssimpCallback;
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleResources::Start()
{
	LoadFBX("Assets/BH/BakerHouse.fbx");
	
	return true;
}


bool ModuleResources::CleanUp()
{
	
	aiDetachAllLogStreams();

	if (IScene)
	{
		delete IScene;
		IScene = nullptr;
	}

	return true;
}

bool ModuleResources::LoadFBX(const char* path)
{

	bool ret = true;

	if (path)
	{
		ImportData data;
		ret = IScene->Import(*path, data);

	}
	else
		ret = false;


	return ret;
}



