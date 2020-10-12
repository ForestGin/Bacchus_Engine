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
#include "JSON/parson.h"

#include <list>
#include <vector>

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleSceneIntro* scene_intro;
	BacchusInterface* bacchusinterface;
	ModuleCamera3D* camera;

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

	JSON_Value* pilar;

private:

	Timer	ms_timer;
	std::list<Module*> list_modules;


public:

	Timer T;

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void RequestBrowser(const char* url) const;
	void JasonReading();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;

#endif //_APPLICATION_