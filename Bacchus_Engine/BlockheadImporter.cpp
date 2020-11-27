#include "BlockheadImporter.h"
#include "Imgui/imgui.h"

#include "mmgr/mmgr.h"

BlockheadImporter::BlockheadImporter(char* name) : Blockhead(name)
{
}

BlockheadImporter::~BlockheadImporter()
{
}

bool BlockheadImporter::Draw()
{
	ImGuiWindowFlags settingsFlags = 0;
	settingsFlags = ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin(name, &enabled, settingsFlags))
	{
		
	}

	ImGui::End();

	return true;
}
