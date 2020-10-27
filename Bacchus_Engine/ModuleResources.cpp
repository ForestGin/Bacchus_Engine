#include "Application.h"
#include "ModuleResources.h"
#include "OpenGL.h"
#include "GameObject.h"
#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ResourceRenderer.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")


#include "mmgr/mmgr.h"

void MyAssimpCallback(const char* msg, char* userData)
{
	LOG("[Assimp]: %s", msg);
}

ModuleResources::ModuleResources(bool start_enabled) : Module(start_enabled)
{
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
	//Detach assimp log stream
	aiDetachAllLogStreams();


	//Release all buffer data and own stored data
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i])
		delete game_objects[i];
	}

	for (uint i = 0; i < Materials.size(); ++i)
	{
		if (Materials[i])
			delete Materials[i];
	}

	return true;
}

bool ModuleResources::LoadFBX(const char* path)
{
	//Import scene from path
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);


	if (scene != nullptr && scene->HasMeshes())
	{
		ResourceMaterial* Material = new ResourceMaterial(Res::ResType::Material);
		Material->CreateFromScene(scene, path);

		if (Material)
			Materials.push_back(Material);

		//Use scene->mNumMeshes to iterate on scene->mMeshes array

		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			GameObject* new_object = new GameObject("GO");
			game_objects.push_back(new_object);

			// Get Scene mesh number i
			aiMesh* mesh = scene->mMeshes[i];

			if (mesh)
			{
				ResourceMesh* new_mesh = (ResourceMesh*)new_object->AddResource(Res::ResType::Mesh);

				new_mesh->ImportMesh(mesh);

				ResourceRenderer* Renderer = (ResourceRenderer*)new_object->AddResource(Res::ResType::Renderer);

			}
			if (Material)
			{
				new_object->SetMaterial(Material);
			}
		}

		// Free scene
		aiReleaseImport(scene);


	}
	else
		LOG("|[error]: Error loading FBX %s", path);


	return true;
}


void ModuleResources::Draw() const
{
		

	for (uint i = 0; i < game_objects.size(); ++i)
	{
		ResourceRenderer* Renderer = (ResourceRenderer*)game_objects[i]->GetResource(Res::ResType::Renderer);

		if (Renderer)
		{
			Renderer->Draw();
		}
		else
		{
			LOG("|[error]: Could not find Renderer component in current game object");
		}
	}
}

uint ModuleResources::GetNumGameObjects() const
{
	return game_objects.size();
}

