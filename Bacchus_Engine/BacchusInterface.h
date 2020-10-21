#ifndef __BACCHUS_INTERFACE_H__
#define __BACCHUS_INTERFACE_H__

#include "Module.h"

#include <vector>

class Blockhead;
class BlockheadSettings;
class BlockheadAbout;
class BlockheadConsole;
class BlockheadInspector;
class BlockheadHierarchy;
//class BlockheadScene;
//class BlockheadToolbar;

class BacchusInterface : public Module
{
public: 
	BacchusInterface(bool start_enabled = true);
	~BacchusInterface();

	bool Init(json file);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void Draw() const;
	//void DockSpace() const;
	void RequestBrowser(const char* url) const;

	void LogFPS(float fps, float ms);

	void SaveStatus(json& file) const override;

	void LoadStatus(const json& file) override;

	void HandleInput(SDL_Event* event);

	bool IsKeyboardCaptured();

public:

	BlockheadSettings* panelSettings = nullptr;
	BlockheadAbout* panelAbout = nullptr;
	BlockheadConsole* panelConsole = nullptr;
	BlockheadInspector* panelInspector = nullptr;
	BlockheadHierarchy* panelHierarchy = nullptr;
	//BlockheadScene* panelScene = nullptr;
	//BlockheadToolbar* panelToolbar = nullptr;

private:

	bool show_demo_window = false;
	bool docking_window = true;
	bool capture_keyboard = false;
	bool capture_mouse = false;

	std::vector<Blockhead*> blockheads;

};

#endif