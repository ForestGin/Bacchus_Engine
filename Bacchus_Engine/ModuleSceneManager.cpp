#include "ModuleSceneManager.h"
#include "OpenGL.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleRenderer3D.h"
#include "ResourceRenderer.h"
#include "ResourceMaterial.h"
#include "ImporterMaterial.h"

#include "mmgr/mmgr.h"

ModuleSceneManager::ModuleSceneManager(bool start_enabled)
{
}

ModuleSceneManager::~ModuleSceneManager()
{
}

bool ModuleSceneManager::Init(json file)
{
	return true;
}

bool ModuleSceneManager::Start()
{
	return true;
}

update_status ModuleSceneManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleSceneManager::CleanUp()
{

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

uint ModuleSceneManager::GetNumGameObjects() const
{
	return game_objects.size();
}

GameObject* ModuleSceneManager::CreateEmptyGameObject()
{
	std::string Name = "GameObject ";
	Name.append("(");
	Name.append(std::to_string(game_objects.size()));
	Name.append(")");

	GameObject* new_object = new GameObject(Name.data());
	game_objects.push_back(new_object);

	return new_object;
}

ResourceMaterial* ModuleSceneManager::CreateEmptyMaterial()
{

	ResourceMaterial* Material = new ResourceMaterial(Res::ResType::Material);

	
	Materials.push_back(Material);

	return Material;
}


void ModuleSceneManager::Draw() const
{
	
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		ResourceRenderer* Renderer = (ResourceRenderer*)game_objects[i]->GetResource(Res::ResType::Renderer);

		if (Renderer)
		{
			Renderer->Draw();
		}
		
	}

}