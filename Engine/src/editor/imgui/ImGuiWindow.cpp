#ifdef __EDITOR__

#include "editor/imgui/ImGuiWindow.h"

#include <cstdint>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx12.h>

#include <core/Engine.h>
#include <system/Logger.h>

#include "editor/imgui/ImguiDefines.h"
#include "editor/font_file.h"

namespace oasis
{
	namespace imgui
	{
		ImGuiWindow::ImGuiWindow() : mainWindow(*this), consoleWindow(*this), sceneWindow(*this), inspectorWindow(*this), hierarchyWindow(*this), explorerWindow(*this)
		{ }

		ImGuiWindow::~ImGuiWindow()
		{ }

		bool ImGuiWindow::Initialize(int nArgs, ...)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();

			if (!CreateContextWin32() || !CreateContextDX12())
			{
				return false;
			}

			CreateImGui();

			graphics::Window& window = core::engine.GetWindow();
			dx12::DX12Window& dx12window = window.GetDX12Window();

			logger::logger.m_LoggerCallback = std::bind(&ConsoleWindow::LoggerCallback, &consoleWindow, std::placeholders::_1);

			LOGF(logger::LOGSEVERITY_SUCCESS, "Created ImGui.");
			return true;
		}

		bool ImGuiWindow::Destroy()
		{
			ImGui_ImplDX12_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();

			LOGF(logger::LOGSEVERITY_SUCCESS, "Uninitialized ImGui.");
			return true;
		}

		bool ImGuiWindow::CreateContextWin32()
		{
			if (!ImGui_ImplWin32_Init(core::engine.GetWindow().GetHWnd()))
			{
				LOGF(logger::LOGSEVERITY_ERROR, "Could not create a Win32 context for ImGui.");
				return false;
			}

			LOGF(logger::LOGSEVERITY_SUCCESS, "Created a Win32 context for ImGui.");
			return true;
		}

		bool ImGuiWindow::CreateContextDX12()
		{
			dx12::DX12Window& dx12window = core::engine.GetWindow().GetDX12Window();
			if (!ImGui_ImplDX12_Init(dx12window.g_pd3dDevice, dx12window.NUM_FRAMES_IN_FLIGHT,
				DXGI_FORMAT_R8G8B8A8_UNORM, dx12window.g_pd3dSrvDescHeap,
				dx12window.g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
				dx12window.g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart()))
			{
				LOGF(logger::LOGSEVERITY_ERROR, "Could not create a DX12 context for ImGui.");
				return false;
			}

			LOGF(logger::LOGSEVERITY_SUCCESS, "Created a DX12 context for ImGui.");
			return true;
		}

		ImFont* m_DefaultFont;
		ImFont* m_Capital;
		ImFont* m_CapitalIcon;
		//ImFont* m_Small;
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
			font_config_small.FontDataOwnedByAtlas = false;
			font_config_small.MergeMode = true;
			font_config_small.PixelSnapH = true;
			font_config_small.FontDataOwnedByAtlas = false;
			m_CapitalIcon = io.Fonts->AddFontFromMemoryTTF(&font::fontAwesome, sizeof(font::fontAwesome), m_FontSize + 10, &font_config_small, icons_ranges_b);

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

		ImFont*& ImGuiWindow::H1() const
		{
			return m_H1;
		}

		ImFont*& ImGuiWindow::Capital() const
		{
			return m_Capital;
		}

        ImFont*& ImGuiWindow::CapitalIcon() const
        {
			return m_CapitalIcon;
        }

		//ImFont*& ImGuiWindow::Small() const
		//{
		//	return m_Small;
		//}

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
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

			ImGui::SetNextWindowPos(ImVec2(0, 0));

			ImGui::PushFont(m_IconFont);

			graphics::Window& window = core::engine.GetWindow();
			glm::vec2 size = window.GetRealSize();
			mainWindow.SetSize(ImVec2(size.x, size.y));
			mainWindow.Update();

			hierarchyWindow.Update();
			sceneWindow.Update();
			inspectorWindow.Update();
			consoleWindow.Update();
			explorerWindow.Update();

			ImGui::PopFont();

			ImGui::EndFrame();

			ImGui::Render();
		}
	}
}

#endif // __EDITOR__