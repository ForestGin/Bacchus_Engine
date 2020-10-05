#pragma once
#include "Module.h"
#include "Globals.h"

#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_opengl3.h"

class BacchusInterface : public Module
{
public: 
	BacchusInterface(Application* app, bool start_enabled = true);
	~BacchusInterface();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	bool show_demo_window = false;
};