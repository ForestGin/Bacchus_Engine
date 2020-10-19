#ifndef _APPLICATION_
#define _APPLICATION_

#include "Globals.h"
#include <list>
#include <string>
#include <vector>
#include "Timer.h"
#include "PerfTimer.h"
#include "MathGeoLib/include/Algorithm/Random/LCG.h"

//#include "JSON/parson.h"

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;
class BacchusInterface;
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
	ModuleCamera3D* camera = nullptr;

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
	FileSystem* fs = nullptr;
	ModuleResources* resources = nullptr;

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

	std::list<Module*> list_modules;

	Timer				ms_timer;
	Timer				fps_timer;
	float				dt = 0;
	std::string			appName;
	std::string			orgName;
	std::string			configpath;
	JSONLoader			JLoader;

	Uint32				frames;
	int					fps_counter;
	int					last_fps;
	uint				capped_ms;
	uint				last_frame_ms;

	//JSON_Value* pilar;

	LCG* RandomNumber = nullptr;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void RequestBrowser(const char* url) const;
	//void JasonReading();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void SaveAllStatus();
	void LoadAllStatus(json& file);
};

extern Application* App;

#endif //_APPLICATION_