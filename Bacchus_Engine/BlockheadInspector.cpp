#include "Application.h"
#include "ModuleSceneManager.h"
#include "GameObject.h"
#include "imgui/imgui.h"
#include "BlockheadInspector.h"
#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ResourceRenderer.h"
#include "ResourceTransform.h"

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

		ImGui::BeginChild("child", ImVec2(0, 35), true);

		static char GOName[100] = "";
		strcpy_s(GOName, 100, Selected->GetName().data());
		if (ImGui::InputText("", GOName, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			Selected->SetName(GOName);

		ImGui::EndChild();

		if (Startup)
		ImGui::SetNextItemOpen(true);

		if (ImGui::TreeNode("Transform"))
		{
			ResourceTransform* transform = Selected->GetResource<ResourceTransform>(Res::ResType::Transform);

			ImGui::Text("Position  ");
			ImGui::SameLine();

			float3 position = transform->GetPosition();
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			ImGui::DragFloat("PX", &position.x, 0.005f);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			ImGui::DragFloat("PY", &position.y, 0.005f);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			ImGui::DragFloat("PZ", &position.z, 0.005f);

			ImGui::Text("Rotation  ");
			ImGui::SameLine();

			float3 rotation = transform->GetRotation();
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			ImGui::DragFloat("RX", &rotation.x, 0.005f);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			ImGui::DragFloat("RY", &rotation.y, 0.005f);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			ImGui::DragFloat("RZ", &rotation.z, 0.005f);

			ImGui::Text("Scale     ");
			ImGui::SameLine();

			float3 scale = transform->GetScale();
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			ImGui::DragFloat("SX", &scale.x, 0.005f);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			ImGui::DragFloat("SY", &scale.y, 0.005f);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);

			ImGui::DragFloat("SZ", &scale.z, 0.005f);

			transform->SetPosition(position.x, position.y, position.z);
			transform->Scale(scale.x, scale.y, scale.z);


			ImGui::TreePop();
		}

		ImGui::Separator();

		if (Selected->GetResource<ResourceMesh>(Res::ResType::Mesh))
		{
			ResourceMesh* mesh = Selected->GetResource<ResourceMesh>(Res::ResType::Mesh);

			if (Startup)
				ImGui::SetNextItemOpen(true);

			if (ImGui::TreeNode("Mesh"))
			{
				std::string Triangle_count = "Triangles   ";
				Triangle_count.append(std::to_string(mesh->IndicesSize / 3));
				ImGui::Text(Triangle_count.data());

				ImGui::TreePop();
			}
		}
		ImGui::Separator();

		if (Selected->GetResource<ResourceRenderer>(Res::ResType::Renderer))
		{

			ResourceRenderer* renderer = Selected->GetResource<ResourceRenderer>(Res::ResType::Renderer);

			if (Startup)
				ImGui::SetNextItemOpen(true);

			if (ImGui::TreeNode("Mesh Renderer"))
			{
				ImGui::Checkbox("Vertex Normals", &renderer->draw_vertexnormals);
				ImGui::SameLine();
				ImGui::Checkbox("Face Normals  ", &renderer->draw_facenormals);
				ImGui::SameLine();
				ImGui::Checkbox("Checkers", &renderer->checkers);

				ImGui::TreePop();
			}
		}
		ImGui::Separator();

		if (Selected->GetResource<ResourceMaterial>(Res::ResType::Material))
		{
			ResourceMaterial* material = Selected->GetResource<ResourceMaterial>(Res::ResType::Material);

			if (Startup)
				ImGui::SetNextItemOpen(true);

			if (ImGui::TreeNode("Material"))
			{
				std::string Path = "Path: ";
				Path.append(material->Texture_path);

				ImGui::Text(Path.data());

				ImGui::Text(std::to_string(material->Texture_width).data());
				ImGui::SameLine();
				ImGui::Text(std::to_string(material->Texture_height).data());

				ImGui::TreePop();
			}
		}

		ImGui::Separator();

		if (Startup)
			Startup = false;
	}

	ImGui::End();


	return true;
}