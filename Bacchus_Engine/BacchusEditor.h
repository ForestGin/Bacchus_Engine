#ifndef __BACCHUS_EDITOR_H__
#define __BACCHUS_EDITOR_H__

#include "Module.h"

#include <vector>

class Blockhead;
class BlockheadAbout;
class BlockheadSettings;
class BlockheadConsole;
class BlockheadInspector;
class BlockheadHierarchy;
class BlockheadToolbar;
class BlockheadImporter;

class BacchusEditor : public Module
{
public: 
	BacchusEditor(bool start_enabled = true);
	~BacchusEditor();

	bool Init(json file);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void Draw() const;
	void DockingSpace() const;
	void RequestBrowser(const char* url) const;

	void SetDarkThemeColors();
	//void SetLightThemeColors();

	void LogFPS(float fps, float ms);

	void SaveStatus(json& file) const override;

	void LoadStatus(const json& file) override;

	void HandleInput(SDL_Event* event);

	bool IsKeyboardCaptured() const;
	bool IsMouseCaptured() const;

public:

	BlockheadSettings*	blockheadSettings = nullptr;
	BlockheadAbout*		blockheadAbout = nullptr;
	BlockheadConsole*	blockheadConsole = nullptr;
	BlockheadInspector* blockheadInspector = nullptr;
	BlockheadHierarchy* blockheadHierarchy = nullptr;
	BlockheadToolbar* blockheadToolbar = nullptr;
	BlockheadImporter* blockheadImporter = nullptr;

private:

	bool show_demo_window = false;
	bool docking_window = true;
	bool capture_keyboard = false;
	bool capture_mouse = false;

	std::vector<Blockhead*> blockheads;

};

#endif