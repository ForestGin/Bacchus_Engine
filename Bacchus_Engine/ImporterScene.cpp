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
#include "ImporterMesh.h"
#include "ImporterMaterial.h"
#include "ModuleCamera3D.h"

#include "mmgr/mmgr.h"

ImporterScene::ImporterScene()
{
	IMesh = new ImporterMesh;
	IMaterial = new ImporterMaterial;
}

ImporterScene::~ImporterScene()
{
	delete IMesh;
	delete IMaterial;
}

bool ImporterScene::Import(const char* File_path, const ImportData& IData) const
{
	ImportSceneData Sdata = (ImportSceneData&)IData;

	const aiScene* scene = aiImportFile(File_path, aiProcessPreset_TargetRealtime_MaxQuality);

	GameObject* rootnode = App->scene_manager->CreateEmptyGameObject();

	std::string rootnodename = File_path;
	uint count = rootnodename.find_last_of("/");
	rootnodename = rootnodename.substr(count + 1, rootnodename.size());

	uint countdot = rootnodename.find_last_of(".");
	rootnodename = rootnodename.substr(0, countdot);
	rootnode->SetName(rootnodename.data());

	if (scene != nullptr && scene->HasMeshes())
	{

		ResourceMaterial* Material = App->scene_manager->CreateEmptyMaterial();

		ImportMaterialData MData;
		MData.scene = scene;
		MData.new_material = Material;
		IMaterial->Import(File_path, MData);

		LoadNodes(scene->mRootNode, rootnode, scene, Material);
		aiReleaseImport(scene);
		

	}
	else
		LOG("|[error]: Error loading FBX %s", &File_path);

	return true;
}

void ImporterScene::LoadNodes(const aiNode* node, GameObject* parent, const aiScene* scene, ResourceMaterial* Material) const
{
	GameObject* nodeGo = nullptr;

	if (node != scene->mRootNode && node->mNumMeshes > 1)
	{
		nodeGo = App->scene_manager->CreateEmptyGameObject();
		nodeGo->SetName(node->mName.C_Str());
		nodeGo->SetParent(parent);
	}
	else
		nodeGo = parent;

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		LoadNodes(node->mChildren[i], nodeGo, scene, Material);
	}

	for (int j = 0; j < node->mNumMeshes; ++j)
	{
		GameObject* new_object = App->scene_manager->CreateEmptyGameObject();
		new_object->SetName(node->mName.C_Str());
		new_object->SetParent(nodeGo);

		uint mesh_index = node->mMeshes[j];
		aiMesh* mesh = scene->mMeshes[mesh_index];

		if (mesh)
		{
			ResourceMesh* new_mesh = (ResourceMesh*)new_object->AddResource(Res::ResType::Mesh);

			if (new_mesh)
			{
				ImportMeshData Mdata;
				Mdata.mesh = mesh;
				Mdata.new_mesh = new_mesh;
				IMesh->Import(Mdata);

				ResourceRenderer* Renderer = (ResourceRenderer*)new_object->AddResource(Res::ResType::Renderer);

				if (Material)
				{
					new_object->SetMaterial(Material);
				}

			}

			App->camera->FrameObject(new_object);

		}
	}
}