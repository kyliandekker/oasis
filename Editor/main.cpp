#include <windows.h>
#include <thread>
#include <glm/vec2.hpp>

#ifdef __EDITOR__
#include <editor/settings/EditorSettings.h>
#endif // __ENGINE__

#include <core/Engine.h>
#include <graphics/Window.h>
#include <system/Logger.h>
#include <gameplay/GameEngine.h>

#include "MainScene.h"

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
	oasis::core::engine.Initialize(3, hInstance, static_cast<uint32_t>(screenSize.x), static_cast<uint32_t>(screenSize.y));

	oasis::graphics::Window& window = oasis::core::engine.GetWindow();
	window.Show();

	oasis::gameplay::gameEngine.Initialize(0);

	oasis::gameplay::gameEngine.Start();

	oasis::gameplay::gameEngine.Run();
	oasis::gameplay::gameEngine.Destroy();

	return 0;
}