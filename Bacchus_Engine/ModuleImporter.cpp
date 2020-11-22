#include "Application.h"
#include "ModuleImporter.h"
#include "FileSystem.h"

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

ModuleImporter::ModuleImporter(bool start_enabled) : Module(start_enabled)
{
	IScene = new ImporterScene;
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init(json file)
{
	//Stream LOG messages to MyAssimpCallback, that sends them to console
	struct aiLogStream stream;
	stream.callback = MyAssimpCallback;
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleImporter::Start()
{
	LoadFromPath("Assets/BakerHouse.fbx");
	
	return true;
}

update_status ModuleImporter::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{
	
	aiDetachAllLogStreams();

	if (IScene)
	{
		delete IScene;
		IScene = nullptr;
	}

	return true;
}

bool ModuleImporter::LoadFromPath(const char* path)
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

Resource* ModuleImporter::GetResource(uint UID)
{
	Resource* ret = nullptr;

	// --- If resource is loaded into memory, return pointer to it, else load it ---

	std::map<uint, Resource*>::iterator it = resources.find(UID);

	if (it != resources.end())
		ret = it->second;
	else
	{
		// --- If resource is not in memory, search in library ---
		std::map<uint, ResourceMeta>::iterator it = loaded_resources.find(UID);

		if (it != loaded_resources.end())
		{
			switch (it->second.type)
			{
			case Resource::ResourceType::MESH:

				break;

			case Resource::ResourceType::TEXTURE:

				break;

			case Resource::ResourceType::MATERIAL:

				break;

			}
		}

		if (ret)
		{
			ret->og_file = it->second.original_file;
			ret->name = it->second.resource_name;
			LOG("Loaded Resource: %s", ret->name);
		}
	}

	return ret;
}

Resource::ResourceType ModuleImporter::GetResourceTypeFromPath(const char* path)
{
	std::string extension = "";
	App->fs->SplitFilePath(path, nullptr, nullptr, &extension);

	Resource::ResourceType type = Resource::ResourceType::UNKNOWN;

	if (extension == ".mesh")
		type = Resource::ResourceType::MESH;
	else if (extension == ".dds")
		type = Resource::ResourceType::TEXTURE;

	return type;
}



