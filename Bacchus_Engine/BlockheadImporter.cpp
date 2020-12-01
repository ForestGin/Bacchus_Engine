#include "BlockheadImporter.h"
#include "Imgui/imgui.h"
#include "Importer.h"
#include "ImporterScene.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "ModuleSceneManager.h"
#include "BacchusEditor.h"
#include "BlockheadInspector.h"

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

		importObject = App->scene_manager->CreateEmptyGameObject();

		if (importObject == nullptr)
		{
			ImGui::End();
			return false;
		}

		ImGui::SetNextItemOpen(true);
	
		if (ImGui::TreeNode("Transform"))
		{
			ComponentTransform* importTransform = importObject->GetComponent<ComponentTransform>(Component::ComponentType::Transform);

			float3  importScale = importTransform->GetScale();
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			ImGui::DragFloat("SX", &importScale.x, 0.005f);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			ImGui::DragFloat("SY", &importScale.y, 0.005f);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			ImGui::DragFloat("SZ", &importScale.z, 0.005f);

			if (!importTransform->GetScale().Equals(importScale))
				importTransform->Scale(importScale.x, importScale.y, importScale.z);

		

			ImGui::TreePop();
			
		

			/*App->bacchuseditor->blockheadInspector->Selected = App->scene_manager->GetSelectedGameObjects();

			App->bacchuseditor->blockheadInspector->transform = App->bacchuseditor->blockheadInspector->Selected->GetComponent<ComponentTransform>(Component::ComponentType::Transform);

			App->bacchuseditor->blockheadInspector->scale = App->bacchuseditor->blockheadInspector->transform->GetScale();
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			ImGui::DragFloat("SX", &App->bacchuseditor->blockheadInspector->scale.x, 0.005f);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			ImGui::DragFloat("SY", &App->bacchuseditor->blockheadInspector->scale.y, 0.005f);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			ImGui::DragFloat("SZ", &App->bacchuseditor->blockheadInspector->scale.z, 0.005f);

			if (!App->bacchuseditor->blockheadInspector->transform->GetScale().Equals(App->bacchuseditor->blockheadInspector->scale))
				App->bacchuseditor->blockheadInspector->transform->Scale(App->bacchuseditor->blockheadInspector->scale.x, App->bacchuseditor->blockheadInspector->scale.y, App->bacchuseditor->blockheadInspector->scale.z);

			ImGui::TreePop();*/
		}

		//---------------------------BUTTONS
	
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 4);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);

		if (ImGui::Button("APPLY"))
		{
		
			TestImport();
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