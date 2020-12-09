#include "Application.h"
#include "ModuleImporter.h"
#include "Resource.h"

#include "ModuleTextures.h"
#include "ModuleSceneManager.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "FileSystem.h"
#include "BacchusEditor.h"
#include "BlockheadImporter.h"
#include "ModuleResources.h"

#include "ResourceMaterial.h"
#include "ResourceTexture.h"

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
	/*LoadFromPath("Assets/BakerHouse.fbx");*/

	return true;
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

ImporterScene* ModuleImporter::GetImporterScene() const
{
	return IScene;
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
			//PopUp when fbx is dropped
			App->bacchuseditor->blockheadImporter->OnOff();

			ImportData data;
			ret = IScene->Import(DroppedFile_path.data(), data);
		}
		// If it is a json file ...
		else if (DroppedFile_path.find(".json") != std::string::npos || DroppedFile_path.find(".JSON") != std::string::npos)
		{

		}
		// If it is an image file file ...
		else if (DroppedFile_path.find(".dds") != std::string::npos || DroppedFile_path.find(".png") != std::string::npos)
		{
			// MYTODO: We are not checking if the texture was already loaded, duplicating data

			//Get Selected Game Object's Material
			GameObject* Selected = App->scene_manager->GetSelectedGameObjects();
			ComponentMaterial* mat = nullptr;

			if (Selected)
			{
				mat = Selected->GetComponent<ComponentMaterial>(Component::ComponentType::Material);

				if (mat)
				{
					if (mat->resource_material->resource_diffuse->Texture_path == "Default")
					{
						mat = App->scene_manager->CreateEmptyMaterial();
						mat->resource_material = (ResourceMaterial*)App->res->CreateResource(Resource::ResourceType::MATERIAL);
					}

					std::string destination = ASSETS_FOLDER;
					std::string filename;
					App->fs->SplitFilePath(path, nullptr, &filename, nullptr);
					destination.append(filename);

					ResourceTexture* tex = nullptr;
					
					//Look for meta, if found load image from library
					if (App->res->IsFileImported(destination.data()))
					{
						uint uid = App->res->GetUIDFromMeta(destination.data());

						std::string lib_Tex = TEXTURES_FOLDER;
						lib_Tex.append(std::to_string(uid));
						lib_Tex.append(".dds");

						tex = (ResourceTexture*)App->res->GetResource(lib_Tex.data());

					}
					else
						tex = (ResourceTexture*)App->res->GetResource(destination.data());

					if (tex)
					{
						if (mat->resource_material->resource_diffuse)
						{
							mat->resource_material->resource_diffuse->instances--;
							// MYTODO: UNLOAD RESOURCE FROM MEMORY
						}

						mat->resource_material->resource_diffuse = tex;
					}
					else
					{
						mat->resource_material->resource_diffuse = (ResourceTexture*)App->res->CreateResource(Resource::ResourceType::TEXTURE);
						App->scene_manager->SetTextureToSelectedGO(App->tex->CreateTextureFromFile(destination.data(), mat->resource_material->resource_diffuse->Texture_width, mat->resource_material->resource_diffuse->Texture_height, mat->resource_material->resource_diffuse->GetUID()));

						std::string lib_Tex = TEXTURES_FOLDER;
						lib_Tex.append(std::to_string(mat->resource_material->resource_diffuse->GetUID()));
						lib_Tex.append(".dds");

						mat->resource_material->resource_diffuse->Texture_path = lib_Tex.data();
						mat->resource_material->resource_diffuse->SetOriginalFilename(lib_Tex.data());
					}

					App->scene_manager->GetSelectedGameObjects()->SetMaterial(mat);
				}
			}
			else
				ret = false;
		}
	}
	else
		ret = false;

	return ret;
}




