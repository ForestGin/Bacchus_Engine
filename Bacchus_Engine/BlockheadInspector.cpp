#include "Application.h"
#include "ModuleSceneManager.h"
#include "GameObject.h"
#include "imgui/imgui.h"
#include "BlockheadInspector.h"

#include "mmgr/mmgr.h"

BlockheadInspector::BlockheadInspector(char* name) : Blockhead(name)
{
}

BlockheadInspector::~BlockheadInspector()
{
}

bool BlockheadInspector::Draw()
{
	ImGuiWindowFlags settingsFlags = 0;
	settingsFlags = ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin(name, &enabled, settingsFlags))
	{
		GameObject* Selected = App->scene_manager->GetGameObjects().at(App->scene_manager->GetSelectedGameObjects());

		ImGui::SetNextItemOpen(true);

		if (ImGui::TreeNode("Transform"))
		{

			ImGui::Text("Position  ");
			ImGui::SameLine();

			float3 position = Selected->GetPosition();
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(75.0f);

			ImGui::DragFloat("PX", &position.x, 0.005f);

			ImGui::SameLine();

			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(75.0f);

			ImGui::DragFloat("PY", &position.y, 0.005f);

			ImGui::SameLine();

			ImGui::Text("Z");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(75.0f);

			ImGui::DragFloat("PZ", &position.z, 0.005f);


			ImGui::Text("Scale     ");
			ImGui::SameLine();

			float3 scale = Selected->GetScale();
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(75.0f);

			ImGui::DragFloat("SX", &scale.x, 0.005f);

			ImGui::SameLine();

			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(75.0f);

			ImGui::DragFloat("SY", &scale.y, 0.005f);

			ImGui::SameLine();

			ImGui::Text("Z");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(75.0f);

			ImGui::DragFloat("SZ", &scale.z, 0.005f);

			Selected->SetPosition(position.x, position.y, position.z);
			Selected->Scale(scale.x, scale.y, scale.z);


			ImGui::TreePop();
		}

		ImGui::Separator();

	}

	ImGui::End();


	return true;
}