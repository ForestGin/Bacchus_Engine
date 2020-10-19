#pragma once
#include "Module.h"
#include "Globals.h"

#include "glew/include/GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include "MathGeoLib\include\MathGeoLib.h"

#include "SDL/include/SDL_opengl.h"

#include <string>
#include "JSONLoader.h"

class JSONLoader;

class BacchusInterface : public Module
{
public: 
	BacchusInterface(Application* app, bool start_enabled = true);
	~BacchusInterface();

	bool Init(json file);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void ConsoleText(std::string console_Text);
	void DockSpace() const;
	ImGuiTextBuffer console_text;

public:
	bool show_demo_window = false;
	bool config_window = false;
	bool about_window = false;
	bool console_window = false;

	void WindowConfig();
	void Hardware();
	void FPSGraph();
	//bool LoadEditorConfig() const;
	void SaveStatus(json file) const override;

	void LoadStatus(const json file) override;

	

	
};