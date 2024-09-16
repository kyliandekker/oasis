#include "imgui/ImGuiUIView.h"

#include <imgui/imgui.h>

#include "imgui/ImGuiWindow.h"

namespace oasis
{
	namespace imgui
	{
        void ImGuiUIView::Capital(const char* a_Text)
        {
			ImGui::PushFont(imgui::window.Capital());
			ImGui::Text("%s", a_Text);
			ImGui::PopFont();
        }

        void ImGuiUIView::H1(const char* a_Text)
		{
			ImGui::PushFont(imgui::window.H1());
			ImGui::Text("%s", a_Text);
			ImGui::PopFont();
		}

		void ImGuiUIView::Bold(const char* a_Text)
		{
			ImGui::PushFont(imgui::window.Bold());
			ImGui::Text("%s", a_Text);
			ImGui::PopFont();
		}

		void ImGuiUIView::Italics(const char* a_Text)
		{
			ImGui::PushFont(imgui::window.Italics());
			ImGui::Text("%s", a_Text);
			ImGui::PopFont();
		}

		void ImGuiUIView::BoldItalics(const char* a_Text)
		{
			ImGui::PushFont(imgui::window.BoldItalics());
			ImGui::Text("%s", a_Text);
			ImGui::PopFont();
		}

		float ImGuiUIView::GetRGBColor(int a_Color)
		{
			return 1.0f / 255.0f * static_cast<float>(a_Color);
		}
	}
}