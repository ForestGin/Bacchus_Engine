#ifndef __BACCHUS_INTERFACE_H__
#define __BACCHUS_INTERFACE_H__

#include "Module.h"

#include <vector>

class Blockhead;
class BlockheadAbout;
class BlockheadSettings;
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

	BlockheadSettings*	blockheadSettings = nullptr;
	BlockheadAbout*		blockheadAbout = nullptr;
	BlockheadConsole*	blockheadConsole = nullptr;
	BlockheadInspector* blockheadInspector = nullptr;
	BlockheadHierarchy* blockheadHierarchy = nullptr;
	//BlockheadScene*	blockheadScene = nullptr;
	//BlockheadToolbar* blockheadToolbar = nullptr;

private:

	bool show_demo_window = false;
	bool docking_window = true;
	bool capture_keyboard = false;
	bool capture_mouse = false;

	std::vector<Blockhead*> blockheads;

};

#endif