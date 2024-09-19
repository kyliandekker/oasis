#include <windows.h>
#include <thread>
#include <glm/vec2.hpp>

#ifdef __EDITOR__
#include <editor/settings/EditorSettings.h>
#endif // __ENGINE__

#include <core/Engine.h>
#include <graphics/Window.h>
#include <system/Logger.h>


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nShowCmd)
{
	while (!oasis::logger::logger.Running())
	{

	}

	glm::vec2 screenSize(1920, 1080);

#ifdef __EDITOR__
	oasis::settings::settings.Load();
	screenSize = oasis::settings::settings.Size();
#endif
	oasis::engine::engine.Initialize(hInstance, screenSize.x, screenSize.y);

	oasis::graphics::Window& window = oasis::engine::engine.GetWindow();
	window.Show();

	// Main thread will be gameplay.
	while (true)
	{
	}

	return 0;
}