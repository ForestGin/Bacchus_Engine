#include "Application.h"
#include "Globals.h"
#include "BacchusEditor.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleSceneManager.h"

#include "Blockhead.h"
#include "BlockheadAbout.h"
#include "BlockheadSettings.h"
#include "BlockheadConsole.h"
#include "BlockheadHierarchy.h"
#include "BlockheadInspector.h"


#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"

#include "OpenGL.h"

#include "mmgr/mmgr.h"


BacchusEditor::BacchusEditor(bool start_enabled) : Module(start_enabled) 
{
	name = "GUI";
}

BacchusEditor::~BacchusEditor() {}

bool BacchusEditor::Init(json file)
{
	blockheadAbout = new BlockheadAbout("About");
	blockheads.push_back(blockheadAbout);

	blockheadSettings = new BlockheadSettings("Settings");
	blockheads.push_back(blockheadSettings);

	blockheadConsole = new BlockheadConsole("Console");
	blockheads.push_back(blockheadConsole);

	blockheadHierarchy = new BlockheadHierarchy("Hierarchy");
	blockheads.push_back(blockheadHierarchy); 

	blockheadInspector = new BlockheadInspector("Inspector");
	blockheads.push_back(blockheadInspector);

	LoadStatus(file);

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

	//Font
	io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/QuattrocentoSans/QuattrocentoSans-Bold.ttf", 15);

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	SetDarkThemeColors();

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

	//Mouse & Keyboard Checkers
	ImGuiIO& io = ImGui::GetIO();
	capture_keyboard = io.WantCaptureKeyboard;
	capture_mouse = io.WantCaptureMouse;
	
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

		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::BeginMenu("3D Object"))
			{
				if (ImGui::MenuItem("Tetrahedron"))
				{
					App->scene_manager->CreateCylinder(1, 0, 1, 3, 1, false);
				}

				if (ImGui::MenuItem("Cube"))
				{
					App->scene_manager->CreateCubeSphere(1, 0, false);
				}

				if (ImGui::MenuItem("Octahedron"))
				{
					App->scene_manager->CreateSphere(1, 4, 2, false);
				}

				/*if (ImGui::MenuItem("Icosahedron"))
				{
					App->scene_manager->CreateIcoSphere(1, 0, false);
				}*/

				if (ImGui::BeginMenu("Spheres"))
				{
					if (ImGui::MenuItem("Sphere"))
					{
						App->scene_manager->CreateSphere(1, 36, 18, false);
					}

					if (ImGui::MenuItem("Cubesphere"))
					{
						App->scene_manager->CreateCubeSphere(1, 3, false);
					}

					/*if (ImGui::MenuItem("Icosphere"))
					{
						App->scene_manager->CreateCubeSphere(1, 3, false);
					}*/
					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Pyramid"))
				{
					App->scene_manager->CreateCylinder(1, 0, 1, 4, 1, false);
				}
				if (ImGui::MenuItem("Cylinder"))
				{
					App->scene_manager->CreateCylinder(1, 1, 1, 36, 4, false);
				}
				if (ImGui::MenuItem("Cone"))
				{
					App->scene_manager->CreateCylinder(1, 0, 1, 36, 4, false);
				}

				ImGui::EndMenu();
			}

			/*if (ImGui::BeginMenu("4D Object"))
			{

				if (ImGui::MenuItem("Tesseract"))
				{
					
				}

				ImGui::EndMenu();
			}*/
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

			if (ImGui::MenuItem("Hierarchy"))
			{
				blockheadHierarchy->OnOff();
			}

			if (ImGui::MenuItem("Inspector"))
			{
				blockheadInspector->OnOff();
			}

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

	
	for (uint i = 0; i < blockheads.size(); ++i)
	{
		delete blockheads[i];
		blockheads[i] = nullptr;
	}

	blockheadSettings = nullptr;
	blockheadAbout = nullptr;
	blockheadConsole = nullptr;
	blockheadHierarchy = nullptr;
	blockheadInspector = nullptr;

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

void BacchusEditor::SetDarkThemeColors()
{
	auto& colors = ImGui::GetStyle().Colors;

	//Window Background
	colors[ImGuiCol_WindowBg] =				ImVec4{ 0.f, 0.f, 0.f, 0.8f };
	
	//Headers
	colors[ImGuiCol_Header] =				ImVec4{ 0.420f, 0.023f, 0.125f, 0.8f };
	colors[ImGuiCol_HeaderHovered] =		ImVec4{ 0.420f, 0.023f, 0.125f, 0.7f };
	colors[ImGuiCol_HeaderActive] =			ImVec4{ 1.f, 1.f, 1.f, 0.7f };

	//Buttons
	colors[ImGuiCol_Button] =				ImVec4{ 0.420f, 0.023f, 0.125f, 0.8f };
	colors[ImGuiCol_ButtonHovered] =		ImVec4{ 0.420f, 0.023f, 0.125f, 0.7f };
	colors[ImGuiCol_ButtonActive] =			ImVec4{ 1.f, 1.f, 1.f, 0.7f };

	//Frame Bg
	colors[ImGuiCol_FrameBg] =				ImVec4{ 1.f, 1.f, 1.f, 0.2f };
	colors[ImGuiCol_FrameBgHovered] =		ImVec4{ 1.f, 1.f, 1.f, 0.3f };
	colors[ImGuiCol_FrameBgActive] =		ImVec4{ 1.f, 1.f, 1.f, 0.4f };

	//Tabs
	colors[ImGuiCol_Tab] =					ImVec4{ 0.f, 0.f, 0.f, 0.5f };
	colors[ImGuiCol_TabHovered] =			ImVec4{ 1.f, 1.f, 1.f, 0.4f };
	colors[ImGuiCol_TabActive] =			ImVec4{ 0.520f, 0.023f, 0.225f, 0.8f };
	colors[ImGuiCol_TabUnfocused] =			ImVec4{ 0.f, 0.f, 0.f, 0.4f };
	colors[ImGuiCol_TabUnfocusedActive] =	ImVec4{ 1.f, 1.f, 1.f, 0.2f };

	//Titles
	colors[ImGuiCol_TitleBg] =				ImVec4{ 1.f, 1.f, 1.f, 0.2f };
	colors[ImGuiCol_TitleBgActive] =		ImVec4{ 0.420f, 0.023f, 0.125f, 0.8f };
	colors[ImGuiCol_TitleBgCollapsed] =		ImVec4{ 0.420f, 0.023f, 0.125f, 0.8f };

	//Docking
	colors[ImGuiCol_DockingEmptyBg] = ImVec4{ 1.f, 1.f, 1.f, 0.6f };
	colors[ImGuiCol_DockingPreview] = ImVec4{ 1.f, 1.f, 1.f, 0.4f };
}

void BacchusEditor::SaveStatus(json& file) const
{
	for (uint i = 0; i < blockheads.size(); ++i)
		file["GUI"][blockheads[i]->GetName()] = blockheads[i]->IsEnabled();


};

void BacchusEditor::LoadStatus(const json& file)
{
	for (uint i = 0; i < blockheads.size(); ++i)
	{
		if (file["GUI"].find(blockheads[i]->GetName()) != file["GUI"].end())
			blockheads[i]->SetOnOff(file["GUI"][blockheads[i]->GetName()]);
		else
			LOG("|[error]: Could not find sub-node %s in GUI JSON Node, please check JSON EditorConfig", blockheads[i]->GetName());
	}
};

void BacchusEditor::HandleInput(SDL_Event* event)
{
	ImGui_ImplSDL2_ProcessEvent(event);
}

bool BacchusEditor::IsKeyboardCaptured() const
{
	return capture_keyboard;
}

bool BacchusEditor::IsMouseCaptured() const
{
	return capture_mouse;
}

void BacchusEditor::LogFPS(float fps, float ms)
{
	if (blockheadSettings != nullptr)
		blockheadSettings->AddFPS(fps, ms);
}