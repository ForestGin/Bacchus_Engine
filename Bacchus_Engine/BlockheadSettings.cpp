#include <vector>
#include "BlockheadSettings.h"
#include "Application.h"
#include "Imgui/imgui.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "BacchusHardware.h"
#include "ModuleTimeManager.h"
#include "ModuleSceneManager.h"

#include "mmgr/mmgr.h"

BlockheadSettings::BlockheadSettings(char* name) : Blockhead(name), FPS_Tracker(FPS_TRACKER_SIZE), MS_Tracker(FPS_TRACKER_SIZE)
{

}

BlockheadSettings::~BlockheadSettings()
{

}

bool BlockheadSettings::Draw()
{
	ImGuiWindowFlags settingsFlags = 0;
	settingsFlags = ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin(name, &enabled, settingsFlags))
	{
		if (ImGui::CollapsingHeader("Application"))
		{
			ApplicationNode();
			ImGui::Separator();
		}
		if (ImGui::CollapsingHeader("Window"))
		{
			WindowNode();
			ImGui::Separator();
		}
		if (ImGui::CollapsingHeader("Input"))
		{
			InputNode();
			ImGui::Separator();
		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
			HardwareNode();
			ImGui::Separator();
		}
		if (ImGui::CollapsingHeader("Renderer"))
		{
			RendererNode();
			ImGui::Separator();
		}
	}
	ImGui::End();


	return true;
}

void BlockheadSettings::ApplicationNode() const
{
	// Application name
	static char appName[100];
	if (App->GetAppName() != nullptr)
		strcpy_s(appName, 100, App->GetAppName());
	if (ImGui::InputText("App Name", appName, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		App->SetAppName(appName);

	ImGui::Separator();

	// Organization name 
	static char orgName[100];
	if (App->GetOrganizationName() != nullptr)
		strcpy_s(orgName, 100, App->GetOrganizationName());
	if (ImGui::InputText("Organization Name", orgName, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		App->SetOrganizationName(orgName);

	ImGui::Separator();
	// Cap frames
	int maxFramerate = App->time->GetMaxFramerate();
	if (ImGui::SliderInt("Max FPS", &maxFramerate, 0, App->window->GetDisplayRefreshRate()))
		App->time->SetMaxFramerate(maxFramerate);

	ImGui::SameLine();
	// VSync
	bool vsync = App->renderer3D->GetVSync();
	if (ImGui::Checkbox("Use VSync", &vsync))
	{
		App->renderer3D->SetVSync(vsync);
	}

	ImGui::Separator();

	ImGui::Text("Limit Framerate:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", App->time->GetMaxFramerate());

	// Framerate && Ms 
	char title[25];
	sprintf_s(title, 25, "Framerate %.1f", FPS_Tracker[FPS_Tracker.size() - 1]);
	ImGui::PlotHistogram("##Framerate", &FPS_Tracker[0], FPS_Tracker.size(), 0, title, 0.0f, 100.0f, ImVec2(500, 75));
	sprintf_s(title, 25, "Milliseconds %0.1f", MS_Tracker[MS_Tracker.size() - 1]);
	ImGui::PlotHistogram("##Milliseconds", &MS_Tracker[0], MS_Tracker.size(), 0, title, 0.0f, 40.0f, ImVec2(500, 75));

	// Memory 

	sMStats memStats = m_getMemoryStatistics();

	static std::vector<float> memory(100);

	for (uint i = memory.size() - 1; i > 0; --i)
		memory[i] = memory[i - 1];

	memory[0] = (float)memStats.peakActualMemory;

	ImGui::PlotHistogram("##RAMusage", &memory.front(), memory.size(), 0, "Ram Usage", 0.0f, (float)memStats.peakReportedMemory * 1.2f, ImVec2(310, 100));

	ImGui::Text("Total Reported Mem: %u", memStats.totalReportedMemory);
	ImGui::Text("Total Actual Mem: %u", memStats.totalActualMemory);
	ImGui::Text("Peak Reported Mem: %u", memStats.peakReportedMemory);
	ImGui::Text("Peak Actual Mem: %u", memStats.peakActualMemory);
	ImGui::Text("Accumulated Reported Mem: %u", memStats.accumulatedReportedMemory);
	ImGui::Text("Accumulated Actual Mem: %u", memStats.accumulatedActualMemory);
	ImGui::Text("Acumulated Alloc Unit Count: %u", memStats.accumulatedAllocUnitCount);
	ImGui::Text("Total Alloc Unit Count: %u", memStats.totalAllocUnitCount);
	ImGui::Text("Peak Alloc Unit Count: %u", memStats.peakAllocUnitCount);
}

void BlockheadSettings::WindowNode() const
{
	ImGui::Separator();
	//Brightness
	float brightness = App->window->GetWinBrightness();
	if (ImGui::SliderFloat("Window Brightness", &brightness, 0.0f, 1.0f))
		App->window->SetWinBrightness(brightness);

	// Window ReSize
	uint width, height, min_width, min_height, max_width, max_height;
	App->window->GetWinMaxMinSize(min_width, min_height, max_width, max_height);
	width = App->window->GetWindowWidth();
	height = App->window->GetWindowHeight();

	if (ImGui::SliderInt("Width", (int*)&width, min_width, max_width))
		App->window->SetWindowWidth(width);
	if (ImGui::SliderInt("Height", (int*)&height, min_height, max_height))
		App->window->SetWindowHeight(height);

	ImGui::Separator();

	//Refresh Rate Indicator
	ImGui::Text("Refresh Rate:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "%u", App->window->GetDisplayRefreshRate());


	//Window Flags
	bool fullscreen = App->window->IsFullscreen();
	bool fullscreen_desktop = App->window->IsFullscreenDesktop();
	bool resizable = App->window->IsResizable();
	bool borderless = App->window->IsBorderless();

	if (ImGui::Checkbox("Fullscreen", &fullscreen))
		App->window->SetFullscreen(fullscreen);

	ImGui::SameLine();

	if (ImGui::Checkbox("Resizable", &resizable))
		App->window->SetResizable(resizable);

	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Restart to apply"); // Cannot Change Resizable flag at runtime

	if (ImGui::Checkbox("Borderless", &borderless))
		App->window->SetBorderless(borderless);

	ImGui::SameLine();

	if (ImGui::Checkbox("FullScreen Desktop", &fullscreen_desktop))
		App->window->SetFullscreenDesktop(fullscreen_desktop);
}

void BlockheadSettings::InputNode() const
{
	ImGui::Separator();
	//Mouse position
	int mouse_x, mouse_y;
	mouse_x = App->input->GetMouseX();
	mouse_y = App->input->GetMouseY();
	ImGui::Text("Mouse Position:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i,%i", mouse_x, mouse_y);

	//Mouse Motion
	mouse_x = App->input->GetMouseXMotion();
	mouse_y = App->input->GetMouseYMotion();
	ImGui::Text("Mouse Motion:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i,%i", mouse_x, mouse_y);

	//Mouse Wheel acc
	int wheel = App->input->GetMouseZ();
	ImGui::Text("Mouse Wheel:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", wheel);

	ImGui::Separator();
}

void BlockheadSettings::HardwareNode() const
{
	hw_info hardware_info = App->hardware->GetInfo();

	ImGui::Separator();
	//SDL Version 
	ImGui::Text("SDL Version:");	ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", hardware_info.sdl_version);
	ImGui::Separator();

	//CPU
	ImGui::Text("CPU Logic Cores:");	ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", hardware_info.cpu_count);

	ImGui::Text("CPU L1 Cache (Kb):");	ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", hardware_info.l1_cachekb);

	ImGui::Text("CPU Instruction Support:"); ImGui::SameLine();

	if (hardware_info.rdtsc)
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", "rdtsc"); ImGui::SameLine();
	if (hardware_info.altivec)
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", "altivec"); ImGui::SameLine();
	if (hardware_info.now3d)
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", "now3d"); ImGui::SameLine();
	if (hardware_info.mmx)
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", "mmx");	ImGui::SameLine();
	if (hardware_info.sse)
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", "sse");	ImGui::SameLine();
	if (hardware_info.sse2)
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", "sse2"); ImGui::SameLine();
	if (hardware_info.sse3)
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", "sse3"); ImGui::SameLine();
	if (hardware_info.sse41)
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", "sse41"); ImGui::SameLine();
	if (hardware_info.sse42)
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", "sse42"); ImGui::SameLine();
	if (hardware_info.avx)
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", "avx");	ImGui::SameLine();
	if (hardware_info.avx2)
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", "avx2");

	ImGui::Separator();
	//RAM
	ImGui::Text("RAM Memory (Gb)"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "%f", hardware_info.ram_gb);

	ImGui::Separator();

	// GPU
	ImGui::Text("GPU Vendor");	ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", hardware_info.gpu_vendor.data());

	ImGui::Text("GPU Model"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", hardware_info.gpu_brand.data());

	ImGui::Text("GPU Driver"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", hardware_info.gpu_driver.data());

	// (Currently NVIDIA only)
	ImGui::Text("VRAM Budget");	ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "%f", hardware_info.vram_mb_budget);

	ImGui::Text("VRAM Available"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "%f", hardware_info.vram_mb_available);

	ImGui::Text("VRAM Usage"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "%f", hardware_info.vram_mb_usage);
}

void BlockheadSettings::RendererNode() const
{
	//wireframe
	if (ImGui::Checkbox("Wireframe", &App->renderer3D->wireframe))
	{
		App->renderer3D->wireframe != App->renderer3D->wireframe;

	}

	//depth-test
	if (ImGui::Checkbox("Depth", &App->renderer3D->depth_test))
	{
		App->renderer3D->depth_test != App->renderer3D->depth_test;

	}

	//lightning
	if (ImGui::Checkbox("Lights", &App->renderer3D->lighting))
	{
		App->renderer3D->lighting != App->renderer3D->lighting;

	}

	//color material
	if (ImGui::Checkbox("Color Material", &App->renderer3D->color_material))
	{
		App->renderer3D->color_material != App->renderer3D->color_material;

	}

	//color material
	if (ImGui::Checkbox("Bounding Boxes", &App->scene_manager->display_boundingboxes))
	{
		App->scene_manager->display_boundingboxes != App->scene_manager->display_boundingboxes;
	}
}

void BlockheadSettings::AddFPS(float fps, float ms)
{
	static uint counter = 0;

	// If the plot historiogram is already full we shift 
	if (counter == FPS_TRACKER_SIZE)
	{
		for (uint i = 0; i < FPS_TRACKER_SIZE - 1; ++i)
		{
			FPS_Tracker[i] = FPS_Tracker[i + 1];
			MS_Tracker[i] = MS_Tracker[i + 1];
		}
	}
	else // We count until the max size is reached, then substitute values in the if above
		++counter;

	FPS_Tracker[counter - 1] = fps;
	MS_Tracker[counter - 1] = ms;
}