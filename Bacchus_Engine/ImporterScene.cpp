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

	// Remove the directory and file type, obtaining the file name
	uint count = rootnodename.find_last_of("/");
	rootnodename = rootnodename.substr(count + 1, rootnodename.size());

	uint countdot = rootnodename.find_last_of(".");
	rootnodename = rootnodename.substr(0, countdot);

	// Duplicate File into Assets folder, save relative path 
	std::string relative_path;

	App->fs->DuplicateFile(File_path, ASSETS_FOLDER, relative_path);

	char* buffer;
	uint size = App->fs->Load(relative_path.data(), &buffer);


	//Import scene from path
	const aiScene* scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

	GameObject* rootnode = App->scene_manager->CreateEmptyGameObject();

	//Set root node name as file name with no extension
	rootnode->SetName(rootnodename.data());

	if (scene != nullptr && scene->HasMeshes())
	{

		//Create new Component Material to store scene's, meshes will use this for now since we do not want to create a material for every mesh if not needed
		ResourceMaterial* Material = App->scene_manager->CreateEmptyMaterial();

		//Import Material Data (fill Material)
		ImportMaterialData MData;
		MData.scene = scene;
		MData.new_material = Material;
		IMaterial->Import(File_path, MData);

		std::vector<GameObject*> scene_gos;
		scene_gos.push_back(rootnode);

		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		LoadNodes(scene->mRootNode, rootnode, scene, Material, scene_gos);

		SaveSceneToFile(scene_gos, rootnodename);

		// Free scene
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
		file[scene_gos[i]->GetName()]["Components"];
		for (int j = 0; j < scene_gos[i]->GetResources().size(); ++j)
		{
			file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetResources()[j]->GetType())];
		}
	}
	std::string data;
	data = App->GetJLoader()->Serialize(file);

	std::string path = LIBRARY_FOLDER;
	path.append(scene_name);
	path.append(".model");

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
		//Create Game Object
		GameObject* new_object = App->scene_manager->CreateEmptyGameObject();
		new_object->SetName(node->mName.C_Str());
		nodeGo->AddChildGO(new_object);
		scene_gos.push_back(new_object);

		//Get Scene mesh number i
		uint mesh_index = node->mMeshes[j];
		aiMesh* mesh = scene->mMeshes[mesh_index];

		if (mesh)
		{
			// Create new Component Mesh to store current scene mesh data
			ResourceMesh* new_mesh = (ResourceMesh*)new_object->AddResource(Res::ResType::Mesh);

			if (new_mesh)
			{
				//Import mesh data (fill new_mesh)
				ImportMeshData Mdata;
				Mdata.mesh = mesh;
				Mdata.new_mesh = new_mesh;
				IMesh->Import(Mdata);

				//Create new Component Renderer to draw mesh
				ResourceRenderer* Renderer = (ResourceRenderer*)new_object->AddResource(Res::ResType::Renderer);

				if (Material)
				{
					//Set Object's Material 
					new_object->SetMaterial(Material);
				}

			}

			//When the mesh is loaded, frame it with the camera
			App->camera->FrameObject(new_object);

		}
	}
}