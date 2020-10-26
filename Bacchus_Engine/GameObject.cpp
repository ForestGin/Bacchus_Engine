#include "Application.h"
#include "GameObject.h"
#include "ResourceMesh.h"
#include "ResourceRenderer.h"

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

float3 GameObject::GetPosition()
{
	float3 position;
	Local_transform.TransformPos(position);
	Local_transform.TranslatePart();

	return position;
}

float4x4 GameObject::GetLocalTransform()
{
	return Local_transform;
}

Res* GameObject::GetResource(Res::ResType type)
{
	for (std::list<Res*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->GetType() == type)
		{
			return *it;
		}
	}

	return nullptr;
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
	case Res::ResType::Renderer:
		new_component = new ResourceRenderer(this);
		break;

	}

	if (new_component)
		components.push_back(new_component);

	return new_component;
}

void GameObject::SetPosition(float x, float y, float z)
{
	Local_transform.Translate(x, y, z);
}

void GameObject::SetRotationAxisAngle(const float3& rot_axis, float degrees_angle)
{
	Local_transform.RotateAxisAngle(rot_axis, degrees_angle);
}

void GameObject::Scale(float x, float y, float z)
{
	Local_transform.Scale(x, y, z);
}

void GameObject::SetLocalTransform(float4x4 new_transform)
{
	Local_transform = new_transform;
}