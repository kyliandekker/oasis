#pragma once

#include <stdint.h>
#include <glm/vec2.hpp>

#include "dx12/DX12Window.h"

#if defined(CreateWindow)
#undef CreateWindow
#endif

#ifdef __EDITOR__
#include "editor/imgui/ImGuiWindow.h"
#endif // __EDITOR__

namespace oasis
{
	namespace graphics
	{
		class Window
		{
		public:
			~Window() = default;

			bool Initialize(HINSTANCE a_hInstance, uint32_t a_Width, uint32_t a_Height);
			void PollEvents();
			void Destroy();

			void Show();
			void Hide();
			void ToggleFullscreen();

			glm::vec2 GetRealSize() const;

			HWND& GetHWnd() { return m_hWnd; }
			WNDCLASSEX& GetWc() { return m_Wc; }
			dx12::DX12Window& GetDX12Window() { return m_DX12Window; }
#ifdef __EDITOR__
			imgui::ImGuiWindow& GetImGuiWindow() { return m_ImGuiWindow; }
#endif // __EDITOR__

			bool Ready() const { return  m_Ready; }
			void SetReady(bool a_Ready) { m_Ready = a_Ready; }

			void (*MsgCallback)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = nullptr;
		private:
			dx12::DX12Window m_DX12Window;
#ifdef __EDITOR__
			imgui::ImGuiWindow m_ImGuiWindow;
#endif // __EDITOR__

			void RegisterWindowClass(HINSTANCE hInst);
			bool CreateWindow(HINSTANCE a_hInstance, uint32_t a_Width, uint32_t a_Height, const char* a_WindowTitle);
			
			HWND m_hWnd = HWND();
			WNDCLASSEX m_Wc = WNDCLASSEX();

			bool m_Fullscreen = false;
			bool m_Ready = false;
		};
		void ThreadedWindowCreation(Window* a_Window, HINSTANCE a_hInstance, uint32_t a_Width, uint32_t a_Height);
	}
}