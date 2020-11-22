#ifndef _APPLICATION_
#define _APPLICATION_

#include "Globals.h"
#include <list>
#include <string>
#include <vector>
#include "Timer.h"
#include "PerfTimer.h"
#include "MathGeoLib/include/Algorithm/Random/LCG.h"
#include "JSONLoader.h"

enum AppState
{
	PLAY = 0,
	TO_PLAY,

	EDITOR,
	TO_EDITOR,

	PAUSE,
	TO_PAUSE,

	STEP,
};

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleRenderer3D;
class ModuleCamera3D;

class BacchusEditor;
class BacchusHardware;
class FileSystem;
class ModuleImporter;
class ModuleTextures;
class ModuleSceneManager;
class ModuleTimeManager;

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;

	BacchusEditor* bacchuseditor = nullptr;
	BacchusHardware* hardware = nullptr;
	ModuleCamera3D* camera = nullptr;
	FileSystem* fs = nullptr;
	ModuleImporter* importer = nullptr;
	ModuleTextures* tex = nullptr;
	ModuleSceneManager* scene_manager = nullptr;
	ModuleTimeManager* time = nullptr;

public:
	// Getts
	const char* GetAppName() const;
	const char* GetOrganizationName() const;
	json GetDefaultConfig() const;
	std::vector<std::string>& GetLogs();
	LCG& GetRandom();
	JSONLoader* GetJLoader();
	AppState& GetAppState();

	// Sets
	void SetAppName(const char* name);
	void SetOrganizationName(const char* name);

	void Log(const char* entry);
	void ClearLogsFromConsole();

private:

	std::list<Module*> list_modules;

	JSONLoader			JLoader;
	std::string			appName;
	std::string			orgName;
	std::string			configpath;
	std::string			log;
	std::vector<std::string> logs;

	LCG* RandomNumber = nullptr;

	AppState EngineState = AppState::EDITOR;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void RequestBrowser(const char* url) const;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void SaveAllStatus();
	void LoadAllStatus(json& file);
};

extern Application* App;

#endif //_APPLICATION_