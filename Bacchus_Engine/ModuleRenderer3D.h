#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
//#include "JSONLoader.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(json file);
	update_status PreUpdate(float dt);
	//update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void UpdateGLCapabilities();

	void OnResize(int width, int height);

	// Sets 
	bool SetVSync(bool vsync);

	// Gets
	bool GetVSync() const;

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	// Flags
	bool vsync = true;
	bool depth_test = true;
	bool cull_face = false;
	bool lighting = true;
	bool color_material = false;
	bool wireframe = false;
};