#include "Application.h"
#include "GameObject.h"
#include "ResourceMesh.h"
#include "ResourceRenderer.h"
#include "ResourceMaterial.h"
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
			if ((*it)->GetType() != Res::ResType::Material)
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
	position.x = Local_transform.ptr()[12];
	position.y = Local_transform.ptr()[13];
	position.z = Local_transform.ptr()[14];


	return position;
}

float3 GameObject::GetScale()
{
	float3 scale;

	scale.x = Local_transform.ptr()[0];
	scale.y = Local_transform.ptr()[5];
	scale.z = Local_transform.ptr()[10];

	return scale;
}

float3 GameObject::GetRotation()
{
	float3 rotation = float3::zero;


	return rotation;
}

float4x4 GameObject::GetLocalTransform()
{
	return Local_transform;
}

Res* GameObject::GetResource(Res::ResType type)
{
	for (std::vector<Res*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->GetType() == type)
		{
			return *it;
		}
	}

	return nullptr;
}

std::string GameObject::GetName() const
{
	return name;
}

Res* GameObject::AddResource(Res::ResType type)
{
	if (type == Res::ResType::Unknown)
		LOG("|[error]: Invalid type of Component");

	Res* new_component = nullptr;

	if (GetResource(type) == nullptr)
	{

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

	}
	else
	{
		// --- If we find a component of the same type, tell the user ---

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

void GameObject::SetPosition(float x, float y, float z)
{
	Local_transform.ptr()[12] = x;
	Local_transform.ptr()[13] = y;
	Local_transform.ptr()[14] = z;
}

void GameObject::SetRotationAxisAngle(const float3& rot_axis, float degrees_angle)
{
	degrees_angle = degrees_angle / 180.0f * (float)pi;

	float3 v = rot_axis.Normalized();

	float c = 1.0f - cosf(degrees_angle), s = sinf(degrees_angle);

	Local_transform.ptr()[0] = 1.0f + c * (v.x * v.x - 1.0f);
	Local_transform.ptr()[1] = c * v.x * v.y + v.z * s;
	Local_transform.ptr()[2] = c * v.x * v.z - v.y * s;
	Local_transform.ptr()[4] = c * v.x * v.y - v.z * s;
	Local_transform.ptr()[5] = 1.0f + c * (v.y * v.y - 1.0f);
	Local_transform.ptr()[6] = c * v.y * v.z + v.x * s;
	Local_transform.ptr()[8] = c * v.x * v.z + v.y * s;
	Local_transform.ptr()[9] = c * v.y * v.z - v.x * s;
	Local_transform.ptr()[10] = 1.0f + c * (v.z * v.z - 1.0f);
}

void GameObject::Scale(float x, float y, float z)
{
	Local_transform.ptr()[0] = x;
	Local_transform.ptr()[5] = y;
	Local_transform.ptr()[10] = z;
}

void GameObject::SetLocalTransform(float4x4 new_transform)
{
	Local_transform = new_transform;
}

void GameObject::SetMaterial(ResourceMaterial* material)
{
	if (material)
		components.push_back(material);
}

void GameObject::SetName(const char* name)
{
	if (name)
		this->name = name;
}