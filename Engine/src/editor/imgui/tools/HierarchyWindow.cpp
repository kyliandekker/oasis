#ifdef __EDITOR__

#include "editor/imgui/tools/HierarchyWindow.h"

#include "editor/imgui/ImGuiWindow.h"
#include "editor/imgui/ImGuiDefines.h"
#include "imgui/imgui_helpers.h"
#include "system/Logger.h"
#include "utils/string_extensions.h"

namespace oasis
{
	namespace imgui
	{
		HierarchyItemView::HierarchyItemView()
		{ }

		HierarchyItemView::HierarchyItemView(const std::string& a_Icon, const std::string& a_Name, int a_ID) : m_Icon(a_Icon), m_Name(a_Name), m_ID(a_ID)
		{ }

		// TODO: Move this to another class. This needs to be used by various other windows like the inspector, explorer, etc.
		int SELECTED_HIERARCHY_ITEM = -1;
		char SEARCHSTRING[256] = { '\0' };
		std::string searchString;

		void HierarchyItemView::Render(HierarchyWindow& a_Window)
		{
			const float fontSize = a_Window.Window().FontSize();

			float initialX = ImGui::GetCursorPosX();

			float x = ImGui::GetCursorPosX() + 5.0f;
			ImGui::SetCursorPosX(x);
			bool tempVar = temp;
			ImGui::CheckboxText(IMGUI_FORMAT_ID("", CHECKBOX_ID, m_Name.c_str()).c_str(), &tempVar, ICON_FA_CHECKMARK_CHECKED, ICON_FA_CHECKMARK, ImVec2(fontSize, fontSize));

			ImGui::SameLine();

			if (!temp)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25f);
			}

			x = x + 25.0f;
			ImGui::SetCursorPosX(x);
			// TODO: This needs to only be visible if the object has children.
			//if (a_Index % 2 == 0)
			//{
			//	ImGui::CheckboxText(IMGUI_FORMAT_ID("", FOLD_ID, m_Name.c_str()).c_str(), &m_FoldedIn, ICON_FA_FOLDED_OUT, ICON_FA_FOLDED_IN, ImVec2(fontSize, fontSize));
			//	ImGui::SameLine();
			//}

			x = x + 15.0f;
			ImGui::SetCursorPosX(x);
			ImGui::Text(m_Icon.c_str());
			ImGui::SameLine();
			ImGui::SetCursorPosX(x + 25.0f);
			if (a_Window.IsRenaming() && SELECTED_HIERARCHY_ITEM == m_ID)
			{
				char renameText[256];
				strcpy_s(renameText, sizeof(renameText), m_Name.c_str());

				int status = ImGui::InvisInputText(IMGUI_FORMAT_ID("", INPUT_ID, m_Name.c_str()).c_str(), renameText, sizeof(renameText), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EnterReturnsTrue);
				if (status == 1 || status == 2)
				{
					m_Name = renameText;
					a_Window.SetIsRenaming(false);
				}
			}
			else
			{
				ImGui::Text(m_Name.c_str());
			}

			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			ImVec2 item_pos = ImGui::GetItemRectMin();
			ImVec2 item_size = ImGui::GetItemRectSize();

			const ImVec2 button_pos = ImVec2(initialX, item_pos.y);
			ImVec2 button_size = ImVec2(ImGui::GetContentRegionAvail().x, item_size.y);

			if (!temp)
			{
				ImGui::PopStyleVar();
			}

			ImGui::SetCursorScreenPos(button_pos); // Move cursor to where you want the button
			if (ImGui::HierarchyItem(IMGUI_FORMAT_ID("", DRAG_ITEM_ID, m_Name.c_str()).c_str(), button_size, SELECTED_HIERARCHY_ITEM == m_ID))
			{ }

			// Right click.
			if (ImGui::IsItemHovered() && ImGui::IsItemClicked(1))
			{
				// If the window is renaming but the item clicked is not being renamed, stop the renaming process.
				if (a_Window.IsRenaming() && SELECTED_HIERARCHY_ITEM != m_ID)
				{
					a_Window.SetIsRenaming(false);
				}

				a_Window.SetShowContextMenu(true);
				SELECTED_HIERARCHY_ITEM = m_ID;
			}

			// Left click.
			if (ImGui::IsItemHovered() && ImGui::IsItemClicked(0))
			{
				// If the window is renaming but the item clicked is not being renamed, stop the renaming process.
				if (a_Window.IsRenaming() && SELECTED_HIERARCHY_ITEM != m_ID)
				{
					a_Window.SetIsRenaming(false);
				}

				SELECTED_HIERARCHY_ITEM = m_ID;
			}

			if (ImGui::IsWindowFocused())
			{
				if (SELECTED_HIERARCHY_ITEM == m_ID)
				{
					if (ImGui::IsKeyPressed(ImGuiKey_Delete))
					{
						DEBUG_ITEMS.erase(DEBUG_ITEMS.begin() + SELECTED_HIERARCHY_ITEM);
					}

					if (ImGui::IsKeyPressed(ImGuiKey_F2))
					{
						a_Window.SetIsRenaming(true);
					}

					if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_D))
					{
						// TODO: Duplicate.
					}

					if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_C))
					{
						// TODO: Copy.
					}

					if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_V))
					{
						// TODO: Paste.
					}
				}
			}

			temp = tempVar;
		}

		std::string HierarchyItemView::Name() const
		{
			return m_Name;
		}

		std::string HierarchyItemView::Icon() const
		{
			return m_Icon;
		}

		int HierarchyItemView::ID() const
		{
			return m_ID;
		}

		HierarchyWindow::HierarchyWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_HIERARCHY) + " Hierarchy", "Hierarchy")
		{
			DEBUG_ITEMS.push_back(HierarchyItemView(ICON_FA_CAMERA, "Main Camera", 0));
			DEBUG_ITEMS.push_back(HierarchyItemView(ICON_FA_CUBE, "GameObject", 1));
			DEBUG_ITEMS.push_back(HierarchyItemView(ICON_FA_PARTICLE_SYSTEM, "Particle System", 2));
			DEBUG_ITEMS.push_back(HierarchyItemView(ICON_FA_LIGHT, "Environment Light", 3));
		}

		void HierarchyWindow::Render()
		{
			if (m_NeedsRefresh)
			{
				m_FilteredItems.clear();

				m_NeedsRefresh = false;

				searchString = string_extensions::StringToLower(std::string(SEARCHSTRING));
				bool isEmptyString = searchString.empty();

				for (auto& item : DEBUG_ITEMS)
				{
					if (isEmptyString || string_extensions::StringToLower(item.Name()).find(searchString) != std::string::npos)
					{
						m_FilteredItems.push_back(&item);
					}
				}
			}

			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			// Define the position and size of your button
			ImVec2 button_pos = ImGui::GetCursorScreenPos();
			ImVec2 button_size = ImVec2(ImGui::GetContentRegionAvail().x, 32); // Width, Height

			// Calculate the rectangle for the button
			ImVec2 button_min = button_pos;
			ImVec2 button_max = ImVec2(button_pos.x + button_size.x, button_pos.y + button_size.y);

			draw_list->AddRectFilled(button_min, button_max, ImGui::ImColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Button)), 0);  // 10.0f is the rounding radius

			if (ImGui::TransparentButton(IMGUI_FORMAT_ID(ICON_FA_ADD, BUTTON_ID, "ADD_MENU_HIERARCHY").c_str()))
			{ }

			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 100);
			float windowWidth = ImGui::GetContentRegionAvail().x - 25;

			if (ImGui::SearchBar(m_Window.FontSize(), IMGUI_FORMAT_ID("", INPUT_ID, "SEARCH_ENTITIES_HIERARCHY").c_str(), SEARCHSTRING, sizeof(SEARCHSTRING), ImVec2(windowWidth, 0)))
			{
				m_NeedsRefresh = true;
			}

			if (ImGui::BeginChild(IMGUI_FORMAT_ID("", CHILD_ID, "ENTITY_LIST_HIERARCHY").c_str()))
			{
				m_ShowContextMenu = false;

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

				if (!searchString.empty())
				{
					for (HierarchyItemView* itemPtr : m_FilteredItems)
					{
						if (!itemPtr)
						{
							continue;
						}

						HierarchyItemView& item = *itemPtr;
						item.Render(*this);
					}
				}
				else
				{
					int move_from = -1, move_to = -1;
					for (int n = 0; n < DEBUG_ITEMS.size(); n++)
					{
						DEBUG_ITEMS[n].Render(*this);

						if (m_IsRenaming)
						{
							continue;
						}
						ImGuiDragDropFlags src_flags = 0;
						src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
						src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;
						src_flags |= ImGuiDragDropFlags_AcceptNoPreviewTooltip;

						if (ImGui::BeginDragDropSource(src_flags))
						{
							ImGui::SetDragDropPayload("ENTITY_LIST_DND_HIERARCHY", &n, sizeof(int));
							ImGui::EndDragDropSource();
							ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
						}

						if (ImGui::BeginDragDropTarget())
						{
							ImGuiDragDropFlags target_flags = 0;
							target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
							target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_LIST_DND_HIERARCHY", target_flags))
							{
								move_from = *(const int*)payload->Data;
								move_to = n;
							}
							ImGui::EndDragDropTarget();
						}
					}
					if (!m_IsRenaming && move_from != -1 && move_to != -1)
					{
						int copy_dst = (move_from < move_to) ? move_from : move_to + 1;
						int copy_src = (move_from < move_to) ? move_from + 1 : move_to;
						int copy_count = (move_from < move_to) ? move_to - move_from : move_from - move_to;
						auto tmp = DEBUG_ITEMS[move_from];
						auto tmp2 = DEBUG_ITEMS[move_to];
						DEBUG_ITEMS[move_to] = tmp;
						DEBUG_ITEMS[move_from] = tmp2;
						ImGui::SetDragDropPayload("ENTITY_LIST_DND_HIERARCHY", &move_to, sizeof(int));
					}
				}
				ImGui::PopStyleVar();

				if (m_ShowContextMenu)
				{
					ImGui::OpenPopup(IMGUI_FORMAT_ID("", POPUP_WINDOW_ID, "CONTEXT_MENU_HIERARCHY").c_str());
				}

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
				if (SELECTED_HIERARCHY_ITEM != -1 && ImGui::BeginPopup(IMGUI_FORMAT_ID("", POPUP_WINDOW_ID, "CONTEXT_MENU_HIERARCHY").c_str()))
				{
					ImVec4 textColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
					textColor.w = 0.5f;
					ImGui::TextColored(textColor, DEBUG_ITEMS[SELECTED_HIERARCHY_ITEM].Name().c_str());
					if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_COPY) + " Copy", MENU_ITEM_ID, "COPY_ITEM_HIERARCHY").c_str()))
					{
						// TODO: Copy.
					}
					if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_PASTE) + " Paste", MENU_ITEM_ID, "PASTE_ITEM_HIERARCHY").c_str()))
					{
						// TODO: Paste.
					}
					ImGui::Separator();
					if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_RENAME) + " Rename", MENU_ITEM_ID, "RENAME_ITEM_HIERARCHY").c_str()))
					{
						m_IsRenaming = true;
					}
					if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_COPY) + " Duplicate", MENU_ITEM_ID, "DUPLICATE_ITEM_HIERARCHY").c_str()))
					{
						// TODO: Duplicate.
					}
					if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_DELETE) + " Delete", MENU_ITEM_ID, "DELETE_ITEM_HIERARCHY").c_str()))
					{
						DEBUG_ITEMS.erase(DEBUG_ITEMS.begin() + SELECTED_HIERARCHY_ITEM);
					}

					ImGui::EndPopup();
				}
				ImGui::PopStyleVar();

				ImGui::EndChild();
			}
		}

		bool HierarchyWindow::WindowBegin()
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 5));
			return BaseWindow::WindowBegin();
		}

		void HierarchyWindow::WindowEnd()
		{
			BaseWindow::WindowEnd();
			ImGui::PopStyleVar();
		}

		void HierarchyWindow::SetShowContextMenu(bool a_ShowContextMenu)
		{
			m_ShowContextMenu = true;
		}

		void HierarchyWindow::SetIsRenaming(bool a_IsRenaming)
		{
			m_IsRenaming = a_IsRenaming;
		}

		bool HierarchyWindow::ShowContextMenu() const
		{
			return m_ShowContextMenu;
		}

		bool HierarchyWindow::IsRenaming() const
		{
			return m_IsRenaming;
		}
	}
}

#endif // __EDITOR__