#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#include <array>

using namespace std;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:
	uint cube_id;//array 

	uint indices_id;
	uint buffIndicesID;
};