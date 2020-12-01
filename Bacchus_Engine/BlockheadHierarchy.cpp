#include "BlockheadHierarchy.h"
#include "Imgui/imgui.h"
#include "Application.h"
#include "ModuleSceneManager.h"
#include "ModuleInput.h"

#include "ModuleImporter.h"
#include "ImporterScene.h"
#include "BacchusEditor.h"
#include "FileSystem.h"

#include "GameObject.h"
#include "BlockheadProject.h"

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
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FBX"))
			{
				if (App->bacchuseditor->blockheadProject)
				{
					std::string extension;
					App->fs->SplitFilePath(App->bacchuseditor->blockheadProject->dragged.data(), nullptr, nullptr, &extension);

					if (extension.compare("fbx") == 0 || extension.compare("FBX") == 0)
					{
						ImportData data;
						App->importer->GetImporterScene()->Import(App->bacchuseditor->blockheadProject->dragged.data(), data);
						App->bacchuseditor->blockheadProject->dragged = "";
					}
				}
			}

			ImGui::EndDragDropTarget();
		}

		DrawRecursive(App->scene_manager->GetRootGO());
	}
	ImGui::End();

	if (end_drag)
	{
		if (!dragged->FindChildGO(target) && target != dragged)
				target->AddChildGO(dragged);

		end_drag = false;
		dragged = nullptr;
		target = nullptr;
	}

	if (to_destroy)
	{
		App->scene_manager->DestroyGameObject(to_destroy);
		to_destroy = nullptr;
		App->scene_manager->SetSelectedGameObject(nullptr);
	}

	return true;
}

void BlockheadHierarchy::DrawRecursive(GameObject* Go)
{
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow;

	ImGuiTreeNodeFlags node_flags = base_flags;

	if (Go == App->scene_manager->GetSelectedGameObjects())
		node_flags |= ImGuiTreeNodeFlags_Selected;

	if (Go->GetName().data() == App->scene_manager->GetRootGO()->GetName())
	{
		if (Go->childs.size() > 0)
		{
			for (std::vector<GameObject*>::iterator it = Go->childs.begin(); it != Go->childs.end(); ++it)
			{
				DrawRecursive(*it);
			}
		}
	}

	else
	{
		if (Go->childs.size() == 0)
			node_flags |= ImGuiTreeNodeFlags_Leaf;

		bool open = ImGui::TreeNodeEx((void*)Go->GetUID(), node_flags, Go->GetName().data());

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload("GO", Go, sizeof(GameObject));   
			dragged = Go;
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GO"))
			{
				target = Go;
				end_drag = true;
			}

			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsWindowFocused() && Go == App->scene_manager->GetSelectedGameObjects() && App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
		{
			LOG("Destroying: %s ...", Go->GetName().data());
			to_destroy = Go;
		}

		if (ImGui::IsItemClicked())
			App->scene_manager->SetSelectedGameObject(Go);

		if (open)
		{
			if (Go->childs.size() > 0)
			{
				for (std::vector<GameObject*>::iterator it = Go->childs.begin(); it != Go->childs.end(); ++it)
				{
					DrawRecursive(*it);
				}
			}

			ImGui::TreePop();
		}

	}
}