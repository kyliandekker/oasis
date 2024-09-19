#ifdef __EDITOR__

#include "editor/imgui/tools/ConsoleWindow.h"

#include <imgui/imgui_helpers.h>

#include "editor/imgui/ImGuiDefines.h"
#include "editor/settings/EditorSettings.h"

namespace oasis
{
	namespace imgui
	{
		ConsoleWindow::ConsoleWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_CONSOLE) + " Console", "Console")
		{ }

		ConsoleWindow::~ConsoleWindow()
		{
		}

		char logo_arr[7][10] =
		{
			ICON_FA_INFO,
			ICON_FA_WARNING,
			ICON_FA_ERROR,
			ICON_FA_ASSERT,
			ICON_FA_SUCCESS,
			ICON_FA_INFO_SUCCESS,
			ICON_FA_AWESOME
		};

		ImVec4 colors_arr[7] =
		{
			ImGui::ConvertColorsRgba(0, 148, 255, 255),
			ImGui::ConvertColorsRgba(255, 216, 0, 255),
			ImGui::ConvertColorsRgba(255, 0, 0, 255),
			ImGui::ConvertColorsRgba(255, 0, 0, 255),
			ImGui::ConvertColorsRgba(76, 255, 0, 255),
			ImGui::ConvertColorsRgba(76, 255, 0, 255),
			ImGui::ConvertColorsRgba(255, 110, 220, 255),
		};

		void ConsoleWindow::Render()
		{
			// Filter messages if need be.
			if (m_NeedsRefresh)
			{
				std::vector<bool> filters =
				{
					settings::settings.Info(),
					settings::settings.Warning(),
					settings::settings.Error(),
					settings::settings.Assert(),
					settings::settings.Success(),
					settings::settings.InfoSuccess(),
					settings::settings.Awesome(),
				};

				// Mutex to ensure new messages cannot be added.
				std::lock_guard<std::mutex> lock(MESSAGE_MUTEX);

				m_NeedsRefresh = false;
				m_FilteredMessages.clear();

				for (size_t i = 0; i < m_Messages.size(); i++)
				{
					auto& message = m_Messages[i];
					if (filters[(int)message.a_Severity])
					{
						m_FilteredMessages.push_back(i);
					}
				}
			}

			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			ImVec2 button_pos = ImGui::GetCursorScreenPos();
			ImVec2 button_size = ImVec2(ImGui::GetContentRegionAvail().x, 32); // Width, Height

			ImVec2 button_min = button_pos;
			ImVec2 button_max = ImVec2(button_pos.x + button_size.x, button_pos.y + button_size.y);

			draw_list->AddRectFilled(button_min, button_max, ImGui::ImColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Button)), 0);  // 10.0f is the rounding radius

			if (ImGui::TransparentButton(IMGUI_FORMAT_ID(std::string(ICON_FA_ERASE) + " Clear", BUTTON_ID, "CLEAR_CONSOLE").c_str()))
			{
				std::lock_guard<std::mutex> lock(MESSAGE_MUTEX);
				m_FilteredMessages.clear();
				m_Messages.clear();
			}
			ImGui::SameLine();
			bool scrollToBottom = settings::settings.ScrollToBottom();
			if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_DOWN_TO_BOTTOM) + " Scroll To Bottom", BUTTON_ID, "SCROLL_TO_BOTTOM_CONSOLE").c_str(), &scrollToBottom))
			{
				settings::settings.SetScrollToBottom(scrollToBottom);
				m_NeedsRefresh = true;
			}

			ImGui::SameLine();
			bool info = settings::settings.Info();
			if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_INFO), BUTTON_ID, "SHOW_INFO_CONSOLE").c_str(), &info, ImVec2(), colors_arr[logger::LOGSEVERITY_INFO]))
			{
				settings::settings.SetInfo(info);
				m_NeedsRefresh = true;
			}

			ImGui::SameLine();
			bool warning = settings::settings.Warning();
			if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_WARNING), BUTTON_ID, "SHOW_WARNING_CONSOLE").c_str(), &warning, ImVec2(), colors_arr[logger::LOGSEVERITY_WARNING]))
			{
				settings::settings.SetWarning(warning);
				m_NeedsRefresh = true;
			}

			ImGui::SameLine();
			bool error = settings::settings.Error();
			if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_ERROR), BUTTON_ID, "SHOW_ERROR_CONSOLE").c_str(), &error, ImVec2(), colors_arr[logger::LOGSEVERITY_ERROR]))
			{
				settings::settings.SetError(error);
				m_NeedsRefresh = true;
			}

			ImGui::SameLine();
			bool assert = settings::settings.Assert();
			if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_ASSERT), BUTTON_ID, "SHOW_ASSERT_CONSOLE").c_str(), &assert, ImVec2(), colors_arr[logger::LOGSEVERITY_ASSERT]))
			{
				settings::settings.SetAssert(assert);
				m_NeedsRefresh = true;
			}

			ImGui::SameLine();
			bool success = settings::settings.Success();
			if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_SUCCESS), BUTTON_ID, "SHOW_SUCCESS_CONSOLE").c_str(), &success, ImVec2(), colors_arr[logger::LOGSEVERITY_SUCCESS]))
			{
				settings::settings.SetSuccess(success);
				m_NeedsRefresh = true;
			}

			ImGui::SameLine();
			bool infoSuccess = settings::settings.InfoSuccess();
			if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_INFO_SUCCESS), BUTTON_ID, "SHOW_INFO_SUCCESS_CONSOLE").c_str(), &infoSuccess, ImVec2(), colors_arr[logger::LOGSEVERITY_INFO_SUCCESS]))
			{
				settings::settings.SetInfoSuccess(infoSuccess);
				m_NeedsRefresh = true;
			}

			ImGui::SameLine();
			bool awesome = settings::settings.Awesome();
			if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_AWESOME), BUTTON_ID, "SHOW_AWESOME_CONSOLE").c_str(), &awesome, ImVec2(), colors_arr[logger::LOGSEVERITY_AWESOME]))
			{
				settings::settings.SetAwesome(awesome);
				m_NeedsRefresh = true;
			}

			if (ImGui::BeginChild("ScrollRegion##", ImVec2(0, ImGui::GetContentRegionAvail().y), false, 0))
			{
				static const float timestamp_width = ImGui::CalcTextSize("00:00:00:0000").x;    // Timestamp.
				int count = 0;

				ImGui::PushTextWrapPos();
				std::lock_guard<std::mutex> lock(MESSAGE_MUTEX);
				for (int i : m_FilteredMessages)
				{
					auto& message = m_Messages[i];

					ImVec4 color = colors_arr[(int)message.a_Severity];
					ImGui::TextColored(color, std::string(logo_arr[(int)message.a_Severity]).c_str());
					ImGui::SameLine();
					ImGui::TextUnformatted(message.m_RawMessage.c_str());

					time_t time_t = std::chrono::system_clock::to_time_t(message.m_Time);
					struct tm buf;

					localtime_s(&buf, &time_t);

					std::string s(30, '\0');
					std::strftime(&s[0], s.size(), "%Y-%m-%d %H:%M:%S", &buf);

					ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), s.c_str());
					ImGui::SameLine(160);
					ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), message.m_Location.c_str());

					ImGui::Separator();

					if (scrollToBottom)
					{
						ImGui::SetScrollHereY(1.0f);
					}
				}

				ImGui::PopTextWrapPos();
			}

			ImGui::EndChild();
		}

		void ConsoleWindow::Clear()
		{

		}

		void ConsoleWindow::AddMessage(const logger::Message& a_Message)
		{
			std::lock_guard<std::mutex> lock(MESSAGE_MUTEX);
			m_Messages.push_back(a_Message);
			m_NeedsRefresh = true;
		}

		void ConsoleWindow::LoggerCallback(const logger::Message& a_Message)
		{
			AddMessage(a_Message);
		}
	}
}

#endif // __EDITOR__