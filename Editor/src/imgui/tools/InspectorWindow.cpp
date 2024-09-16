#include "imgui/tools/InspectorWindow.h"

#include "imgui/ImGuiDefines.h"

namespace oasis
{
	namespace imgui
	{
		InspectorWindow::InspectorWindow() : BaseWindow(ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_INSPECTOR) + " Inspector", "Inspector")
		{ }

		void InspectorWindow::Render()
		{
		}

		bool InspectorWindow::WindowBegin()
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 5));
			return BaseWindow::WindowBegin();
		}

		void InspectorWindow::WindowEnd()
		{
			BaseWindow::WindowEnd();
			ImGui::PopStyleVar();
		}
	}
}