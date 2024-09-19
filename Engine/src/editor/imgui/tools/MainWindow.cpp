#ifdef __EDITOR__

#include "editor/imgui/tools/MainWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <string>

#include "utils/abstractions.h"
#include "utils/string_extensions.h"
#include "system/Logger.h"
#include "editor/imgui/ImGuiDefines.h"
#include "editor/settings/EditorSettings.h"

namespace oasis
{
	namespace imgui
	{
		MainWindow::MainWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking, "", "DockSpace", true)
		{ }

		void MainWindow::Render()
		{
			ImGui::DockSpace(ImGui::GetID("DockSpace"));

			ImGui::BeginMainMenuBar();
			ImGui::EndMainMenuBar();
		}
	}
}

#endif // __EDITOR__