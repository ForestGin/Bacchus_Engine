#ifndef __RESOURCE_TRANSFORM_H__
#define __RESOURCE_TRANSFORM_H__

#include "Res.h"
#include "Math.h"

class ResourceTransform : public Res
{
public:

	ALIGN_CLASS_TO_16

	ResourceTransform(GameObject* ContainerGO);
	virtual ~ResourceTransform();

	
	float3			GetPosition() const;
	float3			GetScale() const;
	float3			GetRotation() const;
	float4x4        GetLocalTransform() const;

	
	void			SetPosition(float x, float y, float z);
	void			SetRotation(float3 euler_angles);
	void			Scale(float x, float y, float z);
	void			SetLocalTransform(float4x4 new_transform);

private:
	void UpdateLocalTransform();
	void UpdateTRS();

private:
	float4x4 Local_transform = math::float4x4::identity;

	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3	rotation_euler = float3::zero;
	float3 scale = float3::one;
};

#endif
