#include "ResourceTransform.h"
#include "GameObject.h"

ResourceTransform::ResourceTransform(GameObject* ContainerGO) : Res(ContainerGO, Res::ResType::Transform)
{
}

ResourceTransform::~ResourceTransform()
{
}

float3 ResourceTransform::GetPosition() const
{
	return position;
}

float3 ResourceTransform::GetScale() const
{
	return scale;
}

float3 ResourceTransform::GetRotation() const
{
	return rotation_euler;
}

float4x4 ResourceTransform::GetLocalTransform() const
{
	return Local_transform;
}

float4x4 ResourceTransform::GetGlobalTransform() const
{
	return Global_transform;
}

void ResourceTransform::SetPosition(float x, float y, float z)
{
	position = float3(x, y, z);

	UpdateLocalTransform();
}

void ResourceTransform::SetRotation(float3 euler_angles)
{
	
	float3 difference = (euler_angles - rotation_euler) * DEGTORAD;
	Quat quatrot = Quat::FromEulerXYZ(difference.x, difference.y, difference.z);

	
	rotation = rotation * quatrot;
	rotation_euler = euler_angles;

	
	UpdateLocalTransform();
}

void ResourceTransform::Scale(float x, float y, float z)
{
	if (x > 0.0f && y > 00.0f && z > 00.0f)
	{
		scale = float3(x, y, z);
		UpdateLocalTransform();
	}
}

void ResourceTransform::SetGlobalTransform(float4x4 new_transform)
{
	Local_transform = Local_transform = new_transform.Inverted() * Global_transform;
	Global_transform = new_transform;
	OnUpdateTransform(Global_transform);
}

void ResourceTransform::UpdateLocalTransform()
{
	Local_transform = float4x4::FromTRS(position, rotation, scale);
	update_transform = true;
}

void ResourceTransform::OnUpdateTransform(const float4x4& ParentGlobal)
{
	Global_transform = ParentGlobal * Local_transform;
	UpdateTRS();

	update_transform = false;
}

void ResourceTransform::UpdateTRS()
{
	Local_transform.Decompose(position, rotation, scale);
	rotation_euler = rotation.ToEulerXYZ() * RADTODEG;
}
