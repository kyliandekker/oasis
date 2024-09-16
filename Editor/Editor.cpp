#include <windows.h>
#include <thread>
#include <imgui/backends/imgui_impl_win32.h>
#include <settings/StaticSettings.h>

#include "imgui/ImGuiWindow.h"

const char g_szClassName[] = "Oasis";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	oasis::imgui::window.ProcessEvents(hwnd, msg, wParam, lParam);
	switch (msg)
	{
		case WM_CLOSE:
		{
			oasis::imgui::window.Stop();
			oasis::imgui::window.DeleteWindow();
			DestroyWindow(hwnd);
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		case WM_EXITSIZEMOVE:
		{
			if (oasis::imgui::window.IsEnabled())
			{
				RECT r;
				GetWindowRect(hwnd, &r);

				int width = r.right - r.left;
				int height = r.bottom - r.top;

				oasis::project::settings.SetSize(ImVec2(width, height));

				oasis::imgui::window.ResetDevice();
			}
			break;
		}
		default:
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
	return 0;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nShowCmd)
{
	oasis::project::settings.Load();

	WNDCLASSEX wc{};
	HWND hwnd;
	MSG Msg;

#ifdef _DEBUG
	AllocConsole();
	FILE* console = nullptr;
	freopen_s(&console, "CONOUT$", "w", stdout);

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
#endif

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCWSTR)g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ImVec2 size = oasis::project::settings.Size();
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		(LPCWSTR)g_szClassName,
		L"Oasis",
		WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(size.x), static_cast<int>(size.y),
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	oasis::imgui::window.SetHwnd(hwnd, wc);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	oasis::imgui::window.Initialize();

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
		oasis::imgui::window.Render();
	}

	DestroyWindow(hwnd);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);

#ifdef _DEBUG
	fclose(console);
#endif

	return static_cast<int>(Msg.wParam);
}