#include "BlockheadImporter.h"
#include "Imgui/imgui.h"
#include "Importer.h"
#include "ImporterScene.h"
#include "Application.h"
#include "ModuleImporter.h"

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
	settingsFlags = ImGuiWindowFlags_NoDocking;

	if (ImGui::Begin(name, &enabled, settingsFlags))
	{
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 4);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);

		if (ImGui::Button("APPLY"))
		{
			/*applied = true;*/
			TestImport();
			OnOff();
		}

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 80);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);

		if (ImGui::Button("Default"))
		{
			defaulted = true;
			OnOff();
		}
		
	}

	ImGui::End();

	return true;
}


bool BlockheadImporter::TestImport()
{
	bool ret = true;
	
	ImportData data;
	ret = App->importer->IScene->Import(App->importer->DroppedFile_path.data(), data);

	return ret;
}