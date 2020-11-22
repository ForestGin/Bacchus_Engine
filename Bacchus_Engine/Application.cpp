#include "Application.h"
#include "Globals.h"

#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "BacchusEditor.h"
#include "ModuleSceneManager.h"
#include "BacchusHardware.h"
#include "ModuleRenderer3D.h"
#include "FileSystem.h"
#include "ModuleImporter.h"
#include "ModuleTextures.h"

#include "mmgr/mmgr.h"

Application::Application()
{
	frames = 0;
	last_frame_ms = -1;
	last_fps = -1;
	capped_ms = 1000 / 60; // Get Display RR!!
	fps_counter = 0;
	appName = "";
	configpath = "Settings/EditorConfig.json";
	log = "Application Logs:";
	RandomNumber = new math::LCG();

	//.-.-.-.-.-.-
	window = new ModuleWindow(true);
	input = new ModuleInput(true);
	renderer3D = new ModuleRenderer3D(true);
	camera = new ModuleCamera3D(true);
	bacchuseditor = new BacchusEditor(true);
	hardware = new BacchusHardware(true);
	fs = new FileSystem(true, ASSETS_FOLDER);
	importer = new ModuleImporter(true);
	tex = new ModuleTextures(true);
	scene_manager = new ModuleSceneManager(true);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(hardware);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(fs);
	AddModule(importer);
	AddModule(tex);

	//Scene
	AddModule(scene_manager);

	//Engine UI
	AddModule(bacchuseditor);

	// Renderer last!
	AddModule(renderer3D);

}

Application::~Application()
{
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend())
	{
		delete(*item);
		++item;
	}
	

	if (RandomNumber)
	{
		delete RandomNumber;
		RandomNumber = nullptr;
	}
}

bool Application::Init()
{
	bool ret = true;

	//Load App data from JSON files
	json config = JLoader.Load(configpath.data());

	if (config.is_null())
	{
		//call defaultconfig
		config = GetDefaultConfig();
	}

	// Reading App Name/ Org Name from json file
	std::string tmp = config["Application"]["Title"];
	appName = tmp;

	std::string tmp2 = config["Application"]["Organization"];
	orgName = tmp2;

	// Call Init() in all modules
	std::list<Module*>::const_iterator item= list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init(config);
		++item;
	}

	// After all Init calls we call Start() in all modules
	//LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		++item;
	}
	
	ms_timer.Start();

	SetMaxFramerate(App->window->GetDisplayRefreshRate());

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{

	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	// Recap on framecount and fps
	++frames;
	++fps_counter;

	if (fps_timer.Read() >= 1000)
	{
		last_fps = fps_counter;
		fps_counter = 0;
		fps_timer.Start();
	}

	last_frame_ms = ms_timer.Read();

	// cap fps
	if (capped_ms > 0 && (last_frame_ms < capped_ms))
		SDL_Delay(capped_ms - last_frame_ms);

	//Send data to GUI- PanelSettings Historiograms
	App->bacchuseditor->LogFPS((float)last_fps, (float)last_frame_ms);

}

void Application::SaveAllStatus()
{
	json config = GetDefaultConfig();

	std::string tmp = appName;
	config["Application"]["Title"] = tmp;
	std::string tmp2 = orgName;
	config["Application"]["Organization"] = tmp2;

	// --- Call Save of all modules ---

	std::list<Module*>::const_iterator item = list_modules.begin();

	while (item != list_modules.end())
	{
		(*item)->SaveStatus(config);
		item++;
	}

	JLoader.Save(configpath.data(), config);
}

void Application::LoadAllStatus(json& file)
{
	//Reading App name from json file
	std::string tmp = file["Application"]["Title"];
	appName = tmp;

	std::string tmp2 = file["Application"]["Organization"];
	orgName = tmp2;

	//Call Load of all modules

	json config = JLoader.Load(configpath.data());

	std::list<Module*>::const_iterator item = list_modules.begin();

	while (item != list_modules.end())
	{
		(*item)->LoadStatus(config);
		item++;
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		++item;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		++item;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		++item;
	}

	FinishUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		++item;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::SetMaxFramerate(uint maxFramerate)
{
	if (maxFramerate > 0)
		capped_ms = 1000 / maxFramerate;
	else
		capped_ms = 0;
}

uint Application::GetMaxFramerate() const
{
	if (capped_ms > 0)
		return (uint)((1.0f / (float)capped_ms) * 1000.0f);
	else
		return 0;
}



void Application::SetAppName(const char* name)
{
	appName.assign(name);
	App->window->SetTitle(appName.data());
}

void Application::SetOrganizationName(const char* name)
{
	orgName = name;
}

const char* Application::GetAppName() const
{
	return appName.data();
}

const char* Application::GetOrganizationName() const
{
	return orgName.data();
}

void Application::Log(const char* entry)
{
	//Append all logs to a string so we can print them on console 
	log.append(entry);

	std::string to_add = entry;
	logs.push_back(to_add);
}

void Application::ClearLogsFromConsole()
{
	logs.erase(logs.begin(), logs.end());
	logs.clear();
}

std::vector<std::string>& Application::GetLogs()
{
	return logs;
}

json Application::GetDefaultConfig() const
{
	// --- Create Config with default values ---
	json config = {
		{"Application", {

		}},

		{"GUI", {

		}},

		{"Window", {
			{"width", 1024},
			{"height", 720},
			{"fullscreen", false},
			{"resizable", true},
			{"borderless", false},
			{"fullscreenDesktop", false}
		}},

		{"Input", {

		}},

		{"Renderer3D", {
			{"VSync", true}
		}},
	};

	return config;
}

void Application::RequestBrowser(const char* url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

LCG& Application::GetRandom()
{
	return *RandomNumber;
}

JSONLoader* Application::GetJLoader()
{
	return &JLoader;
}

