#pragma once

namespace oasis
{
	namespace imgui
	{
		class ImGuiUIView
		{
		public:
			virtual void Render() = 0;

			void Capital(const char* a_Text);
			void H1(const char* a_Text);
			void Bold(const char* a_Text);
			void Italics(const char* a_Text);
			void BoldItalics(const char* a_Text);
			static float GetRGBColor(int a_Color);
		};
	}
}