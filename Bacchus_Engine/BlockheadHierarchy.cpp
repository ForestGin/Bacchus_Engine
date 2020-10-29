#include "BlockheadHierarchy.h"
#include "Imgui/imgui.h"

#include "Application.h"
#include "ModuleSceneManager.h"

#include "GameObject.h"


#include "mmgr/mmgr.h"

BlockheadHierarchy::BlockheadHierarchy(char* name) : Blockhead(name)
{
}

BlockheadHierarchy::~BlockheadHierarchy()
{
}

bool BlockheadHierarchy::Draw()
{
	ImGuiWindowFlags settingsFlags = 0;
	settingsFlags = ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin(name, &enabled, settingsFlags))
	{
		ImGui::BeginChild("GO_List", ImVec2(325, 0), false);
		for (int i = 0; i < App->scene_manager->GetNumGameObjects(); i++)
		{
			if (ImGui::Selectable(App->scene_manager->GetGameObjects().at(i)->GetName().data(), App->scene_manager->GetSelectedGameObjects() == i))
				App->scene_manager->SetSelectedGameObject(i);
		}
		ImGui::EndChild();
		ImGui::SameLine();
	}

	ImGui::End();


	return true;
}