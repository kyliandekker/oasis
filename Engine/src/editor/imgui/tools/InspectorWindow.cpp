#ifdef __EDITOR__

#include "editor/imgui/tools/InspectorWindow.h"

#include <imgui/imgui_helpers.h>

#include "editor/imgui/ImGuiDefines.h"
#include "system/Logger.h"
#include "editor/imgui/ImGuiWindow.h"

namespace oasis
{
	namespace imgui
	{
		char RENAMETEXT[256] = "GameObject";
		InspectorWindow::InspectorWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_INSPECTOR) + " Inspector", "Inspector")
		{ }

		bool temp = false;
		void InspectorWindow::Render()
		{
			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			ImVec2 header_pos = ImGui::GetCursorScreenPos();
			ImVec2 header_size = ImVec2(ImGui::GetContentRegionAvail().x, 32 * 2); // Width, Height

			ImVec2 header_min = header_pos;
			ImVec2 header_max = ImVec2(header_pos.x + header_size.x, header_pos.y + header_size.y);

			draw_list->AddRectFilled(header_min, header_max, ImGui::ImColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Button)), 0);

			float y = ImGui::GetCursorPosY();
			float x = ImGui::GetCursorPosX() + m_Window.FontSize();
			ImGui::SetCursorPos(ImVec2(x, y + m_Window.FontSize()));
			ImGui::PushFont(m_Window.CapitalIcon());
			ImGui::Text(ICON_FA_CUBE);
			ImGui::PopFont();

			ImGui::SetCursorPosY(y + (m_Window.FontSize() / 2));
			ImGui::SetCursorPosX(x + (m_Window.FontSize() * 3));
			ImGui::CheckboxText(IMGUI_FORMAT_ID("", CHECKBOX_ID, m_Name.c_str()).c_str(), &temp, ICON_FA_CHECKMARK_CHECKED, ICON_FA_CHECKMARK, ImVec2(m_Window.FontSize(), m_Window.FontSize()));

			ImGui::SameLine();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 50);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::InputText(IMGUI_FORMAT_ID("", INPUT_ID, "RENAME_OBJECT_INSPECTOR").c_str(), RENAMETEXT, sizeof(RENAMETEXT));
			ImGui::PopStyleVar();

			ImGui::SetCursorPosY(y);
			if (ImGui::InvisibleButton(IMGUI_FORMAT_ID(ICON_FA_ADD, BUTTON_ID, "ADD_MENU_HIERARCHY").c_str(), header_size))
			{ }



			ImGui::Separator();







			if (ImGui::Button("Info"))
			{
				LOGF(logger::LOGSEVERITY_INFO, "This is an info message.");
			}
			ImGui::SameLine();
			if (ImGui::Button("Warning"))
			{
				LOGF(logger::LOGSEVERITY_WARNING, "This is a warning message.");
			}
			ImGui::SameLine();
			if (ImGui::Button("Error"))
			{
				LOGF(logger::LOGSEVERITY_ERROR, "This is an error message.");
			}
			if (ImGui::Button("Assert"))
			{
				LOGF(logger::LOGSEVERITY_ASSERT, "This is an assert message.");
			}
			ImGui::SameLine();
			if (ImGui::Button("Success"))
			{
				LOGF(logger::LOGSEVERITY_SUCCESS, "This is a success message.");
			}
			ImGui::SameLine();
			if (ImGui::Button("Info Success"))
			{
				LOGF(logger::LOGSEVERITY_INFO_SUCCESS, "This is an info success message.");
			}
			ImGui::SameLine();
			if (ImGui::Button("Awesome"))
			{
				LOGF(logger::LOGSEVERITY_AWESOME, "This is an awesome message.");
			}
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

#endif // __EDITOR__