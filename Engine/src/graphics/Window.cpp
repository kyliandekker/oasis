#include "graphics/Window.h"

#include <windows.h>
#if defined(CreateWindow)
#undef CreateWindow
#endif
#if defined(min)
#undef min
#endif
#if defined(max)
#undef max
#endif
#include <utility>

#include "system/Logger.h"
#include "core/Engine.h"

#ifdef __EDITOR__
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx12.h>
#include "editor/settings/EditorSettings.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif // __EDITOR__

namespace oasis
{
	namespace graphics
	{
		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
#ifdef __EDITOR__
			ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
#endif // __EDITOR__
			switch (msg)
			{
				case WM_EXITSIZEMOVE:
				{
					Window& window = engine::engine.GetWindow();
					window.SetReady(false);

					dx12::DX12Window& dx12window = window.GetDX12Window();
#ifdef __EDITOR__
					imgui::ImGuiWindow& imGuiWindow = window.GetImGuiWindow();
#endif // __EDITOR__

					glm::vec2 size(window.GetRealSize().x, window.GetRealSize().y);

#ifdef __EDITOR__
					ImGui_ImplDX12_InvalidateDeviceObjects();
#endif // __EDITOR__

					dx12window.CleanupRenderTarget();
					DXGI_SWAP_CHAIN_DESC1 sd;
					dx12window.g_pSwapChain->GetDesc1(&sd);
					sd.Width = static_cast<UINT>(window.GetRealSize().x);
					sd.Height = static_cast<UINT>(window.GetRealSize().y);

					dx12window.g_pSwapChain->ResizeBuffers(sd.BufferCount, sd.Width, sd.Height, sd.Format, sd.Flags);
					dx12window.CreateRenderTarget();

#ifdef __EDITOR__
					ImGui_ImplDX12_CreateDeviceObjects();

					ImGuiIO& io = ImGui::GetIO();
					io.DisplaySize.x = size.x;
					io.DisplaySize.y = size.y;

					settings::settings.SetSize(size);
#endif // __EDITOR__

					window.SetReady(true);
					break;
				}
				case WM_DESTROY:
				{
					::PostQuitMessage(0);
					break;
				}
				default:
				{
					return ::DefWindowProcW(hwnd, msg, wParam, lParam);
				}
			}

			return 0;
		}

		glm::vec2 Window::GetRealSize() const
		{
			RECT rect;
			GetClientRect(m_hWnd, &rect);

			return glm::vec2(static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top));
		}

		bool Window::Initialize(HINSTANCE a_hInstance, uint32_t a_Width, uint32_t a_Height)
		{
			RegisterWindowClass(a_hInstance);
			CreateWindow(a_hInstance, a_Width, a_Height, "Oasis");

			// Initialize DX12.
			if (!m_DX12Window.CreateDeviceD3D(m_hWnd))
			{
				LOGF(logger::LOGSEVERITY_ASSERT, "Creating DX12 Window failed.");
				m_DX12Window.CleanupDeviceD3D();
				return false;
			}

#ifdef __EDITOR__
			// Initialize editor.
			if (!m_ImGuiWindow.Initialize())
			{
				LOGF(logger::LOGSEVERITY_ASSERT, "Could not create ImGui.");
				return false;
			}
#endif // __EDITOR__

			// TODO: Fullscreen.
//#ifndef __ENGINE__
//			ToggleFullscreen();
//			RECT clientRect = {};
//			::GetClientRect(this->m_hWnd, &clientRect);
//
//			window->SetWidth(static_cast<uint32_t>(std::max(1l, clientRect.right - clientRect.left)));
//			window->SetHeight(static_cast<uint32_t>(std::max(1l, clientRect.bottom - clientRect.top)));
//
//			window->SetHasResized(true);
//#endif

			return true;
		}

		void Window::PollEvents()
		{
			MSG msg = {};
			while (GetMessage(&msg, NULL, 0, 0) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (!m_Ready)
				{
					return;
				}
#ifdef __EDITOR__
				m_ImGuiWindow.Render();
#endif // __EDITOR__
				m_DX12Window.Render();
			}
		}

		void Window::Destroy()
		{
			m_Ready = false;

			// Destroy ImGui an DX12 first.
#ifdef __EDITOR__
			m_ImGuiWindow.Destroy();
#endif // __EDITOR__
			m_DX12Window.Destroy();

			::DestroyWindow(m_hWnd);
			::UnregisterClassW(m_Wc.lpszClassName, m_Wc.hInstance);

			LOGF(logger::LOGSEVERITY_SUCCESS, "Destroyed window.");
		}

		void Window::Show()
		{
			::ShowWindow(m_hWnd, 1);
			::UpdateWindow(m_hWnd);
		}

		void Window::Hide()
		{
			::ShowWindow(m_hWnd, 0);
		}

		void Window::ToggleFullscreen()
		{
			RECT windowRect;

			m_Fullscreen = !m_Fullscreen;
			if (m_Fullscreen)
			{
				::GetWindowRect(m_hWnd, &windowRect);

				UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU |
					WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

				::SetWindowLongW(m_hWnd, GWL_STYLE, windowStyle);

				HMONITOR hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
				MONITORINFOEX monitorInfo = {};
				monitorInfo.cbSize = sizeof(MONITORINFOEX);
				::GetMonitorInfo(hMonitor, &monitorInfo);
				::SetWindowPos(m_hWnd, HWND_TOP, monitorInfo.rcMonitor.left,
					monitorInfo.rcMonitor.top, monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
					monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
					SWP_FRAMECHANGED | SWP_NOACTIVATE);

				::ShowWindow(m_hWnd, SW_MAXIMIZE);
			}
			else
			{
				// TODO: Fix this.
				// Restore all the window decorators.
				//::SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

				//::SetWindowPos(m_hWnd, HWND_NOTOPMOST, windowRect.left,
				//	windowRect.top, windowRect.right - windowRect.left,
				//	windowRect.bottom - windowRect.top, SWP_FRAMECHANGED | SWP_NOACTIVATE);

				//::ShowWindow(m_hWnd, SW_NORMAL);
			}
		}

		void Window::RegisterWindowClass(HINSTANCE hInst)
		{
			const wchar_t* windowClassName = L"Gudenuff Engine Window";
			WNDCLASSEXW windowClass = {};

			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = CS_HREDRAW | CS_VREDRAW;
			windowClass.lpfnWndProc = &WndProc;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = hInst;
			windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
			windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			windowClass.lpszMenuName = NULL;
			windowClass.lpszClassName = windowClassName;

			static ATOM atom = ::RegisterClassExW(&windowClass);
		}

		bool Window::CreateWindow(HINSTANCE a_hInstance, uint32_t a_Width, uint32_t a_Height, const char* a_WindowTitle)
		{
			glm::vec2 size(a_Width, a_Height);

			m_Wc = {};
			m_Wc.cbSize = sizeof(WNDCLASSEX);
			m_Wc.style = 0;
			m_Wc.lpfnWndProc = WndProc;
			m_Wc.cbClsExtra = 0;
			m_Wc.cbWndExtra = 0;
			m_Wc.hInstance = a_hInstance;
			m_Wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			m_Wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			m_Wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
			m_Wc.lpszMenuName = NULL;
			m_Wc.lpszClassName = (LPCWSTR)a_WindowTitle;
			m_Wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
			// TODO: Icon.
			// 
			//m_Wc.hIcon = static_cast<HICON>(::LoadImage(hInst,
			//	MAKEINTRESOURCE(IDI_ICON2),
			//	IMAGE_ICON,
			//	16, 16,
			//	LR_DEFAULTCOLOR));
			// 
			//m_Wc.hIconSm = static_cast<HICON>(::LoadImage(hInst,
			//	MAKEINTRESOURCE(IDI_ICON2),
			//	IMAGE_ICON,
			//	16, 16,
			//	LR_DEFAULTCOLOR));

			if (!RegisterClassEx(&m_Wc))
			{
				LOGF(logger::LOGSEVERITY_ASSERT, "Window registration failed.");
				return m_Ready;
			}

			m_hWnd = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				(LPCWSTR)a_WindowTitle,
				L"Oasis",
				WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX,
				CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(size.x), static_cast<int>(size.y),
				NULL, NULL, a_hInstance, NULL);

			if (!m_hWnd)
			{
				LOGF(logger::LOGSEVERITY_ASSERT, "Failed to create window.");
				return m_Ready;
			}

			m_Ready = true;

			LOGF(logger::LOGSEVERITY_SUCCESS, "Window has been created.");
			return m_Ready;
		}

		void ThreadedWindowCreation(Window* a_Window, HINSTANCE a_hInstance, uint32_t a_Width, uint32_t a_Height)
		{
			a_Window->Initialize(a_hInstance, a_Width, a_Height);
			a_Window->PollEvents();
			a_Window->Destroy();
		}
	}
}