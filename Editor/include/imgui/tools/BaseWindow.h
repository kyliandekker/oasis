#pragma once

#include <string>
#include <imgui/imgui.h>

#include "imgui/ImGuiUIView.h"

typedef int ImGuiWindowFlags;

namespace oasis
{
	namespace imgui
	{
		class BaseWindow : public ImGuiUIView
		{
		public:
			BaseWindow(ImGuiWindowFlags a_Flags, std::string a_Name, std::string a_WindowID, bool a_FullScreen = false);
			~BaseWindow();

			virtual bool WindowBegin();
			virtual void WindowEnd();
			void Update();

			void SetSize(ImVec2 a_Size);
			bool IsFullScreen() const;

			void SetEnabled(bool a_Enabled);
			void ToggleEnabled();
			std::string GetName() const;
			bool IsEnabled() const;
		protected:
			ImGuiWindowFlags m_Flags = 0;
			ImVec2 m_Size = ImVec2(0, 0);
			std::string m_Name, m_WindowID;
			bool m_FullScreen = false;
			bool m_Enabled = true;
			bool m_RenderWindowItself = false;
		};
	}
}