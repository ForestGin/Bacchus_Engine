#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Math.h"

class GameObject;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3&Position, const vec3&Reference, bool RotateAroundReference = false);
	void LookAt(const vec3&Spot);
	void Move(const vec3&Movement);
	void LookAround();
	void Pan(float speed);
	void Zoom(float speed);
	void Orbit();
	

	float* GetViewMatrix();
	void FrameObject(GameObject* GO);

private:

	void CalculateViewMatrix();

public:
	float speed = 0.0f;
	vec3 coordzero = { 0,0,0 };

	vec3 X, Y, Z, Position, Reference;


private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};