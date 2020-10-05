#include "Application.h"
#include "Globals.h"
#include "BacchusInterface.h"

#include "imgui/imgui_internal.h"
#include <gl/GL.h>

BacchusInterface::BacchusInterface(Application* app, bool start_enabled) : Module(app, start_enabled) {}

BacchusInterface::~BacchusInterface() {}

bool BacchusInterface::Start()
{
	LOG("Loading Engine UI...");
	bool ret = true;

	// Initialize imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls ImGuiWindowFlags_MenuBar

	io.DisplaySize.x = 1280.0f;
	io.DisplaySize.y = 720.0f;
	io.IniFilename = "imgui.ini";

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);

	

	return ret;
}

bool BacchusInterface::CleanUp()
{
	LOG("Unloading Bacchus...");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();


	return true;
}

update_status BacchusInterface::PreUpdate(float dt)
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

// Update
update_status BacchusInterface::Update(float dt)
{
	//bool show_demo_window = false;
	//ImGui::ShowDemoWindow(&show_demo_window);

	//ImGuiIO& io = ImGui::GetIO();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
				return update_status::UPDATE_STOP;
			if (ImGui::MenuItem("Demo"))
				show_demo_window = true;

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (show_demo_window == true)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	return UPDATE_CONTINUE;
}

update_status BacchusInterface::PostUpdate(float dt)
{

	ImGui::Render();

	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	


	return UPDATE_CONTINUE;
}