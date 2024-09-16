#pragma once

#include <stdint.h>

#if defined(CreateWindow)
#undef CreateWindow
#endif

struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;

struct HWND__;
typedef HWND__* HWND;

namespace oasis
{
	namespace graphics
	{
		class Window
		{
		public:
			explicit Window() = default;
			~Window() = default;

			void Initialize(HINSTANCE hInst, uint32_t width, uint32_t height);
			void PollEvents();
			void Finalize();

			void Show();
			void Hide();
			void ToggleFullscreen();

			uint32_t GetWidth() const { return m_Width; }
			void SetWidth(uint32_t width) { m_Width = width; }
			uint32_t GetHeight() const { return m_Height; }
			void SetHeight(uint32_t height) { m_Height = height; }

			bool ShouldClose() const { return m_ShouldClose; }
			bool HasResized() const { return m_HasResized; }
			void SetHasResized(bool resized) { m_HasResized = resized; }
			HWND GetHWnd() const { return m_hWnd; }
		private:
			void RegisterWindowClass(HINSTANCE hInst);
			void CreateWindow(HINSTANCE hInst, uint32_t width, uint32_t height);
			
			HWND m_hWnd = HWND();

			uint32_t m_Width = 1280;
			uint32_t m_Height = 720;

			bool m_ShouldClose = false;
			bool m_Fullscreen = false;
			bool m_HasResized = false;
		};
		void ThreadedWindowCreation(Window* window, HINSTANCE hInst, uint32_t width, uint32_t height);
		void ThreadedWindowMessages(Window* window);
		extern bool WindowCreationFinished;
	}
}