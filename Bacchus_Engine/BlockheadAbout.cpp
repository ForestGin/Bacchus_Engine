#include "BlockheadAbout.h"
#include "Application.h"
#include "BacchusEditor.h"
#include "Imgui/imgui.h"
#include "SDL/include/SDL.h"

#include "OpenGL.h"

BlockheadAbout::BlockheadAbout(char* name) : Blockhead(name)
{
}

BlockheadAbout::~BlockheadAbout()
{
}

bool BlockheadAbout::Draw()
{
	ImGuiWindowFlags settingsFlags = 0;
	settingsFlags = ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin(name, &enabled, settingsFlags))
	{
		// --- Introduction ---
		ImGui::Separator();
		ImGui::Text("Bacchus Engine");
		ImGui::Text("Version 0.01 ");
		ImGui::Text("Initial steps into game engine development");
		ImGui::Text("By Oscar Larios and Pol Bosch (ForestGin)");

		// --- Libraries --- //REVISION
		ImGui::Text("Libraries");
		ImGui::Spacing();

		SDL_version version;
		SDL_GetVersion(&version);
		if (ImGui::Button("SDL")) { App->bacchuseditor->RequestBrowser("https://www.libsdl.org/"); }
		ImGui::SameLine(); ImGui::TextWrapped("%i.%i.%i", version.major, version.minor, version.patch);

		if (ImGui::Button("OpenGL")) { App->bacchuseditor->RequestBrowser("https://www.opengl.org/"); }
		ImGui::SameLine(); ImGui::TextWrapped("%s", glGetString(GL_VERSION));

		if (ImGui::Button("Glew")) { App->bacchuseditor->RequestBrowser("http://glew.sourceforge.net/"); }
		ImGui::SameLine(); ImGui::TextWrapped("%s", glewGetString(GLEW_VERSION));

		if (ImGui::Button("ImGui")) { App->bacchuseditor->RequestBrowser("https://github.com/ocornut/imgui"); }
		ImGui::SameLine(); ImGui::TextWrapped("%s", ImGui::GetVersion());

		if (ImGui::Button("MathGeoLib")) { App->bacchuseditor->RequestBrowser("https://github.com/juj/MathGeoLib"); }
		ImGui::SameLine(); ImGui::TextWrapped("1.5");

		if (ImGui::Button("JSON For Modern C++")) { App->bacchuseditor->RequestBrowser("https://github.com/nlohmann/json"); }
		ImGui::SameLine(); ImGui::TextWrapped("3.7.0");

		// --- License ---

		ImGui::TextWrapped("MIT License");
		ImGui::TextWrapped("Copyright(c) 2020 ForestGin");
		ImGui::TextWrapped("Permission is hereby granted, free of charge, to any person obtaining a copyof this software and associated documentation files(the <Software>), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions : ");
		ImGui::TextWrapped("The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.");
		ImGui::TextWrapped("THE SOFTWARE IS PROVIDED <AS IS>, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");

	}

	ImGui::End();

	return true;
}