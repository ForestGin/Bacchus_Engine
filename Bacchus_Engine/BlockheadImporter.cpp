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

		ImGui::SetNextItemOpen(true);
	
		if (ImGui::TreeNode("Transform"))
		{			

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			
			ImGui::InputFloat("Scale", &scale);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);


			ImGui::InputFloat("Axis", &axis);

			if (ImGui::Button("X-UP"))
			{
				SetXAxis();
			}

			if (ImGui::Button("Z-UP"))
			{
				SetZAxis();
			}

			if (ImGui::Button("IgnoreCam"))
			{
				SetCamIgnored();
			}

			if (ImGui::Button("FLIP"))
			{
				SetFlipped();

			}


			ImGui::TreePop();
			
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

	//reset image flip selection
	App->importer->IScene->fliped = false;

	App->importer->IScene->Xaxis = false;
	App->importer->IScene->Zaxis = false;

	return ret;
}

void BlockheadImporter::SetFlipped()
{
	App->importer->IScene->FlipIt();
}

void BlockheadImporter::SetCamIgnored()
{
	App->importer->IScene->IgnoreCam();
}

void BlockheadImporter::SetXAxis()
{
	App->importer->IScene->XAxisUp();
}

void BlockheadImporter::SetZAxis()
{
	App->importer->IScene->ZAxisUp();
}