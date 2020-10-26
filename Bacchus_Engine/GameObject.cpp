#include "Application.h"
#include "GameObject.h"
#include "ResourceMesh.h"

#include "mmgr/mmgr.h"

GameObject::GameObject(const char* name)
{
	UID = App->GetRandom().Int();
	this->name = name;
}

GameObject::~GameObject()
{
	for (std::list<Res*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if (*it)
		{
			delete(*it);
			*it = nullptr;
		}
	}
}

uint GameObject::GetUID() const
{
	return UID;
}

Res* GameObject::AddResource(Res::ResType type)
{
	if (type == Res::ResType::Unknown)
		LOG("|[error]: Invalid type of Component");

	Res* new_component = nullptr;

	switch (type)
	{
	case Res::ResType::Mesh:
		new_component = new ResourceMesh(this);
		break;

	}

	if (new_component)
		components.push_back(new_component);

	return new_component;
}