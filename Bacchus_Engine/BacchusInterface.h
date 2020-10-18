#ifndef __BACCHUS_INTERFACE_H__
#define __BACCHUS_INTERFACE_H__

#include "Module.h"
#include "Globals.h"
#include "imgui/imgui.h"

#include <string>

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

	void ConsoleText(std::string console_Text);

	ImGuiTextBuffer console_text;

public:
	int window_with = 0;
	int window_height = 0;
	ImGuiID dockspaceID = 0;
	bool docking_window = true;
	bool show_demo_window = false;
	bool config_window = false;
	bool about_window = false;
	bool console_window = false;

	void WindowConfig();
	void Hardware();
	void FPSGraph();

	
};

#endif