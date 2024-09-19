#pragma once

#ifdef __EDITOR__

#include "editor/imgui/tools/BaseWindow.h"

namespace oasis
{
	namespace imgui
	{
		class MainWindow : public BaseWindow
		{
		public:
			MainWindow(ImGuiWindow& a_Window);
			void Render() override;
		};
	}
}

#endif // __EDITOR__