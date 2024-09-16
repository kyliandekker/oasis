#include "core/Engine.h"

#include <windows.h>

#include "graphics/Window.h"

namespace oasis
{
	namespace engine
	{
		graphics::Window* m_Window = nullptr;

		float FPSCounter::GetFPS()
		{
			float m_FPS = m_Frames / m_TimeAccumulation;
			constexpr float STEPINTERVAL = 4.0f; // In seconds
			constexpr float INTERVAL = 1.0f / STEPINTERVAL;
			if ((m_TimeAccumulation * INTERVAL) > 1.0f)
			{
				m_Frames = 0, m_TimeAccumulation = 0;
			}
			return m_FPS;
		}

		void FPSCounter::AddFrame()
		{
			m_TimeAccumulation += engine.GetDeltaTime();
			m_Frames++;
		}

		void Engine::Initialize(int argsN, va_list& list)
		{
			m_Window = new graphics::Window;
			std::thread windowThread(&ThreadedWindowCreation, m_Window, hInst, width, height);
			windowThread.detach();
			logger::LogInfo("Window is being created.");
			while (!WindowCreationFinished)
			{//Wait...
				std::this_thread::yield();
			}
			logger::LogInfo("Window is created.");
			CopyWindowNameToString(windowTitle);
			logger::LogInfo("RenderSystem.Initialize is called.");
			ECS.GetSystem<RenderSystem>().Initialize(m_Window->GetHWnd(), width, height, false);
#ifdef __ENGINE__
			logger::LogInfo("GUISystem.Initialize is called.");
			ECS.GetSystem<GUISystem>().Initialize(m_Window->GetHWnd());
#endif
			logger::LogInfo("ResourceManager.LoadAllResources is called.");
			ECS.GetSystem<ResourceManager>().LoadAllResources();
		}
	}
}