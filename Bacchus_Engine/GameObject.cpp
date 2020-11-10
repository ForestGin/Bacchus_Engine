#include "Application.h"
#include "GameObject.h"
#include "ResourceMesh.h"
#include "ResourceRenderer.h"
#include "ResourceMaterial.h"
#include "ResourceTransform.h"
#include "Math.h"

#include "mmgr/mmgr.h"

GameObject::GameObject(const char* name)
{
	UID = App->GetRandom().Int();
	this->name = name;
}

GameObject::~GameObject()
{

	for (std::vector<Res*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if (*it)
		{
			if((*it)->GetType() != Res::ResType::Material)
				delete(*it);

			*it = nullptr;
		}
	}
}

void GameObject::RecursiveDelete(GameObject* GO)
{
	if (GO->childs.size() > 0)
	{
		for (std::vector<GameObject*>::iterator it = GO->childs.begin(); it != GO->childs.end(); ++it)
		{
			RecursiveDelete(*it);
		}
		GO->childs.clear();
	}

	delete GO;
}

void GameObject::RemoveChildGO(GameObject* GO)
{
	if (childs.size() > 0)
	{
		std::vector<GameObject*>::iterator go = childs.begin();

		for (std::vector<GameObject*>::iterator go = childs.begin(); go != childs.end(); ++go)
		{
			if (*go == GO)
			{
				childs.erase(go);
				break;
			}
		}
	}
}

void GameObject::AddChildGO(GameObject* GO)
{
	if (!FindChildGO(GO))
	{

		if (GO->parent)
			GO->parent->RemoveChildGO(GO);

		childs.push_back(GO);
		GO->parent = this;
	}
}

bool GameObject::FindChildGO(GameObject* GO)
{
	bool ret = false;

	if (childs.size() > 0)
	{
		std::vector<GameObject*>::iterator go = childs.begin();

		for (std::vector<GameObject*>::iterator go = childs.begin(); go != childs.end(); ++go)
		{
			if (*go == GO)
				ret = true;
		}
	}

	return ret;
}

Res* GameObject::AddResource(Res::ResType type)
{
	static_assert(static_cast<int>(Res::ResType::Unknown) == 4, "Component Creation Switch needs to be updated");

	Res* new_component = nullptr;

	if (HasResource(type) == false)
	{

		switch (type)
		{
		case Res::ResType::Transform:
			new_component = new ResourceTransform(this);
			break;
		case Res::ResType::Mesh:
			new_component = new ResourceMesh(this);
			break;
		case Res::ResType::Renderer:
			new_component = new ResourceRenderer(this);
			break;
		}

		if (new_component)
			components.push_back(new_component);

	}
	else
	{
		
		LOG("|[error]: The current Game Object already has a component of the type given");

	}

	return new_component;
}

void GameObject::RemoveResource(Res::ResType type)
{
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->GetType() == type)
		{
			std::vector<Res*>::iterator it = components.begin();
			it += i;

			components.erase(it);

			break;
		}
	}
}

bool GameObject::HasResource(Res::ResType type) const
{
	// --- Search for given type of component ---

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->GetType() == type)
			return true;
	}

	return false;
}

uint GameObject::GetUID() const
{
	return UID;
}

std::string GameObject::GetName() const
{
	return name;
}

void GameObject::SetName(const char* name)
{
	if (name)
		this->name = name;
}

void GameObject::SetMaterial(ResourceMaterial* material)
{
	if (material)
		components.push_back(material);
}