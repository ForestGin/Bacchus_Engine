#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "Component.h"
#include "Math.h"

class ComponentCamera : public Component
{
public:

	ComponentCamera(GameObject* ContainerGO);
	virtual ~ComponentCamera();

	float GetNearPlane() const;
	float GetFarPlane() const;
	float GetFOV() const;
	float GetAspectRatio() const;
	float4x4 GetOpenGLViewMatrix();
	float4x4 GetOpenGLProjectionMatrix();

	void SetNearPlane(float distance);
	void SetFarPlane(float distance);
	void SetFOV(float fov);
	void SetAspectRatio(float ar);

	void Look(const float3& position);
	void OnUpdateTransform(const float4x4& global);

public:
	Frustum		frustum;
	bool		update_projection = true;
	bool		culling = false;
};

#endif 
