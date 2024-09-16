#include "imgui/tools/MainWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <string>

#include "utils/abstractions.h"
#include "utils/string_extensions.h"
#include "system/Logger.h"
#include "imgui/ImGuiDefines.h"
#include "settings/StaticSettings.h"
#include "project/Project.h"

namespace oasis
{
	namespace imgui
	{
		MainWindow::MainWindow() : BaseWindow(ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking, "", "DockSpace", true)
		{ }

		void OpenProject()
		{
			std::string path;
			if (abstractions::PickContainer(path))
			{
				project::LoadProject(path);
				LOGF(logger::LOGSEVERITY_INFO, "Successfully opened project: \"%s\"", path.c_str());
			}
		}

		void MainWindow::Render()
		{
			ImGui::DockSpace(ImGui::GetID("DockSpace"));

			ImGui::BeginMainMenuBar();
			if (ImGui::BeginMenu(IMGUI_FORMAT_ID("File", MENU_ID, "FILE").c_str()))
			{
				if (ImGui::MenuItem(IMGUI_FORMAT_ID("Open Project", MENU_ITEM_ID, "OPEN_PROJECT").c_str(), "CTRL+O"))
				{
					OpenProject();
				}
				else if (ImGui::BeginMenu(IMGUI_FORMAT_ID("Recent Projects", MENU_ID, "RECENT_PROJECTS").c_str()))
				{
					if (project::settings.GetPreviousProjects().empty())
					{
						ImGui::MenuItem(IMGUI_FORMAT_ID("No previous projects", MENU_ITEM_ID, "NO_PREVIOUS_PROJECTS").c_str(), "", false, false);
					}
					else
					{
						size_t i = 0;
						for (auto& location : project::settings.GetPreviousProjects())
						{
							if (ImGui::MenuItem(IMGUI_FORMAT_ID(location, MENU_ITEM_ID, std::string(std::to_string(i) + "_" + string_extensions::StringToUpper(location).c_str())).c_str()))
							{
								project::project.Load(location);
							}
							i++;
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			else if (ImGui::BeginMenu(IMGUI_FORMAT_ID("View", MENU_ID, "VIEW").c_str()))
			{
				ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);

				ImGui::PopItemFlag();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}