#ifdef __EDITOR__

#include "editor/imgui/ImGuiUIView.h"

#include <imgui/imgui.h>

#include "editor/imgui/ImGuiWindow.h"

namespace oasis
{
	namespace imgui
	{
		ImGuiUIView::ImGuiUIView(ImGuiWindow& a_Window) : m_Window(a_Window)
		{ }

		void ImGuiUIView::Capital(const char* a_Text)
        {
			ImGui::PushFont(m_Window.Capital());
			ImGui::Text("%s", a_Text);
			ImGui::PopFont();
        }

        void ImGuiUIView::H1(const char* a_Text)
		{
			ImGui::PushFont(m_Window.H1());
			ImGui::Text("%s", a_Text);
			ImGui::PopFont();
		}

		void ImGuiUIView::Bold(const char* a_Text)
		{
			ImGui::PushFont(m_Window.Bold());
			ImGui::Text("%s", a_Text);
			ImGui::PopFont();
		}

		void ImGuiUIView::Italics(const char* a_Text)
		{
			ImGui::PushFont(m_Window.Italics());
			ImGui::Text("%s", a_Text);
			ImGui::PopFont();
		}

		void ImGuiUIView::BoldItalics(const char* a_Text)
		{
			ImGui::PushFont(m_Window.BoldItalics());
			ImGui::Text("%s", a_Text);
			ImGui::PopFont();
		}

        ImGuiWindow& ImGuiUIView::Window() const
        {
			return m_Window;
        }

		float ImGuiUIView::GetRGBColor(int a_Color)
		{
			return 1.0f / 255.0f * static_cast<float>(a_Color);
		}
	}
}

#endif // __EDITOR__