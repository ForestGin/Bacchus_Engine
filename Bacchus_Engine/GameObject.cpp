#include "Application.h"
#include "GameObject.h"

#include "ComponentMesh.h"
#include "ComponentRenderer.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

#include "Math.h"

#include "mmgr/mmgr.h"

GameObject::GameObject(const char* name)
{
	UID = App->GetRandom().Int();
	this->name = name;

	Enable();
}

GameObject::~GameObject()
{

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if (*it)
		{
			if((*it)->GetType() != Component::ComponentType::Material)
				delete(*it);

			*it = nullptr;
		}
	}
	components.clear();
}

void GameObject::Update(float dt)
{
	if (GetComponent<ComponentTransform>(Component::ComponentType::Transform)->update_transform)
		OnUpdateTransform(this);

	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		(*it)->Update(dt);
	}

}

void GameObject::RecursiveDelete(GameObject* GO, bool target)
{
	if (GO->childs.size() > 0)
	{
		for (std::vector<GameObject*>::iterator it = GO->childs.begin(); it != GO->childs.end(); ++it)
		{
			RecursiveDelete(*it, false);
		}
		GO->childs.clear();
	}

	if (target && GO->parent)
		GO->parent->RemoveChildGO(GO);

	delete GO;
}

void GameObject::OnUpdateTransform(GameObject* GO)
{
	ComponentTransform* transform = GO->GetComponent<ComponentTransform>(Component::ComponentType::Transform);
	transform->OnUpdateTransform(GO->parent->GetComponent<ComponentTransform>(Component::ComponentType::Transform)->GetGlobalTransform());

	ComponentCamera* camera = GO->GetComponent<ComponentCamera>(Component::ComponentType::Camera);

	if (camera)
		camera->OnUpdateTransform(transform->GetGlobalTransform());

	//Update all children
	if (GO->childs.size() > 0)
	{
		for (std::vector<GameObject*>::iterator it = GO->childs.begin(); it != GO->childs.end(); ++it)
		{
			OnUpdateTransform(*it);
		}
	}

	UpdateAABB();
}

void GameObject::RemoveChildGO(GameObject* GO)
{
	if (childs.size() > 0)
	{
		
		for (std::vector<GameObject*>::iterator go = childs.begin(); go != childs.end(); ++go)
		{
			if ((*go)->GetUID() == GO->GetUID())
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

		GO->parent = this;
		childs.push_back(GO);

		ComponentTransform* transform = GO->GetComponent<ComponentTransform>(Component::ComponentType::Transform);

		transform->SetGlobalTransform(this->GetComponent<ComponentTransform>(Component::ComponentType::Transform)->GetGlobalTransform());
		
		
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

Component* GameObject::AddComponent(Component::ComponentType type)
{
	static_assert(static_cast<int>(Component::ComponentType::Unknown) == 5, "Component Creation Switch needs to be updated");

	Component* new_component = nullptr;

	if (HasComponent(type) == false)
	{

		switch (type)
		{
		case Component::ComponentType::Transform:
			new_component = new ComponentTransform(this);
			break;
		case Component::ComponentType::Mesh:
			new_component = new ComponentMesh(this);
			UpdateAABB();
			break;
		case Component::ComponentType::Renderer:
			new_component = new ComponentRenderer(this);
			break;
		case Component::ComponentType::Camera:
			new_component = new ComponentCamera(this);
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

void GameObject::RemoveComponent(Component::ComponentType type)
{
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->GetType() == type)
		{
			std::vector<Component*>::iterator it = components.begin();
			it += i;

			components.erase(it);

			break;
		}
	}
}

bool GameObject::HasComponent(Component::ComponentType type) const
{
	//Search for given type of component

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->GetType() == type)
			return true;
	}

	return false;
}

std::vector<Component*>& GameObject::GetComponents()
{
	return components;
}

void GameObject::Enable()
{
	active = true;
}

void GameObject::Disable()
{
	active = false;
}

uint& GameObject::GetUID()
{
	return UID;
}

std::string GameObject::GetName() const
{
	return name;
}

bool& GameObject::GetActive()
{
	return active;
}

bool GameObject::IsEnabled() const
{
	return active;
}

void GameObject::SetName(const char* name)
{
	if (name)
		this->name = name;
}

void GameObject::SetMaterial(ComponentMaterial* material)
{
	if (material)
	{
		RemoveComponent(Component::ComponentType::Material);
		components.push_back(material);
	}
}

const AABB& GameObject::GetAABB() const
{
	return aabb;
}

const OBB& GameObject::GetOBB() const
{
	return obb;
}

void GameObject::UpdateAABB()
{
	ComponentMesh* mesh = GetComponent<ComponentMesh>(Component::ComponentType::Mesh);
	ComponentTransform* transform = GetComponent<ComponentTransform>(Component::ComponentType::Transform);
	if (mesh)
	{
		//AABB meshAABB = mesh->GetAABB();
		obb = mesh->GetAABB();
		obb.Transform(transform->GetGlobalTransform());

		aabb.SetNegativeInfinity();
		aabb.Enclose(obb);
	}
	else
	{
		aabb.SetNegativeInfinity();
		aabb.SetFromCenterAndSize(transform->GetGlobalPosition(), float3(1, 1, 1));
		obb = aabb;
	}
}
