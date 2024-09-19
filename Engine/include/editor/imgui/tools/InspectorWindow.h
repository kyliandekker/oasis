#pragma once

#ifdef __EDITOR__

#include "editor/imgui/tools/BaseWindow.h"

namespace oasis
{
	namespace imgui
	{
		class ImGuiWindow;

		class InspectorWindow : public BaseWindow
		{
		public:
			InspectorWindow(ImGuiWindow& a_Window);
			void Render() override;

			bool WindowBegin() override;
			void WindowEnd() override;
		};
	}
}

#endif // __EDITOR__