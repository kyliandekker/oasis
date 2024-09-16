#include "imgui/ImGuiWindow.h"

#include <cstdint>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx12.h>

#include "imgui/tools/MainWindow.h"
#include "system/Logger.h"
#include "imgui/ImguiDefines.h"
#include "font_file.h"

#include "imgui/tools/HierarchyWindow.h"
#include "imgui/tools/SceneWindow.h"
#include "imgui/tools/InspectorWindow.h"
#include "imgui/tools/ConsoleWindow.h"
#include "imgui/tools/ExplorerWindow.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace oasis
{
	namespace imgui
	{
		ImGuiWindow::ImGuiWindow()
		{
		}

		ImGuiWindow::~ImGuiWindow()
		{
			m_Enabled = false;
		}

		void ImGuiWindow::SetState(EditorState a_EditorState)
		{
			m_EditorState = a_EditorState;
		}

		EditorState ImGuiWindow::GetEditorState() const
		{
			return m_EditorState;
		}

		float GetRGBColor(int color)
		{
			return 1.0f / 255.0f * static_cast<float>(color);
		}

		ImVec4 ColorFromBytes(int r, int g, int b)
		{
			return ImVec4(GetRGBColor(r), GetRGBColor(g), GetRGBColor(b), 1);
		}

		void ImGuiWindow::SetHwnd(HWND a_Hwnd, WNDCLASSEX a_Wc)
		{
			m_Hwnd = a_Hwnd;
			m_Wc = a_Wc;
		}

		void ImGuiWindow::Initialize()
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();

			CreateContextWin32();
			CreateContextDX12();
			CreateImGui();

			m_Enabled = true;

			logger::logger.m_LoggerCallback = &window.LoggerCallback;
		}

		void ImGuiWindow::ProcessEvents(HWND a_Hwnd, UINT a_Msg, WPARAM a_wParam, LPARAM a_lParam)
		{
			ImGui_ImplWin32_WndProcHandler(a_Hwnd, a_Msg, a_wParam, a_lParam);
		}

		void ImGuiWindow::CreateContextWin32()
		{
			ImGui_ImplWin32_Init(m_Hwnd);
		}

		void ImGuiWindow::CreateContextDX12()
		{
			if (!m_DX12Window.CreateDeviceD3D(m_Hwnd))
			{
				m_DX12Window.CleanupDeviceD3D();
				return;
			}

			ImGui_ImplDX12_Init(m_DX12Window.g_pd3dDevice, m_DX12Window.NUM_FRAMES_IN_FLIGHT,
				DXGI_FORMAT_R8G8B8A8_UNORM, m_DX12Window.g_pd3dSrvDescHeap,
				m_DX12Window.g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
				m_DX12Window.g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart());
		}

		ImFont* m_DefaultFont;
		ImFont* m_Capital;
		ImFont* m_Small;
		ImFont* m_H1;
		ImFont* m_BoldFont;
		ImFont* m_ItalicsFont;
		ImFont* m_BoldItalicsFont;
		ImFont* m_IconFont;

		void ImGuiWindow::CreateImGui() const
		{
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			(void)io;

			// setup Dear ImGui style
			ImGui::StyleColorsDark();

			ImFont* font1 = io.Fonts->AddFontDefault();
			(void)font1;

			constexpr ImWchar icons_ranges_b[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };

			ImFontConfig font_config_capital;
			font_config_capital.FontDataOwnedByAtlas = false;
			m_Capital = io.Fonts->AddFontFromMemoryTTF(&font::arial, sizeof(font::arial), m_FontSize + 10, &font_config_capital);

			ImFontConfig font_config_small;
			font_config_capital.FontDataOwnedByAtlas = false;
			font_config_capital.MergeMode = true;
			font_config_capital.PixelSnapH = true;
			font_config_capital.FontDataOwnedByAtlas = false;
			m_Small = io.Fonts->AddFontFromMemoryTTF(&font::fontAwesome, sizeof(font::fontAwesome), m_SmallFontSize, &font_config_small, icons_ranges_b);
			
			ImFontConfig font_config_h1;
			font_config_h1.FontDataOwnedByAtlas = false;
			m_H1 = io.Fonts->AddFontFromMemoryTTF(&font::arial, sizeof(font::arial), m_FontSize + 10, &font_config_h1);

			ImFontConfig font_config_bold;
			font_config_bold.FontDataOwnedByAtlas = false;
			m_BoldFont = io.Fonts->AddFontFromMemoryTTF(&font::arialBold, sizeof(font::arialBold), m_FontSize, &font_config_bold);

			ImFontConfig font_config_italics;
			font_config_italics.FontDataOwnedByAtlas = false;
			m_ItalicsFont = io.Fonts->AddFontFromMemoryTTF(&font::arialItalics, sizeof(font::arialItalics), m_FontSize, &font_config_italics);

			ImFontConfig font_config_boldItalics;
			font_config_boldItalics.FontDataOwnedByAtlas = false;
			m_BoldItalicsFont = io.Fonts->AddFontFromMemoryTTF(&font::arialBoldItalics, sizeof(font::arialBoldItalics), m_FontSize, &font_config_boldItalics);

			ImFontConfig font_config_default;
			font_config_default.FontDataOwnedByAtlas = false;
			m_DefaultFont = io.Fonts->AddFontFromMemoryTTF(&font::arial, sizeof(font::arial), m_FontSize, &font_config_default);

			ImFontConfig icons_config_b;
			icons_config_b.MergeMode = true;
			icons_config_b.PixelSnapH = true;
			icons_config_b.FontDataOwnedByAtlas = false;
			m_IconFont = io.Fonts->AddFontFromMemoryTTF(&font::fontAwesome, sizeof(font::fontAwesome), m_FontSize, &icons_config_b, icons_ranges_b);
			io.Fonts->Build();

			ImGui::StyleColorsDark();

			ImVec4* colors = ImGui::GetStyle().Colors;
			//colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			//colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
			//colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
			//colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			//colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
			//colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
			//colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
			//colors[ImGuiCol_Slider] = ImVec4(1.0f, 0.8f, 0.18f, 1.0f);
			//colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 0.8f, 0.18f, 0.4f);
			//colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
			//colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
			//colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			//colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
			//colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			//colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			//colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
			//colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
			//colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
			//colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
			//colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
			//colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
			//colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
			//colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
			//colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
			//colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
			//colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
			//colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
			//colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
			//colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
			//colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
			//colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
			//colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
			//colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
			//colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
			//colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
			//colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			//colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
			//colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
			//colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			//colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
			//colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			//colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			//colors[ImGuiCol_Link] = ImVec4(0.00f, 0.47f, 0.80f, 1.00f);
			//colors[ImGuiCol_Checked] = ImVec4(1.0f, 0.8f, 0.18f, 1.0f);
			//colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			//colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			//colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			//colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
			//colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
			//colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
			//colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			//colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
			//colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
			//colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
			//colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			//colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
			//colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
			//colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

			ImGuiStyle& style = ImGui::GetStyle();
			style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
			style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
			style.IndentSpacing = 25;
			style.ScrollbarSize = 15;
			style.GrabMinSize = 10;
			style.WindowBorderSize = 1;
			style.ChildBorderSize = 1;
			style.PopupBorderSize = 1;
			style.FrameBorderSize = 1;
			style.TabBorderSize = 1;
			style.WindowRounding = 7;
			style.ChildRounding = 4;
			style.FrameRounding = 3;
			style.PopupRounding = 4;
			style.ScrollbarRounding = 9;
			style.GrabRounding = 3;
			style.LogSliderDeadzone = 4;
			style.TabRounding = 4;
			style.FramePadding = ImVec2(8, 8);
		}

		void ImGuiWindow::ResetDevice()
		{
			if (!m_Enabled)
			{
				return;
			}

			ImGui_ImplDX12_InvalidateDeviceObjects();
			m_DX12Window.CleanupRenderTarget();
			DXGI_SWAP_CHAIN_DESC1 sd;
			m_DX12Window.g_pSwapChain->GetDesc1(&sd);
			sd.Width = static_cast<UINT>(GetRealSize().x);
			sd.Height = static_cast<UINT>(GetRealSize().y);

			m_DX12Window.g_pSwapChain->ResizeBuffers(sd.BufferCount, sd.Width, sd.Height, sd.Format, sd.Flags);
			m_DX12Window.CreateRenderTarget();
			ImGui_ImplDX12_CreateDeviceObjects();

			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize.x = GetRealSize().x;
			io.DisplaySize.y = GetRealSize().y;
		}

		bool ImGuiWindow::IsEnabled() const
		{
			return m_Enabled;
		}

		ImVec2 ImGuiWindow::GetRealSize() const
		{
			RECT rect;
			GetClientRect(m_Hwnd, &rect);

			return ImVec2(static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top));
		}

		void ImGuiWindow::Stop()
		{
			m_Enabled = false;
		}

		dx12::DX12Window& ImGuiWindow::GetDX12()
		{
			return m_DX12Window;
		}

		ImFont*& ImGuiWindow::H1() const
		{
			return m_H1;
		}

		ImFont*& ImGuiWindow::Capital() const
		{
			return m_Capital;
		}

		ImFont*& ImGuiWindow::Small() const
		{
			return m_Small;
		}

		ImFont*& ImGuiWindow::Bold() const
		{
			return m_BoldFont;
		}

		ImFont*& ImGuiWindow::Italics() const
		{
			return m_ItalicsFont;
		}

		ImFont*& ImGuiWindow::BoldItalics() const
		{
			return m_BoldItalicsFont;
		}

		float ImGuiWindow::FontSize() const
		{
			return m_FontSize;
		}

		float ImGuiWindow::SmallFontSize() const
		{
			return m_SmallFontSize;
		}

		void ImGuiWindow::Render()
		{
			if (!m_Enabled)
			{
				return;
			}

			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

			ImGui::SetNextWindowPos(ImVec2(0, 0));

			ImGui::PushFont(m_IconFont);

			ProcessState(m_EditorState);

			hierarchyWindow.Update();
			sceneWindow.Update();
			inspectorWindow.Update();
			consoleWindow.Update();
			explorerWindow.Update();

			if (m_IsError)
			{
				ImGui::PushStyleColor(ImGuiCol_NavWindowingHighlight, ImVec4(1.00f, 0.00f, 0.00f, 0.75f));
				ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(1.00f, 0.00f, 0.00f, 0.35f));
				ImGui::PushStyleColor(ImGuiCol_NavWindowingDimBg, ImVec4(1.00f, 0.00f, 0.00f, 0.2f));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_NavWindowingHighlight, ImVec4(0.00f, 1.00f, 0.00f, 0.75f));
				ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.00f, 1.00f, 0.00f, 0.35f));
				ImGui::PushStyleColor(ImGuiCol_NavWindowingDimBg, ImVec4(0.00f, 1.00f, 0.00f, 0.2f));
			}

			if (m_ShowPopUp)
			{
				ImGui::OpenPopup(IMGUI_FORMAT_ID(m_PopUpTitle.c_str(), POPUP_WINDOW_ID, "POPUP").c_str());

				ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
				ImVec2 size = window.GetRealSize(); // Adjust as needed
				size = ImVec2(size.x * 0.5f, size.y * 0.5f);

				// Calculate the position of the top-left corner
				ImVec2 pos = ImVec2(center.x - size.x * 0.5f, center.y - size.y * 0.5f);
				ImGui::SetNextWindowPos(pos);
				ImGui::SetNextWindowSize(size);
				if (ImGui::BeginPopupModal(IMGUI_FORMAT_ID(m_PopUpTitle.c_str(), POPUP_WINDOW_ID, "POPUP").c_str(), &m_ShowPopUp, ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::PushTextWrapPos();
					ImGui::Text(m_PopUpText.c_str());
					ImGui::PopTextWrapPos();
					if (ImGui::Button("Close"))
					{
						m_ShowPopUp = false;
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::PopFont();

			ImGui::EndFrame();

			ImGui::Render();
			m_DX12Window.Render();
		}

		void ImGuiWindow::DeleteWindow()
		{
			ImGui_ImplDX12_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
			m_DX12Window.CleanupDeviceD3D();
		}

		void ImGuiWindow::LoggerCallback(const logger::Message& a_Message)
		{
			//window.m_PopUpText = a_Message.m_Message;
			//switch (a_Message.a_Severity)
			//{
			//	case logger::LOGSEVERITY_ERROR:
			//	{
			//		window.m_IsError = true;

			//		window.m_PopUpTitle = "Error";
			//		break;
			//	}
			//	case logger::LOGSEVERITY_ASSERT:
			//	{
			//		window.m_IsError = true;

			//		window.m_PopUpTitle = "Critical Error";
			//		break;
			//	}
			//	case logger::LOGSEVERITY_SUCCESS:
			//	{
			//		window.m_IsError = false;

			//		window.m_PopUpTitle = "Success";
			//		window.m_PopUpText = a_Message.m_RawMessage;
			//		break;
			//	}
			//	case logger::LOGSEVERITY_AWESOME:
			//	{
			//		window.m_IsError = false;

			//		window.m_PopUpTitle = "Great";
			//		break;
			//	}
			//	default:
			//	{
			//		return;
			//	}
			//}
			//window.m_ShowPopUp = true;
		}

		MainWindow mainWindow;
		void ImGuiWindow::ProcessState(EditorState a_EditorState)
		{
			switch (a_EditorState)
			{
				case EditorState::Editor:
				{
					mainWindow.SetSize(GetRealSize());
					mainWindow.Update();
					break;
				}
			}
		}
	}
}