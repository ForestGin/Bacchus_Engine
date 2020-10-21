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

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;

class BacchusInterface;
class BacchusHardware;
class FileSystem;
class ModuleResources;

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleSceneIntro* scene_intro = nullptr;

	BacchusInterface* bacchusinterface = nullptr;
	BacchusHardware* hardware = nullptr;
	ModuleCamera3D* camera = nullptr;
	FileSystem* fs = nullptr;
	ModuleResources* resources = nullptr;

public:
	// Getts
	uint GetMaxFramerate() const;
	const char* GetAppName() const;
	const char* GetOrganizationName() const;
	//json GetDefaultConfig() const;
	std::vector<std::string>& GetLogs();
	LCG& GetRandom();

	// Sets
	void SetMaxFramerate(uint maxFramerate);
	void SetAppName(const char* name);
	void SetOrganizationName(const char* name);

	void Log(const char* entry);
	void ClearLogsFromConsole();

private:

	std::list<Module*> list_modules;

	Timer				ms_timer;
	Timer				fps_timer;
	float				dt = 0;

	Uint32				frames;
	int					fps_counter;
	int					last_fps;
	uint				capped_ms;
	uint					last_frame_ms;

	JSONLoader			JLoader;
	std::string			appName;
	std::string			orgName;
	std::string			configpath;
	std::string			log;
	std::vector<std::string> logs;

	LCG* RandomNumber = nullptr;

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