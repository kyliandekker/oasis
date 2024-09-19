#pragma once

#ifdef __EDITOR__

#include <cstdint>
#include <string>
#include <vector>
#include <windows.h>

#include <imgui/imgui.h>

#include "editor/imgui/tools/MainWindow.h"
#include "editor/imgui/tools/HierarchyWindow.h"
#include "editor/imgui/tools/SceneWindow.h"
#include "editor/imgui/tools/InspectorWindow.h"
#include "editor/imgui/tools/ConsoleWindow.h"
//#include "editor/imgui/tools/ExplorerWindow.h"

struct ImFont;
struct ID3D12GraphicsCommandList;

namespace oasis
{
	namespace logger
	{
		struct Message;
	}

	namespace imgui
	{
		class ImGuiWindow
		{
		public:
			ImGuiWindow();
			~ImGuiWindow();

			bool Initialize();
			bool CreateContextWin32();
			bool CreateContextDX12();
			void CreateImGui() const;
			void Render();
			void Destroy();

			ImFont*& Capital() const;
			ImFont*& CapitalIcon() const;
			//ImFont*& Small() const;
			ImFont*& H1() const;
			ImFont*& Bold() const;
			ImFont*& Italics() const;
			ImFont*& BoldItalics() const;

			float FontSize() const;
			float SmallFontSize() const;
		private:
			float m_FontSize = 17.5f;
			float m_SmallFontSize = 12.0f;

			MainWindow mainWindow;
			ConsoleWindow consoleWindow;
			SceneWindow sceneWindow;
			InspectorWindow inspectorWindow;
			HierarchyWindow hierarchyWindow;
		};
	}
}

#endif // __EDITOR__