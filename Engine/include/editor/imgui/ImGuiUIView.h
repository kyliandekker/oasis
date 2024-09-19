#pragma once

#ifdef __EDITOR__

namespace oasis
{
	namespace imgui
	{
		class ImGuiWindow;

		class ImGuiUIView
		{
		public:
			ImGuiUIView(ImGuiWindow& a_Window);
			virtual void Render() = 0;

			void Capital(const char* a_Text);
			void H1(const char* a_Text);
			void Bold(const char* a_Text);
			void Italics(const char* a_Text);
			void BoldItalics(const char* a_Text);
			ImGuiWindow& Window() const;

			static float GetRGBColor(int a_Color);
		protected:
			ImGuiWindow& m_Window;
		};
	}
}

#endif // __EDITOR__