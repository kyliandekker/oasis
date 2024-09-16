#include "imgui/tools/SceneWindow.h"

#include "imgui/ImGuiDefines.h"

namespace oasis
{
	namespace imgui
	{
		SceneWindow::SceneWindow() : BaseWindow(ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_SCENE) + " Scene", "Scene")
		{ }

		void SceneWindow::Render()
		{
		}

		bool SceneWindow::WindowBegin()
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 5));
			return BaseWindow::WindowBegin();
		}

		void SceneWindow::WindowEnd()
		{
			BaseWindow::WindowEnd();
			ImGui::PopStyleVar();
		}
	}
}