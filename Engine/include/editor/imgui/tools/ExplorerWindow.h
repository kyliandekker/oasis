#pragma once

#ifdef __EDITOR__

#include "editor/imgui/tools/BaseWindow.h"
#include "editor/project/ExplorerResource.h"

namespace oasis
{
	namespace imgui
	{
		class ImGuiWindow;

		class ExplorerWindow : public BaseWindow
		{
		public:
			ExplorerResource m_ExplorerResource;

			ExplorerResource* m_SelectedResource = nullptr;
			ExplorerResource* m_SelectedResource2 = nullptr;
			ExplorerResource* m_ExplorerRoot = nullptr;

			bool m_ShowContextMenu = false;
			bool m_NeedsRefresh = true;

			std::vector<ExplorerResource*> m_ResourcesToShow;

			ExplorerWindow(ImGuiWindow& a_Window);
			void RenderResource(imgui::ExplorerResource& a_Resource, bool a_RenderFoldersOnly = false, bool a_ShowSelf = true);
			void Render() override;
		};
	}
}

#endif // __EDITOR__