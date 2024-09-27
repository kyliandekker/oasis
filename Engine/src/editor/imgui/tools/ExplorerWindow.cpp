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
		ExplorerWindow::ExplorerWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_EXPLORER) + " Explorer", "Explorer")
		{
			m_ExplorerResource.m_Path = "./assets";
			m_ExplorerResource.m_FoldedOut = true;
			m_ExplorerResource.Scan();

			m_ExplorerRoot = &m_ExplorerResource;
		}

		std::vector<std::string> RESOURCE_ICONS =
		{
			ICON_FA_CFG,
			ICON_FA_MAP,
			ICON_FA_MAT,
			ICON_FA_TEX,
			ICON_FA_SPR,
			ICON_FA_FONT,
			ICON_FA_SND,
			ICON_FA_SONG,
			ICON_FA_VO,
			ICON_FA_ANIM,
			ICON_FA_LOC,
			ICON_FA_MOD,
		};

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
							m_NeedsRefresh = true;
							m_ExplorerRoot = &a_Resource;
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
								RenderResource(resource, a_RenderFoldersOnly);
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
							m_NeedsRefresh = true;
							m_ExplorerRoot = &a_Resource;
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
						m_NeedsRefresh = true;
						m_ExplorerRoot = &a_Resource;
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
					RenderResource(resource, a_RenderFoldersOnly);
				}
			}
		}

		char SEARCHSTRING_EXPLORER[256] = { '\0' };
		std::string searchStringExplorer;
		void ExplorerWindow::Render()
		{
			auto createNewFolder = [](const imgui::ExplorerResource& a_Folder)
			{
				std::string name = ExplorerResource::GetUniqueName(a_Folder, "New Folder");
				bool success = file::FileLoader::CreateFolder(a_Folder.m_Path + "/" + name);
				return success;
			};

			m_ShowContextMenu = false;

			float windowWidth = ImGui::GetContentRegionAvail().x;
			if (ImGui::SearchBar(m_Window.FontSize(), IMGUI_FORMAT_ID("", INPUT_ID, "SEARCH_FILES_EXPLORER").c_str(), SEARCHSTRING_EXPLORER, sizeof(SEARCHSTRING_EXPLORER), ImVec2(windowWidth, m_Window.FontSize())))
			{
				m_NeedsRefresh = true;
			}

			float x = ImGui::GetContentRegionAvail().x;
			float explorerWidth = x * 0.15f;
			if (ImGui::BeginChild(IMGUI_FORMAT_ID("", CHILD_ID, "DIRECTORIES_EXPLORER").c_str(), ImVec2(explorerWidth, 0), true, ImGuiChildFlags_ResizeX))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				RenderResource(m_ExplorerResource, true, true);
				ImGui::PopStyleVar();
			}
			x -= explorerWidth;
			ImGui::EndChild();

			if (m_NeedsRefresh && m_ExplorerRoot)
			{
				m_ResourcesToShow.clear();

				m_NeedsRefresh = false;

				searchStringExplorer = string_extensions::StringToLower(std::string(SEARCHSTRING_EXPLORER));
				bool isEmptyString = searchStringExplorer.empty();

				for (auto& resource : m_ExplorerRoot->m_ExplorerResources)
				{
					if (isEmptyString || string_extensions::StringToLower(resource.m_Name).find(searchStringExplorer) != std::string::npos)
					{
						m_ResourcesToShow.push_back(&resource);
					}
				}
			}

			ImGui::SameLine();
			if (ImGui::BeginChild(IMGUI_FORMAT_ID("", CHILD_ID, "FILES_EXPLORER").c_str(), ImVec2(0, 0), true, ImGuiChildFlags_ResizeX))
			{
				if (m_ExplorerRoot)
				{
					ImVec4 textColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
					textColor.w = 0.5f;
					for (size_t i = 0; i < m_ExplorerRoot->m_ParentNames.size(); i++)
					{
						ImGui::TextColored(textColor, m_ExplorerRoot->m_ParentNames[i].c_str());
						if (i < m_ExplorerRoot->m_ParentNames.size() - 1)
						{
							ImGui::SameLine();
							ImGui::TextColored(textColor, ICON_FA_ARROW_RIGHT);
							ImGui::SameLine();
						}
					}

					ImGui::Separator();

					for (auto& item : m_ResourcesToShow)
					{
						if (!item)
						{
							continue;
						}

						ImVec2 pos = ImGui::GetCursorScreenPos();

						ImGui::SetCursorScreenPos(ImVec2(pos.x + 10, pos.y));  // Set label position
						if (item->m_ExplorerResourceType == ExplorerResourceType::Folder)
						{
							ImGui::Text(ICON_FA_FOLDER);
						}
						else
						{
							ImGui::Text(RESOURCE_ICONS[(int)item->m_AssetType].c_str());
						}
						ImGui::SetCursorScreenPos(ImVec2(pos.x + 35, pos.y));  // Set label position
						ImGui::Text(item->m_Name.c_str());
					}
				}
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