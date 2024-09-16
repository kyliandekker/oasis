#include "imgui/tools/HierarchyWindow.h"

#include "imgui/ImGuiWindow.h"
#include "imgui/ImGuiDefines.h"
#include "imgui/imgui_helpers.h"
#include "system/Logger.h"
#include "utils/string_extensions.h"

namespace oasis
{
	namespace imgui
	{
		HierarchyItemView::HierarchyItemView(const std::string& a_Icon, const std::string& a_Name, int a_ID) : m_Icon(a_Icon), m_Name(a_Name), m_ID(a_ID)
		{ }

		// TODO: Move this to another class. This needs to be used by various other windows like the inspector, explorer, etc.
		int SELECTED_HIERARCHY_ITEM = -1;
		char SEARCHSTRING[256] = { '\0' };
		std::string searchString;

		void HierarchyItemView::Render()
		{
			const float fontSize = window.FontSize();

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
			if (hierarchyWindow.IsRenaming() && SELECTED_HIERARCHY_ITEM == m_ID)
			{
				char renameText[256];
				strcpy_s(renameText, sizeof(renameText), m_Name.c_str());

				if (ImGui::InvisInputText(IMGUI_FORMAT_ID("", INPUT_ID, m_Name.c_str()).c_str(), renameText, sizeof(renameText), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EnterReturnsTrue))
				{
					m_Name = renameText;
					hierarchyWindow.SetIsRenaming(false);
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

			if (ImGui::IsItemHovered() && ImGui::IsItemClicked(1))
			{
				hierarchyWindow.SetShowContextMenu(true);
				SELECTED_HIERARCHY_ITEM = m_ID;
			}

			if (ImGui::IsItemHovered() && ImGui::IsItemClicked(0))
			{
				SELECTED_HIERARCHY_ITEM = m_ID;
			}

			if (hierarchyWindow.ShowContextMenu())
			{
				ImGui::OpenPopup(IMGUI_FORMAT_ID("", POPUP_WINDOW_ID, "HIERARCHY_CONTEXT_MENU").c_str());
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

		HierarchyWindow::HierarchyWindow() : BaseWindow(ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_HIERARCHY) + " Hierarchy", "Hierarchy")
		{ }

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

			static float windowWidth = ImGui::GetContentRegionAvail().x;
			if (ImGui::TransparentButton(IMGUI_FORMAT_ID(ICON_FA_ADD, BUTTON_ID, "ADD_MENU_HIERARCHY").c_str()))
			{

			}

			static float searchBarSizeX = windowWidth / 2;
			ImGui::SameLine();
			ImGui::SetCursorPosX(windowWidth - (searchBarSizeX - 50));

			ImGui::SetNextItemWidth(searchBarSizeX);
			if (ImGui::SearchBar(IMGUI_FORMAT_ID("", INPUT_ID, "SEARCH_ENTITIES_HIERARCHY").c_str(), SEARCHSTRING, sizeof(SEARCHSTRING), ImVec2(0, searchBarSizeX)))
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
						item.Render();
					}
				}
				else
				{
					int move_from = -1, move_to = -1;
					for (int n = 0; n < DEBUG_ITEMS.size(); n++)
					{
						DEBUG_ITEMS[n].Render();

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

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
				if (SELECTED_HIERARCHY_ITEM != -1 && ImGui::BeginPopup(IMGUI_FORMAT_ID("", POPUP_WINDOW_ID, "CONTEXT_MENU_HIERARCHY").c_str()))
				{
					ImVec4 textColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
					textColor.w = 0.5f;
					ImGui::TextColored(textColor, DEBUG_ITEMS[SELECTED_HIERARCHY_ITEM].Name().c_str());
					if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_COPY) + " Copy", MENU_ITEM_ID, "COPY_ITEM_HIERARCHY").c_str()))
					{

					}
					if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_PASTE) + " Paste", MENU_ITEM_ID, "PASTE_ITEM_HIERARCHY").c_str()))
					{

					}
					ImGui::Separator();
					if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_RENAME) + " Rename", MENU_ITEM_ID, "RENAME_ITEM_HIERARCHY").c_str()))
					{
						m_IsRenaming = true;
					}
					if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_COPY) + " Duplicate", MENU_ITEM_ID, "DUPLICATE_ITEM_HIERARCHY").c_str()))
					{

					}
					if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_DELETE) + " Delete", MENU_ITEM_ID, "DELETE_ITEM_HIERARCHY").c_str()))
					{
						DEBUG_ITEMS.erase(DEBUG_ITEMS.begin() + SELECTED_HIERARCHY_ITEM);
					}

					ImGui::EndPopup();
				}
				ImGui::PopStyleVar();

				//if (ImGui::Button("Info"))
				//{
				//	LOGF(logger::LOGSEVERITY_INFO, "This is an info message.");
				//}
				//ImGui::SameLine();
				//if (ImGui::Button("Warning"))
				//{
				//	LOGF(logger::LOGSEVERITY_WARNING, "This is a warning message.");
				//}
				//ImGui::SameLine();
				//if (ImGui::Button("Error"))
				//{
				//	LOGF(logger::LOGSEVERITY_ERROR, "This is an error message.");
				//}
				//if (ImGui::Button("Assert"))
				//{
				//	LOGF(logger::LOGSEVERITY_ASSERT, "This is an assert message.");
				//}
				//ImGui::SameLine();
				//if (ImGui::Button("Success"))
				//{
				//	LOGF(logger::LOGSEVERITY_SUCCESS, "This is a success message.");
				//}
				//ImGui::SameLine();
				//if (ImGui::Button("Info Success"))
				//{
				//	LOGF(logger::LOGSEVERITY_INFO_SUCCESS, "This is an info success message.");
				//}
				//ImGui::SameLine();
				//if (ImGui::Button("Awesome"))
				//{
				//	LOGF(logger::LOGSEVERITY_AWESOME, "This is an awesome message.");
				//}

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