#pragma once

#ifdef __EDITOR__

#include <vector>

#include "editor/imgui/tools/BaseWindow.h"

namespace oasis
{
	namespace assets
	{
		class Resource;
	}

	namespace imgui
	{
		class ImGuiWindow;

		class ExplorerWindow : public BaseWindow
		{
		public:
			assets::Resource* m_SelectedResource = nullptr;
			assets::Resource* m_SelectedResource2 = nullptr;
			assets::Resource* m_ExplorerRoot = nullptr;

			bool m_ShowContextMenu = false;
			bool m_ShowExplorerInspector = false;
			bool m_NeedsRefresh = true;

			char SEARCHSTRING_EXPLORER[256] = { '\0' };
			char EXPLORER_INSPECTOR_RENAME_RESOURCE[256] = { '\0' };

			std::vector<assets::Resource*> m_ResourcesToShow;

			ExplorerWindow(ImGuiWindow& a_Window);
			void RenderResource(assets::Resource& a_Resource, bool a_RenderFoldersOnly = false, bool a_ShowSelf = true);
			void Render() override;
		};
	}
}

#endif // __EDITOR__