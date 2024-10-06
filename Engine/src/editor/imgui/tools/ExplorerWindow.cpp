#ifdef __EDITOR__

#include "editor/imgui/tools/ExplorerWindow.h"

#include "editor/imgui/ImGuiDefines.h"
#include "utils/string_extensions.h"
#include "system/FileLoader.h"
#include "editor/imgui/ImGuiWindow.h"
#include "imgui/imgui_helpers.h"
#include "assets/AssetDatabase.h"
#include "assets/Resource.h"

namespace oasis
{
	namespace imgui
	{
		ExplorerWindow::ExplorerWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_EXPLORER) + " Explorer", "Explorer")
		{ }

		std::vector<std::string> RESOURCE_ICONS =
		{
			ICON_FA_ASSET_CFG,
			ICON_FA_ASSET_SCENE,
			ICON_FA_ASSET_MAT,
			ICON_FA_ASSET_TEX,
			ICON_FA_ASSET_SPR,
			ICON_FA_ASSET_FONT,
			ICON_FA_ASSET_SND,
			ICON_FA_ASSET_SONG,
			ICON_FA_ASSET_VO,
			ICON_FA_ASSET_ANIM,
			ICON_FA_ASSET_LOC,
			ICON_FA_ASSET_MOD,
		};

		void ExplorerWindow::RenderResource(assets::Resource& a_Resource, bool a_RenderFoldersOnly, bool a_ShowSelf)
		{
			if (a_Resource.m_Name.empty() ||
				a_Resource.m_Path.empty() ||
				!a_Resource.m_Show)
			{
				return;
			}

			if (a_RenderFoldersOnly && a_Resource.m_ResourceType != assets::ResourceType::Folder)
			{
				return;
			}

			std::string name = a_Resource.m_Name;
			std::string icon = ICON_FA_FOLDER_OPEN;
			if (a_Resource.m_ResourceType == assets::ResourceType::Folder && a_Resource.m_FoldedOut)
			{
				icon = ICON_FA_FOLDER_OPEN;
			}
			else if (a_Resource.m_ResourceType == assets::ResourceType::Folder)
			{
				icon = ICON_FA_FOLDER;
			}
			std::string id = IMGUI_FORMAT_ID("", TREE_NODE_ID, string_extensions::StringToUpper(a_Resource.m_Path));

			bool clicked, right_clicked;
			if (a_ShowSelf)
			{
				if (a_Resource.m_ResourceType == assets::ResourceType::Folder)
				{
					if (!a_Resource.m_Resources.empty())
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
							for (auto& resource : a_Resource.m_Resources)
							{
								RenderResource(resource, a_RenderFoldersOnly);
							}
							ImGui::TreePop();
						}
					}
					else if (a_Resource.m_Resources.empty())
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
				for (auto& resource : a_Resource.m_Resources)
				{
					RenderResource(resource, a_RenderFoldersOnly);
				}
			}
		}

		void ExplorerWindow::Render()
		{
			auto createNewFolder = [](const assets::Resource& a_Folder)
			{
				std::string name = assets::Resource::GetUniqueName(a_Folder, "New Folder");
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
				RenderResource(assets::ASSETDATABASE_ROOT, true, true);
				ImGui::PopStyleVar();
			}
			x -= explorerWidth;
			ImGui::EndChild();

			if (m_NeedsRefresh && m_ExplorerRoot)
			{
				m_ResourcesToShow.clear();

				m_NeedsRefresh = false;

				std::string searchStringExplorer = string_extensions::StringToLower(std::string(SEARCHSTRING_EXPLORER));
				bool isEmptyString = searchStringExplorer.empty();

				for (auto& resource : m_ExplorerRoot->m_Resources)
				{
					if (isEmptyString || string_extensions::StringToLower(resource.m_Name).find(searchStringExplorer) != std::string::npos)
					{
						m_ResourcesToShow.push_back(&resource);
					}
				}
			}

			ImGui::SameLine();
			float explorer2Width = 0;
			if (m_SelectedResource2)
			{
				explorer2Width = x * 0.65f;
			}
			if (ImGui::BeginChild(IMGUI_FORMAT_ID("", CHILD_ID, "FILES_EXPLORER").c_str(), ImVec2(explorer2Width, 0), true, ImGuiChildFlags_ResizeX))
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

						bool clicked, right_clicked;
						ImGui::EngineResourceNode(
							IMGUI_FORMAT_ID("", SELECTABLE_ID, item->m_Name.c_str()).c_str(),
							item->m_ResourceType == assets::ResourceType::Folder ? ICON_FA_FOLDER : RESOURCE_ICONS[(int)item->GetAssetType()].c_str(),
							item->m_NameWithoutExtension.c_str(),
							item->m_ResourceType == assets::ResourceType::Folder ? "" : assets::AssetTypeToString(item->GetAssetType()).c_str(),
							clicked,
							right_clicked,
							m_SelectedResource2 == item,
							ImGui::GetContentRegionAvail(),
							0
						);
						if (clicked)
						{
							m_SelectedResource2 = item;
							memset(EXPLORER_INSPECTOR_RENAME_RESOURCE, 0, sizeof(EXPLORER_INSPECTOR_RENAME_RESOURCE));
							memcpy(EXPLORER_INSPECTOR_RENAME_RESOURCE, item->m_NameWithoutExtension.c_str(), item->m_NameWithoutExtension.size());
						}
						if (right_clicked)
						{
							m_SelectedResource = item;
							m_ShowContextMenu |= true;
						}
					}
				}
			}
			ImGui::EndChild();
			if (m_SelectedResource2 && m_SelectedResource2->m_ResourceType != assets::ResourceType::Folder)
			{
				ImGui::SameLine();
				if (ImGui::BeginChild(IMGUI_FORMAT_ID("", CHILD_ID, "FILES_INSPECTOR_EXPLORER").c_str(), ImVec2(0, 0), true, ImGuiChildFlags_ResizeX))
				{
					float startY = ImGui::GetCursorPosY();
					ImDrawList* draw_list = ImGui::GetWindowDrawList();

					ImVec2 header_pos = ImGui::GetCursorScreenPos();
					ImVec2 header_size = ImVec2(ImGui::GetContentRegionAvail().x, m_Window.FontSize() * 4); // Width, Height

					ImVec2 header_min = header_pos;
					ImVec2 header_max = ImVec2(header_pos.x + header_size.x, header_pos.y + header_size.y);

					draw_list->AddRectFilled(header_min, header_max, ImGui::ImColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Button)), 0);
					
					float sizeX = ImGui::GetItemRectSize().x;

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + m_Window.FontSize());
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + m_Window.FontSize());

					float x = ImGui::GetCursorPosX();
					ImGui::PushFont(m_Window.CapitalIcon());
					ImGui::Text(RESOURCE_ICONS[(int)m_SelectedResource2->GetAssetType()].c_str());
					ImGui::PopFont();

					ImGui::SameLine();
					ImGui::SetCursorPosX(x + 50);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 50);
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
					ImGui::InputText(IMGUI_FORMAT_ID("", INPUT_ID, "RENAME_OBJECT_INSPECTOR_EXPLORER").c_str(), EXPLORER_INSPECTOR_RENAME_RESOURCE, sizeof(EXPLORER_INSPECTOR_RENAME_RESOURCE));
					ImGui::PopStyleVar();

					ImGui::SetCursorPosY(startY);
					ImGui::InvisibleButton(IMGUI_FORMAT_ID("", BUTTON_ID, "INVISIBLE_BUTTON_INSPECTOR_EXPLORER").c_str(), ImVec2(sizeX, header_size.y));

					if (ImGui::BeginChild(IMGUI_FORMAT_ID("", CHILD_ID, "FILES_INSPECTOR_INNER_EXPLORER").c_str(), ImVec2(0, 0), true))
					{
						ImGui::DisplayHeader(m_Window.Bold(), "Type");
						ImGui::SameLine();
						switch (m_SelectedResource2->GetAssetType())
						{
								case assets::AssetType::Cfg:
								case assets::AssetType::Scene:
								case assets::AssetType::Material:
								case assets::AssetType::VO:
								case assets::AssetType::Animation:
								case assets::AssetType::Localization:
								case assets::AssetType::Model:
								case assets::AssetType::Prefab:
								{
									ImGui::Text(assets::AssetTypeToString(m_SelectedResource2->GetAssetType()).c_str());
									break;
								}
								case assets::AssetType::Texture:
								case assets::AssetType::Sprite:
								case assets::AssetType::Font:
								{
									int items[3] = {
										(int) assets::AssetType::Texture,
										(int) assets::AssetType::Sprite,
										(int) assets::AssetType::Font,
									};
									int current_item = 0;
									{
										if (m_SelectedResource2->GetAssetType() == assets::AssetType::Sprite)
										{
											current_item = 1;
										}
										else if (m_SelectedResource2->GetAssetType() == assets::AssetType::Font)
										{
											current_item = 2;
										}
									}
									ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));
									if (ImGui::BeginCombo(IMGUI_FORMAT_ID("", COMBO_ID, "ASSETTYPE_INSPECTOR_EXPLORER").c_str(), assets::AssetTypeToString((assets::AssetType)items[current_item]).c_str()))
									{
										for (int n = 0; n < IM_ARRAYSIZE(items); n++)
										{
											bool is_selected = (current_item == n);
											if (ImGui::Selectable(assets::AssetTypeToString((assets::AssetType)items[n]).c_str(), is_selected))
											{
												m_SelectedResource2->SetAssetType((assets::AssetType)items[n]);
											}

											if (is_selected)
											{
												ImGui::SetItemDefaultFocus();
											}
										}
										ImGui::EndCombo(); 
									}
									ImGui::PopStyleVar();
									break;
								}
								case assets::AssetType::Sound:
								case assets::AssetType::Song:
								{
									int items[2] = {
										(int)assets::AssetType::Sound,
										(int)assets::AssetType::Song,
									};
									int current_item = m_SelectedResource2->GetAssetType() == assets::AssetType::Sound ? 0 : 1;
									ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));
									if (ImGui::BeginCombo(IMGUI_FORMAT_ID("", COMBO_ID, "ASSETTYPE_INSPECTOR_EXPLORER").c_str(), assets::AssetTypeToString((assets::AssetType)items[current_item]).c_str()))
									{
										for (int n = 0; n < IM_ARRAYSIZE(items); n++)
										{
											bool is_selected = (current_item == n);
											if (ImGui::Selectable(assets::AssetTypeToString((assets::AssetType)items[n]).c_str(), is_selected))
											{
												m_SelectedResource2->SetAssetType((assets::AssetType)items[n]);
											}

											if (is_selected)
											{
												ImGui::SetItemDefaultFocus();
											}
										}
										ImGui::EndCombo();
									}
									ImGui::PopStyleVar();
									break;
								}
						}
					}

					ImGui::EndChild();
				}
				ImGui::EndChild();
			}

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

					if (m_SelectedResource->m_ResourceType != assets::ResourceType::Folder)
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
							assets::ASSETDATABASE_ROOT.Scan();
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
								assets::ASSETDATABASE_ROOT.Scan();
							}
						}
						ImGui::Separator();
						if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_ARROW_TURN_UP) + " Show in explorer", MENU_ITEM_ID, "SHOW_FOLDER_IN_EXPLORER").c_str()))
						{
							file::FileLoader::OpenInExplorer(m_SelectedResource->m_Parent->m_Path.c_str());
						}
						ImGui::Separator();
						if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_DELETE) + " Delete", MENU_ITEM_ID, "DELETE_FILE_EXPLORER").c_str()))
						{
							m_SelectedResource->Delete();
							assets::ASSETDATABASE_ROOT.Scan();
						}
					}
					ImGui::EndPopup();
				}
			}
		}
	}
}

#endif // __EDITOR__