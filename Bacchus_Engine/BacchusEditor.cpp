#include "Application.h"
#include "Globals.h"
#include "BacchusEditor.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "Blockhead.h"
#include "BlockheadAbout.h"
#include "BlockheadSettings.h"
#include "BlockheadConsole.h"
//#include "BlockheadInspector.h"
//#include "BlockheadHierarchy.h"
//#include "BlockheadScene.h"
//#include "BlockheadToolbar.h"

#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"

#include "OpenGL.h"


BacchusEditor::BacchusEditor(bool start_enabled) : Module(start_enabled) 
{
	name = "GUI";
}

BacchusEditor::~BacchusEditor() {}

bool BacchusEditor::Init(/*json file*/)
{
	blockheadAbout = new BlockheadAbout("About");
	blockheads.push_back(blockheadAbout);

	blockheadSettings = new BlockheadSettings("Settings");
	blockheads.push_back(blockheadSettings);

	blockheadConsole = new BlockheadConsole("Console");
	blockheads.push_back(blockheadConsole);
	/*
	blockheadInspector = new BlockheadInspector("Inspector");
	blockheads.push_back(blockheadInspector);

	blockheadHierarchy = new BlockheadHierarchy("Hierarchy");
	blockheads.push_back(blockheadHierarchy);*/

	/*blockheadScene = new PanelScene("Scene");
	blockheads.push_back(blockheadScene);

	blockheadToolbar = new PanelToolbar("Toolbar");
	blockheads.push_back(blockheadToolbar);*/

	//LoadStatus(file);

	return true;
}

bool BacchusEditor::Start()
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

update_status BacchusEditor::PreUpdate(float dt)
{
	//Starting Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	
	//Starting Docking Space
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		DockingSpace();
	}

	return UPDATE_CONTINUE;
}

// Update
update_status BacchusEditor::Update(float dt)
{
	//ImGuiWindowFlags docking_window_flags = 0;
	//ImGuiID dockspaceID = 0;

	//if (docking_window == true)
	//{
	//	docking_window_flags |= ImGuiWindowFlags_NoMove;
	//	docking_window_flags |= ImGuiWindowFlags_NoBackground;
	//	docking_window_flags |= ImGuiWindowFlags_NoTitleBar;
	//	docking_window_flags |= ImGuiWindowFlags_NoDocking;
	//	docking_window_flags |= ImGuiWindowFlags_NoInputs;

	//	if (ImGui::Begin("Master Window", &docking_window, docking_window_flags))
	//	{
	//		
	//	}

	//	if (docking_window)
	//	{
	//		// Declare Central dockspace
	//		dockspaceID = ImGui::GetID("HUB_DockSpace");
	//		ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode/*|ImGuiDockNodeFlags_NoResize*/);
	//	}
	//	ImGui::End();
	//}

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
			if (ImGui::MenuItem("Console"))
			{
				blockheadConsole->OnOff();
			}

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

update_status BacchusEditor::PostUpdate(float dt)
{

	//Iterate panels and draw
	for (uint i = 0; i < blockheads.size(); ++i)
	{
		if (blockheads[i]->IsEnabled())
			blockheads[i]->Draw();
	}

	//Ending docking space
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
	ImGui::End();

	return UPDATE_CONTINUE;
}

bool BacchusEditor::CleanUp()
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

void BacchusEditor::Draw() const
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

void BacchusEditor::DockingSpace() const
{
	// Adapt to main Window
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	// Docking window flags
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_MenuBar; 
	window_flags |= ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	window_flags |= ImGuiWindowFlags_NoNavFocus;
	window_flags |= ImGuiWindowFlags_NoBackground;

	//Docking Window Style
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));

	static bool p_open = true;
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("MyDockingspace");
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
}

void BacchusEditor::RequestBrowser(const char* url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

//void BacchusEditor::SaveStatus(json& file) const
//{
//
//
//
//};
//
//void BacchusEditor::LoadStatus(const json& file)
//{
//	
//};

void BacchusEditor::HandleInput(SDL_Event* event)
{
	ImGui_ImplSDL2_ProcessEvent(event);
}

bool BacchusEditor::IsKeyboardCaptured()
{
	return capture_keyboard;
}

void BacchusEditor::LogFPS(float fps, float ms)
{
	if (blockheadSettings != nullptr)
		blockheadSettings->AddFPS(fps, ms);
}