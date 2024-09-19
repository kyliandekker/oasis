#pragma once

#ifdef __EDITOR__

#include "editor/imgui/tools/BaseWindow.h"

#include <vector>

#include "editor/imgui/ImGuiUIView.h"

// TODO: Remove this include.
#include "editor/imgui/ImGuiDefines.h"

namespace oasis
{
	namespace imgui
	{
		class HierarchyWindow;

		class HierarchyItemView
		{
		public:
			HierarchyItemView();
			HierarchyItemView(const std::string& a_Icon, const std::string& a_Name, int a_ID);
			void Render(HierarchyWindow& a_Window);
			std::string Name() const;
			std::string Icon() const;
			int ID() const;
		private:
			bool m_FoldedIn = false;

			// TODO: This needs to be removed, as we will probably query this info from the entity manager.
			std::string m_Icon, m_Name;
			int m_ID = -1;

			// TODO: This is the "inactive" functionality of the GameObject.
			bool temp = true;
		};

		// TODO: Remove.
		inline std::vector<HierarchyItemView> DEBUG_ITEMS =
		{ };

		class ImGuiWindow;

		class HierarchyWindow : public BaseWindow
		{
		public:
			HierarchyWindow(ImGuiWindow& a_Window);
			void Render() override;

			bool WindowBegin() override;
			void WindowEnd() override;

			void SetShowContextMenu(bool a_ShowContextMenu);
			void SetIsRenaming(bool a_IsRenaming);

			bool ShowContextMenu() const;
			bool IsRenaming() const;
		private:
			std::vector<HierarchyItemView*> m_FilteredItems;
			bool m_ShowContextMenu = false;
			bool m_IsRenaming = false;
			bool m_NeedsRefresh = true;
		};
	}
}

#endif // __EDITOR__