#ifndef _APPLICATION_
#define _APPLICATION_

#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "BacchusInterface.h"
#include "JSONLoader.h"
#include "FileSystem.h"

#include <list>
#include <vector>

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleSceneIntro* scene_intro = nullptr;
	BacchusInterface* bacchusinterface = nullptr;
	ModuleCamera3D* camera = nullptr;
	FileSystem* fs = nullptr;

	std::vector<float> fps_log;
	std::vector<float> ms_log;
	float	dt;
	float fps;
	int cap;
	int	 capped_ms;
	Timer last_sec_frame_time;
	Uint32 last_sec_frame_count = 0;
	Uint64 frame_count = 0;
	Timer frame_time;
	PerfTimer ptimer;


private:

	Timer	ms_timer;
	std::list<Module*> list_modules;

	std::string			appName;
	std::string			orgName;
	std::string			configpath;
	JSONLoader			JLoader;

public:

	Timer T;

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void RequestBrowser(const char* url) const;
	
	void SetAppName(const char* name);
	void SetOrganizationName(const char* name);
	const char* GetAppName() const;
	const char* GetOrganizationName() const;
	/*json GetDefaultConfig() const;*/

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void SaveAllStatus();
	void LoadAllStatus(json& file);
};

extern Application* App;

#endif //_APPLICATION_