#include "BlockheadProject.h"

#include "Imgui/imgui.h"


#include "mmgr/mmgr.h"

BlockheadProject::BlockheadProject(char* name) : Blockhead(name)
{
}

BlockheadProject::~BlockheadProject()
{
}

bool BlockheadProject::Draw()
{
	ImGuiWindowFlags settingsFlags = 0;
	settingsFlags = ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin(name, &enabled, settingsFlags))
	{

	}

	ImGui::End();


	return true;
}