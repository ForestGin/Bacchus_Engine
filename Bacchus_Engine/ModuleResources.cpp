#include "Application.h"
#include "ModuleResources.h"
#include "Resource.h"

#include "ModuleTextures.h"
#include "ModuleSceneManager.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "FileSystem.h"

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
	LoadFromPath("Assets/BakerHouse.fbx");
	
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
		App->fs->NormalizePath(DroppedFile_path);

		// If it is a 3D Model ...
		if (DroppedFile_path.find(".fbx") != std::string::npos || DroppedFile_path.find(".FBX") != std::string::npos)
		{
			ImportData data;
			ret = IScene->Import(DroppedFile_path.data(), data);
		}
		// If it is an image file file ...
		else if (DroppedFile_path.find(".dds") != std::string::npos || DroppedFile_path.find(".png") != std::string::npos)
		{
			ComponentMaterial* mat = App->scene_manager->GetSelectedGameObjects()->GetComponent<ComponentMaterial>(Component::ComponentType::Material);
			
			if (mat->Texture_path == "Default")
			{
				/*App->scene_manager->GetGameObjects().at(App->scene_manager->GetSelectedGameObjects())->RemoveComponent(Component::ComponentType::Material);*/

				mat = App->scene_manager->CreateEmptyMaterial();

				App->scene_manager->GetSelectedGameObjects()->SetMaterial(mat);
			}

			mat->Texture_path = DroppedFile_path.data();

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



