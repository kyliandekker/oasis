#ifdef __EDITOR__

#include "editor/imgui/tools/SceneWindow.h"

#include <imgui/imgui_helpers.h>

#include "editor/imgui/ImGuiDefines.h"
#include "editor/imgui/ImGuiWindow.h"
#include "gameplay/EntityComponentSystem.h"

namespace oasis
{
	namespace imgui
	{
		SceneWindow::SceneWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_SCENE) + " Scene", "Scene")
		{ }

		void SceneWindow::Render()
		{
			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			ImVec2 header_pos = ImGui::GetCursorScreenPos();
			ImVec2 header_size = ImVec2(ImGui::GetContentRegionAvail().x, m_Window.FontSize() * 2); // Width, Height

			ImVec2 header_min = header_pos;
			ImVec2 header_max = ImVec2(header_pos.x + header_size.x, header_pos.y + header_size.y);

			draw_list->AddRectFilled(header_min, header_max, ImGui::ImColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Button)), 0);

			float x = ImGui::GetContentRegionAvail().x / 2 - (header_size.y / 2);

			ImGui::SetCursorPosX(x);

			if (ImGui::TransparentButton(IMGUI_FORMAT_ID(gameplay::ECS.IsPaused() ? ICON_FA_PLAY : ICON_FA_PAUSE, BUTTON_ID, "PLAYPAUSE_SCENE").c_str(), ImVec2(header_size.y, header_size.y)))
			{
				gameplay::ECS.SetPaused(!gameplay::ECS.IsPaused());
			}
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

#endif // __EDITOR__