#include "Application.h"
#include "Globals.h"
#include "BacchusInterface.h"

#include "imgui/imgui_internal.h"
#include <gl/GLU.h>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include <string>

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

	//aiDetachAllLogStreams();

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
		
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Documentation"))
				App->RequestBrowser("https://github.com/ForestGin/Bacchus_Engine");
			if (ImGui::MenuItem("Download latest"))
				App->RequestBrowser("https://github.com/ForestGin/Bacchus_Engine/releases");
			if (ImGui::MenuItem("Report a bug"))
				App->RequestBrowser("https://github.com/ForestGin/Bacchus_Engine/issues");
			if (ImGui::MenuItem("About"))
				about_window = !about_window;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Configuration"))
		{
			if (ImGui::MenuItem("Open"))
				config_window = true;
			if (ImGui::MenuItem("Close"))
				config_window = false;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Console"))
		{
			if (ImGui::MenuItem("Open"))
				console_window = true;
			if (ImGui::MenuItem("Close"))
				console_window = false;

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	//CONFIG WINDOW LOGIC
	if (config_window == true)
	{
		ImGui::Begin("Configuration", &config_window, ImGuiWindowFlags_MenuBar);
		
		if (ImGui::CollapsingHeader("Application"))
		{
			FPSGraph();
		}
		if (ImGui::CollapsingHeader("Window"))
		{
			WindowConfig();
		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
			Hardware();
		}
		if (ImGui::CollapsingHeader("Mouse"))
		{
			App->input->MousePos();
		}

		ImGui::End();
	}
	
	//DEMO WINDOW LOGIC
	if (show_demo_window == true)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	//ABOUT MODAL WINDOW
	if (about_window == true)
	{
		ImGui::Begin("About Bacchus Engine");

		ImGui::Text("Bacchus Engine v0.0\n");
		ImGui::Text("An engine made for and by borreichs\n\n");
		ImGui::Text("By Pol Bosch & Oscar Larios\n\n");
		ImGui::Text("3rd Party Libraries used: \n");
		ImGui::BulletText("SDL v2.0.12");
		ImGui::BulletText("Glew v2.0.0");
		ImGui::BulletText("ImGui v1.78");
		ImGui::BulletText("MathGeoLib v1.5\n\n");

		ImGui::Text("License\n\n");
		ImGui::Text("MIT License\n\n");
		ImGui::Text("Copyright(c) 2020 Pol Bosch & Oscar Larios\n\n");
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy\nof this software and associated documentation files (the 'Software'), to deal\n");
		ImGui::Text("in the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and /or sell\n");
		ImGui::Text("copies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions : \n");
		ImGui::Text("\n");
		ImGui::Text("The above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software.\n");
		ImGui::Text("\n");
		ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, \n");
		ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n");
		ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n");
		ImGui::Text("SOFTWARE.\n");


		ImGui::End();
	}
	
	if (console_window == true)
	{
		ImGui::Begin("Console");
		ImGui::TextUnformatted(console_text.begin());
		ImGui::SetScrollHere(1.0f);
		ImGui::End();
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

void BacchusInterface::Hardware()
{
	ImGui::Text("SDL Version: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "2.0.12");
	ImGui::Text("CPUs: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d (Cache: %ikb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
	ImGui::Text("System RAM: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%iGb", SDL_GetSystemRAM() / 1000);
	ImGui::Text("Caps: ");
	ImGui::SameLine();

	if (SDL_Has3DNow() == true) {
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "3DNow, ");
	}
	ImGui::SameLine();
	if (SDL_HasAVX() == true) {
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "AVX, ");
	}
	ImGui::SameLine();
	if (SDL_HasAVX2() == true) {
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "AVX2, ");
	}
	ImGui::SameLine();
	if (SDL_HasAltiVec() == true) {
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "AltiVec, ");
	}
	ImGui::SameLine();
	if (SDL_HasMMX() == true) {
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "MMX, ");
	}

	if (SDL_HasRDTSC() == true) {
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "RDTSC, ");
	}
	ImGui::SameLine();
	if (SDL_HasSSE() == true) {
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE, ");
	}
	ImGui::SameLine();
	if (SDL_HasSSE2() == true) {
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE2, ");
	}
	ImGui::SameLine();
	if (SDL_HasSSE3() == true) {
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE3, ");
	}
	ImGui::SameLine();
	if (SDL_HasSSE41() == true) {
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE41, ");
	}
	ImGui::SameLine();
	if (SDL_HasSSE42() == true) {
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE42, ");
	}
	ImGui::Text("GPU: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", glGetString(GL_RENDERER));
	ImGui::Text("Brand: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", glGetString(GL_VENDOR));
	ImGui::Text("VRAM Usage: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d Mb", glGetString(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX));
	
	  
}

void BacchusInterface::WindowConfig()
{

	if (ImGui::SliderFloat("Brightness", &App->window->brightness, 0.0f, 1.0f)) {
		SDL_SetWindowBrightness(App->window->window, App->window->brightness);
		SDL_UpdateWindowSurface(App->window->window);
	};

	if (ImGui::SliderInt("Width", &App->window->width, 0, 1920) || ImGui::SliderInt("Height", &App->window->height, 0, 1080)) {
		SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);
		SDL_UpdateWindowSurface(App->window->window);
	};

	ImGui::Text("Refresh Rate: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%f", App->fps);

	if (ImGui::Checkbox("FullScreen", &App->window->fullscreen)) {
		if (App->window->fullscreen) SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
		else { SDL_SetWindowFullscreen(App->window->window, App->window->flags); }
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Resizable", &App->window->resizable)) {
		if (App->window->resizable)SDL_SetWindowResizable(App->window->window, SDL_TRUE);
		else { SDL_SetWindowResizable(App->window->window, SDL_FALSE); }
	}

	if (ImGui::Checkbox("Borderless", &App->window->borderless)) {
		if (App->window->borderless) SDL_SetWindowBordered(App->window->window, SDL_FALSE);
		else { SDL_SetWindowBordered(App->window->window, SDL_TRUE); }
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Full Desktop", &App->window->fulldesktop)) {
		if (App->window->fulldesktop) SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		else { SDL_SetWindowFullscreen(App->window->window, App->window->flags); }
	}
}

void BacchusInterface::FPSGraph()
{
	App->JasonReading();

	ImGui::InputText("App Name", TITLE, 20);
	ImGui::InputText("Organitzation", ORGANIZATION, 20);

	if (App->fps_log.size() != 30)
	{
		App->fps_log.push_back(App->fps);
		App->ms_log.push_back(App->dt);
	}
	else
	{
		App->fps_log.erase(App->fps_log.begin());
		App->fps_log.push_back(App->fps);
		App->ms_log.erase(App->ms_log.begin());
		App->ms_log.push_back(App->dt);
	}

	char title[25];
	sprintf_s(title, 25, "Framerate %.1f", App->fps_log[App->fps_log.size() - 1]);
	ImGui::PlotHistogram("##framerate", &App->fps_log[0], App->fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
	sprintf_s(title, 25, "Milliseconds %.1f", App->ms_log[App->ms_log.size() - 1]);
	ImGui::PlotHistogram("##framerate", &App->ms_log[0], App->ms_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

}

void BacchusInterface::ConsoleText(std::string console_Text)
{
	console_text.appendf(console_Text.c_str());
}
