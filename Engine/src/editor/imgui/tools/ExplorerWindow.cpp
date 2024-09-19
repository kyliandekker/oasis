#include "imgui/tools/ExplorerWindow.h"

#include "imgui/ImGuiDefines.h"

namespace oasis
{
	namespace imgui
	{
		ExplorerWindow::ExplorerWindow() : BaseWindow(ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_FOLDER) + " Explorer", "Explorer")
		{
		}

		void ExplorerWindow::Render()
		{
		}
	}
}