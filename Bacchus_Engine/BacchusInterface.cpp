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
		
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Documentation"))
				App->RequestBrowser("https://github.com/ForestGin/Bacchus_Engine");
			if (ImGui::MenuItem("Download latest"))
				App->RequestBrowser("https://github.com/ForestGin/Bacchus_Engine/releases");
			if (ImGui::MenuItem("Report a bug"))
				App->RequestBrowser("https://github.com/ForestGin/Bacchus_Engine/issues");
			if (ImGui::MenuItem("About"))
				about_window = true;

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

		ImGui::EndMainMenuBar();
	}

	//CONFIG WINDOW LOGIC
	if (config_window == true)
	{
		ImGui::Begin("Configuration", &config_window, ImGuiWindowFlags_MenuBar);
		
		if (ImGui::CollapsingHeader("Application"))
		{
	
		}
		if (ImGui::CollapsingHeader("Window"))
		{

		}
		if (ImGui::CollapsingHeader("Hardware"))
		{

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