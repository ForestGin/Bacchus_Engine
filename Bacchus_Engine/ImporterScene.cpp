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
#include "FileSystem.h"

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

	

	std::string rootnodename = File_path;
	uint count = rootnodename.find_last_of("/");
	rootnodename = rootnodename.substr(count + 1, rootnodename.size());

	uint countdot = rootnodename.find_last_of(".");
	rootnodename = rootnodename.substr(0, countdot);
	std::string relative_path;

	App->fs->DuplicateFile(File_path, ASSETS_FOLDER, relative_path);

	
	char* buffer;
	uint size = App->fs->Load(relative_path.data(), &buffer);
	

	// Import scene from path
	const aiScene* scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

	GameObject* rootnode = App->scene_manager->CreateEmptyGameObject();
	rootnode->SetName(rootnodename.data());

	if (scene != nullptr && scene->HasMeshes())
	{

		ResourceMaterial* Material = App->scene_manager->CreateEmptyMaterial();

		ImportMaterialData MData;
		MData.scene = scene;
		MData.new_material = Material;
		IMaterial->Import(File_path, MData);

		std::vector<GameObject*> scene_gos;
		scene_gos.push_back(rootnode);
		
		LoadNodes(scene->mRootNode, rootnode, scene, Material, scene_gos);

		SaveSceneToFile(scene_gos, rootnodename);
		aiReleaseImport(scene);
		

	}
	else
		LOG("|[error]: Error loading FBX %s", &File_path);

	return true;
}

bool ImporterScene::Load(const char* exported_file) const
{
	return true;
}

void ImporterScene::SaveSceneToFile(std::vector<GameObject*>& scene_gos, std::string& scene_name) const
{
	json file;

	for (int i = 0; i < scene_gos.size(); ++i)
	{
		file[scene_gos[i]->GetName()];
		file[scene_gos[i]->GetName()]["UID"] = std::to_string(scene_gos[i]->GetUID());
		file[scene_gos[i]->GetName()]["Parent"] = std::to_string(scene_gos[i]->parent->GetUID());
	}
	std::string data;
	data = App->GetJLoader()->Serialize(file);

	std::string path = LIBRARY_FOLDER;
	path.append(scene_name);
	path.append(".scene");

	char* buffer = (char*)data.data();
	uint size = data.length();

	App->fs->Save(path.data(), buffer, size);
}

void ImporterScene::LoadNodes(const aiNode* node, GameObject* parent, const aiScene* scene, ResourceMaterial* Material, std::vector<GameObject*>& scene_gos) const
{
	GameObject* nodeGo = nullptr;

	if (node != scene->mRootNode && node->mNumMeshes > 1)
	{
		nodeGo = App->scene_manager->CreateEmptyGameObject();
		nodeGo->SetName(node->mName.C_Str());
		parent->AddChildGO(nodeGo);
		scene_gos.push_back(nodeGo);
	}
	else
		nodeGo = parent;

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		LoadNodes(node->mChildren[i], nodeGo, scene, Material, scene_gos);
	}

	for (int j = 0; j < node->mNumMeshes; ++j)
	{
		GameObject* new_object = App->scene_manager->CreateEmptyGameObject();
		new_object->SetName(node->mName.C_Str());
		nodeGo->AddChildGO(new_object);
		scene_gos.push_back(new_object);

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