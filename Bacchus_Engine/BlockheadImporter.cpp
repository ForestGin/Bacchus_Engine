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

	if (importObject == nullptr)
	{
		importObject = App->scene_manager->CreateEmptyGameObject();
		App->scene_manager->SetSelectedGameObject(importObject);
	}

	if (ImGui::Begin(name, &enabled, settingsFlags))
	{
		
		/*if (importObject == nullptr)
		{
			importObject = App->scene_manager->CreateEmptyGameObject();
			App->scene_manager->SetSelectedGameObject(importObject);
		}*/

		ImGui::SetNextItemOpen(true);
	
		if (ImGui::TreeNode("Transform"))
		{
			ComponentTransform* importTransform = importObject->GetComponent<ComponentTransform>(Component::ComponentType::Transform);

			float3  importScale = importTransform->GetScale();

			

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			
			ImGui::InputFloat("Scale", &scale);

			importScale.x = scale;
			importScale.y = scale;
			importScale.z = scale;


			if (!importTransform->GetScale().Equals(importScale))
				importTransform->Scale(importScale.x, importScale.y, importScale.z);

		

			ImGui::TreePop();
			
		}

		//---------------------------BUTTONS
	
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 4);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);

		if (ImGui::Button("APPLY"))
		{
		
			TestImport();
			/*importObject->RecursiveDelete(importObject);*/
			/*Destroy();*/

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

void BlockheadImporter::Destroy()
{
	App->scene_manager->DestroyGameObject(importObject);
	importObject = nullptr;
	App->scene_manager->SetSelectedGameObject(nullptr);
}