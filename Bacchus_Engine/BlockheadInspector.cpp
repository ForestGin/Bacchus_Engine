#include "Application.h"
#include "ModuleSceneManager.h"
#include "GameObject.h"
#include "imgui/imgui.h"
#include "BlockheadInspector.h"

#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentRenderer.h"
#include "ComponentTransform.h"

#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ResourceTexture.h"

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
		GameObject* Selected = App->scene_manager->GetSelectedGameObjects();

		if (Selected == nullptr)
		{
			ImGui::End();
			return false;
		}

		ImGui::BeginChild("child", ImVec2(0, 35), true);

		ImGui::Checkbox("##GOActive", &Selected->GetActive());
		ImGui::SameLine();

		static char GOName[100] = "";
		strcpy_s(GOName, 100, Selected->GetName().data());
		if (ImGui::InputText("", GOName, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			Selected->SetName(GOName);

		ImGui::EndChild();

		if (Startup)
		ImGui::SetNextItemOpen(true);

		if (ImGui::TreeNode("Transform"))
		{
			ComponentTransform* transform = Selected->GetComponent<ComponentTransform>(Component::ComponentType::Transform);

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

			if (!transform->GetPosition().Equals(position))
				transform->SetPosition(position.x, position.y, position.z);
			if (!transform->GetScale().Equals(scale))
				transform->Scale(scale.x, scale.y, scale.z);
			if (!transform->GetRotation().Equals(rotation))
				transform->SetRotation(rotation);


			ImGui::TreePop();
		}

		ImGui::Separator();

		if (Selected->GetComponent<ComponentMesh>(Component::ComponentType::Mesh))
		{
			ComponentMesh* mesh = Selected->GetComponent<ComponentMesh>(Component::ComponentType::Mesh);

			ImGui::Checkbox("##MeshActive", &mesh->GetActive());
			ImGui::SameLine();

			if (Startup)
				ImGui::SetNextItemOpen(true);

			if (ImGui::TreeNode("Mesh"))
			{
				std::string Triangle_count = "Triangles   ";
				Triangle_count.append(std::to_string(mesh->resource_mesh->IndicesSize / 3));
				ImGui::Text(Triangle_count.data());

				ImGui::Text("Instances:");
				ImGui::SameLine();
				ImGui::Text(std::to_string(mesh->resource_mesh->instances).data());

				ImGui::TreePop();
			}
		}
		ImGui::Separator();

		if (Selected->GetComponent<ComponentRenderer>(Component::ComponentType::Renderer))
		{

			ComponentRenderer* renderer = Selected->GetComponent<ComponentRenderer>(Component::ComponentType::Renderer);

			ImGui::Checkbox("##RenActive", &renderer->GetActive());
			ImGui::SameLine();

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

		if (Selected->GetComponent<ComponentMaterial>(Component::ComponentType::Material))
		{
			ComponentMaterial* material = Selected->GetComponent<ComponentMaterial>(Component::ComponentType::Material);

			ImGui::Checkbox("##MatActive", &material->GetActive());
			ImGui::SameLine();

			if (Startup)
				ImGui::SetNextItemOpen(true);

			if (ImGui::TreeNode("Material"))
			{
				std::string Path = "Path: ";
				Path.append(material->resource_material->resource_diffuse->Texture_path);

				ImGui::Text(Path.data());

				ImGui::Image((void*)(uint)&material->resource_material->resource_diffuse->buffer_id, ImVec2(150, 150));
				
			ImGui::Text("Instances:");
				ImGui::SameLine();
				ImGui::Text(std::to_string(material->resource_material->resource_diffuse->instances).data());

				ImGui::Text(std::to_string(material->resource_material->resource_diffuse->Texture_width).data());
				ImGui::SameLine();
				ImGui::Text(std::to_string(material->resource_material->resource_diffuse->Texture_height).data());

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