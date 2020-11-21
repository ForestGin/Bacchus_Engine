#include "BlockheadToolbar.h"
#include "Imgui/imgui.h"


#include "mmgr/mmgr.h"

BlockheadToolbar::BlockheadToolbar(char* name) : Blockhead(name)
{
}

BlockheadToolbar::~BlockheadToolbar()
{
}

bool BlockheadToolbar::Draw()
{
	ImGuiWindowFlags settingsFlags = 0;
	settingsFlags = ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoTitleBar;

	if (ImGui::Begin(name, &enabled, settingsFlags))
	{
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() / 2 - 100);

		if (ImGui::Button("PLAY"))
		{

		}
		ImGui::SameLine();

		if (ImGui::Button("PAUSE"))
		{

		}
		ImGui::SameLine();

		if (ImGui::Button("STOP"))
		{

		}
	}

	ImGui::End();


	return true;
}