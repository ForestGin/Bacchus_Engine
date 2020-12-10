#pragma once
#include "Module.h"
#include "Globals.h"
#include "Math.h"

class GameObject;
class ComponentCamera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const float3& Position, const float3& Reference, bool RotateAroundReference = false);
	void LookAt(const float3& Spot);
	void FrameObject(GameObject* GO);

private:
	void LookAround(float dt);
	void Pan(float speed);
	void Zoom(float speed);
	void Orbit();


public:
	float speed = 0.0f;

	float3 reference = { 0.0f,0.0f,0.0f };
	ComponentCamera* camera = nullptr;

	
};