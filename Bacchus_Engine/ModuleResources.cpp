#include "Application.h"
#include "ModuleResources.h"
#include "ModuleTextures.h"
#include "ModuleSceneManager.h"
#include "GameObject.h"
#include "ResourceMaterial.h"


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
	LoadFromPath("Assets/BH/BakerHouse.fbx");
	
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

bool ModuleResources::LoadFromPath(const char* path)
{

	bool ret = true;

	if (path)
	{
		std::string DroppedFile_path = path;

		// If it is a 3D Model ...
		if (DroppedFile_path.find(".fbx") != std::string::npos || DroppedFile_path.find(".FBX") != std::string::npos)
		{
			ImportData data;
			ret = IScene->Import(*path, data);
		}
		// If it is a json file ...
		else if (DroppedFile_path.find(".json") != std::string::npos || DroppedFile_path.find(".JSON") != std::string::npos)
		{

		}
		// If it is an image file file ...
		else if (DroppedFile_path.find(".dds") != std::string::npos || DroppedFile_path.find(".png") != std::string::npos)
		{
			ResourceMaterial* mat = (ResourceMaterial*)App->scene_manager->GetGameObjects().at(App->scene_manager->GetSelectedGameObjects())->GetResource(Res::ResType::Material);
			if (mat)
			{
				App->scene_manager->SetTextureToSelectedGO(App->tex->CreateTextureFromFile(path, mat->Texture_width, mat->Texture_height));

			}
		}

	}
	else
		ret = false;


	return ret;
}



