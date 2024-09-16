#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <imgui/imgui.h>

struct ImFont;

struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;

struct HWND__;
typedef HWND__* HWND;

#include "dx12/DX12Window.h"

namespace oasis
{
	namespace logger
	{
		struct Message;
	}

	enum class EditorState
	{
		None,
		PickProject,
		Editor
	};

	namespace imgui
	{
		class ImGuiWindow
		{
		public:
			ImGuiWindow();
			~ImGuiWindow();
			
			void SetState(EditorState a_EditorState);
			EditorState GetEditorState() const;

			void SetHwnd(HWND a_Hwnd, WNDCLASSEX a_Wc);
			void Initialize();
			void ProcessEvents(HWND a_Hwnd, UINT a_Msg, WPARAM a_wParam, LPARAM a_lParam);
			void CreateContextWin32();
			void CreateContextDX12();
			void CreateImGui() const;
			void Render();
			void DeleteWindow();
			void ResetDevice();
			bool IsEnabled() const;
			ImVec2 GetRealSize() const;
			void Stop();
			dx12::DX12Window& GetDX12();

			ImFont*& Capital() const;
			ImFont*& Small() const;
			ImFont*& H1() const;
			ImFont*& Bold() const;
			ImFont*& Italics() const;
			ImFont*& BoldItalics() const;

			float FontSize() const;
			float SmallFontSize() const;

			static void LoggerCallback(const logger::Message& a_Message);

			bool m_ShowPopUp = false;
			std::string m_PopUpText;
			std::string m_PopUpTitle;
			bool m_IsError = true;
		private:
			void ProcessState(EditorState a_EditorState);

			float m_FontSize = 17.5f;
			float m_SmallFontSize = 12.0f;

			HWND m_Hwnd = nullptr;
			WNDCLASSEX m_Wc = WNDCLASSEX();
			bool m_Enabled = false;

			dx12::DX12Window m_DX12Window;
			EditorState m_EditorState = EditorState::Editor;
		};
		inline extern ImGuiWindow window = {};
	}
}