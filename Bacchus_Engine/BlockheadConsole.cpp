#include "Globals.h"
#include "BlockheadConsole.h"
#include "Application.h"


BlockheadConsole::BlockheadConsole(char* name) : Blockhead(name)
{
}

BlockheadConsole::~BlockheadConsole()
{
	Clear();
}

bool BlockheadConsole::Draw()
{

	ImGuiWindowFlags consoleFlags = 0;
	consoleFlags |= ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin(name, &enabled, consoleFlags))
	{
		if (ImGui::SmallButton("Clear"))
		{
			Clear();
		}

		ImGui::SameLine();

		filter.Draw("Filter", 200.0f);

		ImGui::Separator();

		ImGuiWindowFlags scrollFlags = 0;
		scrollFlags |= ImGuiWindowFlags_HorizontalScrollbar;
		scrollFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

		if (ImGui::BeginChild("Scrollbar", ImVec2(0, 0), false, scrollFlags))
		{
			// --- Print logs to console ---

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 1)); // Tighten spacing

			for (uint i = 0; i < App->GetLogs().size(); ++i)
			{
				const char* item = App->GetLogs().at(i).data();

				if (item[1] == *error_key)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75, 0, 0, 255));
				else
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 255, 255, 255));

				if (!filter.PassFilter(item))
				{
					ImGui::PopStyleColor();
					continue;
				}

				ImGui::TextUnformatted(item);


				ImGui::PopStyleColor();

			}
			ImGui::PopStyleVar();

		}

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();

	}

	ImGui::End();

	return true;
}

void BlockheadConsole::Clear()
{
	App->ClearLogsFromConsole();
}
