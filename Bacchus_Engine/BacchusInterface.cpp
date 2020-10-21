#include "Application.h"
#include "Globals.h"
#include "BacchusInterface.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "Blockhead.h"
#include "BlockheadAbout.h"
#include "BlockheadSettings.h"
//#include "BlockheadConsole.h"
//#include "BlockheadInspector.h"
//#include "BlockheadHierarchy.h"
//#include "BlockheadScene.h"
//#include "BlockheadToolbar.h"

#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"
#include "imgui/ImGuizmo/ImGuizmo.h"

#include "OpenGL.h"


BacchusInterface::BacchusInterface(bool start_enabled) : Module(start_enabled) 
{
	name = "GUI";
}

BacchusInterface::~BacchusInterface() {}

bool BacchusInterface::Init(json file)
{
	blockheadAbout = new BlockheadAbout("About");
	blockheads.push_back(blockheadAbout);

	blockheadSettings = new BlockheadSettings("Settings");
	blockheads.push_back(blockheadSettings);

	/*blockheadConsole = new BlockheadConsole("Console");
	blockheads.push_back(blockheadConsole);

	blockheadInspector = new BlockheadInspector("Inspector");
	blockheads.push_back(blockheadInspector);

	blockheadHierarchy = new BlockheadHierarchy("Hierarchy");
	blockheads.push_back(blockheadHierarchy);*/

	/*blockheadScene = new PanelScene("Scene");
	blockheads.push_back(blockheadScene);

	blockheadToolbar = new PanelToolbar("Toolbar");
	blockheads.push_back(blockheadToolbar);*/

	LoadStatus(file);

	return true;
}

bool BacchusInterface::Start()
{
	LOG("Loading Engine UI...");
	bool ret = true;

	// Initialize imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		// Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls ImGuiWindowFlags_MenuBar
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			// Enable Window Docking (Under Active Development)
	/*io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;*/

	io.IniFilename = "imgui.ini";

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);

	/*ret = LoadEditorConfig();*/

	return ret;
}

update_status BacchusInterface::PreUpdate(float dt)
{
	//Starting Frame

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	//ImGuizmo::BeginFrame();

	return UPDATE_CONTINUE;
}

// Update
update_status BacchusInterface::Update(float dt)
{
	ImGuiWindowFlags docking_window_flags = 0;
	ImGuiID dockspaceID = 0;

	if (docking_window == true)
	{
		docking_window_flags |= ImGuiWindowFlags_NoMove;
		docking_window_flags |= ImGuiWindowFlags_NoBackground;
		docking_window_flags |= ImGuiWindowFlags_NoTitleBar;
		docking_window_flags |= ImGuiWindowFlags_NoDocking;
		docking_window_flags |= ImGuiWindowFlags_NoInputs;

		if (ImGui::Begin("Master Window", &docking_window, docking_window_flags))
		{
			
		}

		if (docking_window)
		{
			// Declare Central dockspace
			dockspaceID = ImGui::GetID("HUB_DockSpace");
			ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode/*|ImGuiDockNodeFlags_NoResize*/);
		}
		ImGui::End();
	}

	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit"))
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			/*if (ImGui::MenuItem("Console"))
			{
				blockheadConsole->OnOff();
			}*/

			if (ImGui::MenuItem("Configuration"))
			{
				blockheadSettings->OnOff();
			}

			/*if (ImGui::MenuItem("Inspector"))
			{
				blockheadInspector->OnOff();
			}*/

			/*if (ImGui::MenuItem("Hierarchy"))
			{
				blockheadHierarchy->OnOff();
			}*/

			/*if (ImGui::MenuItem("Scene"))
			{
				blockheadScene->OnOff();
			}*/

			/*if (ImGui::MenuItem("Toolbar"))
			{
				blockheadToolbar->OnOff();
			}*/

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("ImGui Demo"))
			{
				show_demo_window = !show_demo_window;
			}

			if (ImGui::MenuItem("Documentation"))
			{
				RequestBrowser("https://github.com/ForestGin/Bacchus_Engine");
			}

			if (ImGui::MenuItem("Download latest"))
			{
				RequestBrowser("https://github.com/ForestGin/Bacchus_Engine/releases");
			}

			if (ImGui::MenuItem("Report a bug"))
			{
				RequestBrowser("https://github.com/ForestGin/Bacchus_Engine/issues");
			}

			if (ImGui::MenuItem("About"))
			{
				blockheadAbout->OnOff();
			}

			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	return UPDATE_CONTINUE;
}

update_status BacchusInterface::PostUpdate(float dt)
{

	// Iterate panels and draw
	for (uint i = 0; i < blockheads.size(); ++i)
	{
		if (blockheads[i]->IsEnabled())
			blockheads[i]->Draw();
	}

	// End dock space
	//if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
	//ImGui::End();

	return UPDATE_CONTINUE;
}

bool BacchusInterface::CleanUp()
{
	bool ret = true;

	// Iterate panels and delete 
	for (uint i = 0; i < blockheads.size(); ++i)
	{
		delete blockheads[i];
		blockheads[i] = nullptr;
	}

	LOG("Unloading Bacchus...");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return ret;
}

void BacchusInterface::Draw() const
{
	// Render
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

}

//void BacchusInterface::DockSpace() const
//{
//	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
//	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
//	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
//
//	static bool p_open = true;
//	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
//
//	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
//	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
//	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
//}

void BacchusInterface::RequestBrowser(const char* url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

//void BacchusInterface::SaveStatus(json& file) const
//{
//
//
//
//};
//
//void BacchusInterface::LoadStatus(const json& file)
//{
//	
//};

void BacchusInterface::HandleInput(SDL_Event* event)
{
	ImGui_ImplSDL2_ProcessEvent(event);
}

bool BacchusInterface::IsKeyboardCaptured()
{
	return capture_keyboard;
}