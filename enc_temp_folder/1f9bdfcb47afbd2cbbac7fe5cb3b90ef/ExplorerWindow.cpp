#ifdef __EDITOR__

#include "editor/imgui/tools/ExplorerWindow.h"

#include "editor/imgui/ImGuiDefines.h"
#include "utils/string_extensions.h"
#include "system/FileLoader.h"
#include "editor/imgui/ImGuiWindow.h"
#include "imgui/imgui_helpers.h"

namespace oasis
{
	namespace imgui
	{
		ExplorerWindow::ExplorerWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_FOLDER) + " Explorer", "Explorer")
		{
			m_ExplorerResource.m_Path = "./assets";
			m_ExplorerResource.m_FoldedOut = true;
			m_ExplorerResource.Scan();

			m_ExplorerResourceWindow2 = &m_ExplorerResource;
		}

		void ExplorerWindow::RenderResource(ExplorerResource& a_Resource, bool a_RenderFoldersOnly, bool a_ShowSelf)
		{
			if (a_Resource.m_Name.empty() ||
				a_Resource.m_Path.empty() ||
				!a_Resource.m_Show)
			{
				return;
			}

			if (a_RenderFoldersOnly && a_Resource.m_ExplorerResourceType != ExplorerResourceType::Folder)
			{
				return;
			}

			std::string name = a_Resource.m_Name;
			std::string icon = ICON_FA_FOLDER_OPEN;
			if (a_Resource.m_ExplorerResourceType == imgui::ExplorerResourceType::Folder && a_Resource.m_FoldedOut)
			{
				icon = ICON_FA_FOLDER_OPEN;
			}
			else if (a_Resource.m_ExplorerResourceType == imgui::ExplorerResourceType::Folder)
			{
				icon = ICON_FA_FOLDER;
			}
			std::string id = IMGUI_FORMAT_ID("", TREE_NODE_ID, string_extensions::StringToUpper(a_Resource.m_Path));

			bool clicked, right_clicked;
			if (a_ShowSelf)
			{
				if (a_Resource.m_ExplorerResourceType == imgui::ExplorerResourceType::Folder)
				{
					if (a_Resource.m_HasChildren)
					{
						ImGui::SetNextItemOpen(a_Resource.m_FoldedOut);
						const bool fold = ImGui::EngineTreeNodeExS(id.c_str(), icon.c_str(), name.c_str(), clicked , right_clicked, &a_Resource == m_SelectedResource, ImVec2(ImGui::GetContentRegionAvail().x, m_Window.FontSize()), ImGuiTreeNodeFlags_OpenOnArrow);

						if (right_clicked)
						{
							m_ShowContextMenu |= true;
							m_SelectedResource = &a_Resource;
						}

						if (clicked)
						{
							m_ExplorerResourceWindow2 = &a_Resource;
							m_SelectedResource = &a_Resource;
						}

						if (a_Resource.m_FoldedOut != fold)
						{
							a_Resource.m_FoldedOut = fold;
							if (fold)
							{
								a_Resource.Scan();
							}
						}
						if (a_Resource.m_FoldedOut)
						{
							for (auto& resource : a_Resource.m_ExplorerResources)
							{
								RenderResource(resource);
							}
							ImGui::TreePop();
						}
					}
					else if (!a_Resource.m_HasChildren)
					{
						const bool opened = ImGui::EngineTreeNodeExS(id.c_str(), icon.c_str(), name.c_str(), clicked, right_clicked, &a_Resource == m_SelectedResource, ImVec2(ImGui::GetContentRegionAvail().x, m_Window.FontSize()), ImGuiTreeNodeFlags_Leaf);

						if (right_clicked)
						{
							m_ShowContextMenu |= true;
							m_SelectedResource = &a_Resource;
						}

						if (clicked)
						{
							m_ExplorerResourceWindow2 = &a_Resource;
							m_SelectedResource = &a_Resource;
						}

						if (opened)
						{
							ImGui::TreePop();
						}
					}
				}
				else
				{
					bool opened = false;
					opened = ImGui::TreeNodeEx(id.c_str(), ImGuiTreeNodeFlags_Leaf);

					if (ImGui::IsItemHovered() && ImGui::IsItemClicked(1))
					{
						m_ShowContextMenu |= true;
						m_SelectedResource = &a_Resource;
					}

					if (ImGui::IsItemHovered() && ImGui::IsItemClicked(0))
					{
						m_ExplorerResourceWindow2 = &a_Resource;
					}

					if (opened)
					{
						ImGui::TreePop();
					}
				}
			}
			else
			{
				for (auto& resource : a_Resource.m_ExplorerResources)
				{
					RenderResource(resource);
				}
			}
		}

		void ExplorerWindow::Render()
		{
			auto createNewFolder = [](const imgui::ExplorerResource& a_Folder)
			{
				std::string name = ExplorerResource::GetUniqueName(a_Folder, "New Folder");
				bool success = file::FileLoader::CreateFolder(a_Folder.m_Path + "/" + name);
				return success;
			};

			m_ShowContextMenu = false;

			float x = ImGui::GetContentRegionAvail().x;
			float explorerWidth = x * 0.15f;
			if (ImGui::BeginChild("Test", ImVec2(explorerWidth, 0)))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				RenderResource(m_ExplorerResource, true, true);
				ImGui::PopStyleVar();
			}
			x -= explorerWidth;
			ImGui::EndChild();
			ImGui::SameLine();
			if (ImGui::BeginChild("Fsad", ImVec2(x, 0)))
			{
				ImGui::Text("aah");
			}
			ImGui::EndChild();

			if (m_ShowContextMenu)
			{
				ImGui::OpenPopup(IMGUI_FORMAT_ID("", POPUP_WINDOW_ID, "RESOURCE_OPTIONS_EXPLORER").c_str());
			}

			if (m_SelectedResource)
			{
				if (ImGui::BeginPopup(IMGUI_FORMAT_ID("", POPUP_WINDOW_ID, "RESOURCE_OPTIONS_EXPLORER").c_str()))
				{
					ImVec4 textColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
					textColor.w = 0.5f;
					ImGui::TextColored(textColor, m_SelectedResource->m_Name.c_str());

					if (m_SelectedResource->m_ExplorerResourceType != ExplorerResourceType::Folder)
					{
						ImGui::Separator();
						if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_ARROW_TURN_UP) + " Show in explorer", MENU_ITEM_ID, "SHOW_FILE_IN_EXPLORER_EXPLORER").c_str()))
						{
							file::FileLoader::OpenInExplorer(m_SelectedResource->m_Parent->m_Path.c_str());
						}
						ImGui::Separator();
						if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_DELETE) + " Delete note", MENU_ITEM_ID, "DELETE_FILE_EXPLORER").c_str()))
						{
							m_SelectedResource->Delete();
							m_ExplorerResource.Scan();
						}
					}
					else
					{
						ImGui::Separator();

						if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_FOLDER_PLUS) + " New folder", MENU_ITEM_ID, "CREATE_NEW_FOLDER").c_str()))
						{
							if (createNewFolder(*m_SelectedResource))
							{
								m_SelectedResource->m_FoldedOut = true;
								m_ExplorerResource.Scan();
							}
						}
						ImGui::Separator();
						if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_ARROW_TURN_UP) + " Show in explorer", MENU_ITEM_ID, "SHOW_FOLDER_IN_EXPLORER").c_str()))
						{
							file::FileLoader::OpenInExplorer(m_SelectedResource->m_Parent->m_Path.c_str());
						}
						ImGui::Separator();
						if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_DELETE) + " Delete note", MENU_ITEM_ID, "DELETE_FILE_EXPLORER").c_str()))
						{
							m_SelectedResource->Delete();
							m_ExplorerResource.Scan();
						}
					}
					ImGui::EndPopup();
				}
			}
		}
	}
}

#endif // __EDITOR__