#include "ImporterScene.h"
#include "Application.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "ResourceMaterial.h"
#include "ResourceMesh.h"
#include "ResourceRenderer.h"
#include "ModuleSceneManager.h"
#include "GameObject.h"

ImporterScene::ImporterScene()
{
}

ImporterScene::~ImporterScene()
{
}

bool ImporterScene::Import(const char& File_path, const ImportData& IData) const
{
	ImportSceneData Sdata = (ImportSceneData&)IData;

	const aiScene* scene = aiImportFile(&File_path, aiProcessPreset_TargetRealtime_MaxQuality);


	if (scene != nullptr && scene->HasMeshes())
	{
		ResourceMaterial* Material = App->scene_manager->CreateMaterialFromScene(*scene, File_path);

		
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			
			GameObject* new_object = App->scene_manager->CreateEmptyGameObject();

			aiMesh* mesh = scene->mMeshes[i];

			if (mesh)
			{
				ResourceMesh* new_mesh = (ResourceMesh*)new_object->AddResource(Res::ResType::Mesh);

				new_mesh->ImportMesh(mesh);

				ResourceRenderer* Renderer = (ResourceRenderer*)new_object->AddResource(Res::ResType::Renderer);

				if (Material)
				{
					new_object->SetMaterial(Material);
				}

			}

		}

		aiReleaseImport(scene);

	}
	else
		LOG("|[error]: Error loading FBX %s", File_path);


	return true;
}