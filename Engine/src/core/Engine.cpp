#include "core/Engine.h"

#include <windows.h>

#include "graphics/Window.h"
#include "system/Logger.h"
#include <cstdarg>
#include "core/Scene.h"
#include "assets/AssetDatabase.h"

namespace oasis
{
	namespace core
	{
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

		FILE* console = nullptr;

        bool Engine::Initialize(int nArgs, ...)
        {
			va_list list;
			va_start(list, nArgs);

			HINSTANCE& hInst = va_arg(list, HINSTANCE);
			uint32_t& width = va_arg(list, uint32_t);
			uint32_t& height = va_arg(list, uint32_t);

			va_end(list);

#ifdef _DEBUG
			AllocConsole();
			freopen_s(&console, "CONOUT$", "w", stdout);

			HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD dwMode = 0;
			GetConsoleMode(hOut, &dwMode);
			dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			SetConsoleMode(hOut, dwMode);
#endif
			std::thread windowThread(&graphics::ThreadedWindowCreation, &m_Window, hInst, width, height);
			windowThread.detach();

			while (!m_Window.Ready())
			{
				// Wait...
				std::this_thread::yield();
			}
			LOGF(logger::LOGSEVERITY_SUCCESS, "Engine initialized.");

			assets::AssetDatabase::ScanAssetDatabase();

			//			CopyWindowNameToString(windowTitle);
			//			logger::LogInfo("RenderSystem.Initialize is called.");
			//			ECS.GetSystem<RenderSystem>().Initialize(m_Window->GetHWnd(), width, height, false);
			//#ifdef __ENGINE__
			//			logger::LogInfo("GUISystem.Initialize is called.");
			//			ECS.GetSystem<GUISystem>().Initialize(m_Window->GetHWnd());
			//#endif
			//			logger::LogInfo("ResourceManager.LoadAllResources is called.");
			//			ECS.GetSystem<ResourceManager>().LoadAllResources();

			return true;
        }

		bool Engine::Destroy()
		{
			for (Scene* scene : m_ActiveScenes)
			{
				RemoveScene(scene);
			}
			m_ActiveScenes.clear();
			m_Update = false;

			m_Window.Destroy();

#ifdef _DEBUG
			fclose(console);
#endif
			return true;
		}

		void Engine::RemoveScene(Scene* a_Scene)
		{
			std::vector<Scene*>::iterator it = m_ActiveScenes.begin();
			while (it != m_ActiveScenes.end())
			{
				if (*it == a_Scene)
				{
					Scene* toBeDeleted = *it;
					m_ActiveScenes.erase(it);
					DeleteObject(toBeDeleted);
					break;
				}
			}
		}

		float Engine::GetDeltaTime()
		{
			return m_DeltaTime;
		}

		float Engine::GetTime()
		{
			SecondsDuration elapsed = std::chrono::duration_cast<SecondsDuration>(std::chrono::steady_clock::now() - m_TimeSinceAppStart);
			return elapsed.count();
		}

		std::chrono::steady_clock::time_point Engine::GetStartTime()
		{
			return m_TimeSinceAppStart;
		}

		graphics::Window& Engine::GetWindow()
		{
			return m_Window;
		}

		void Engine::UpdateDeltaTime()
		{
			std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
			SecondsDuration elapsed = std::chrono::duration_cast<SecondsDuration>(now - m_LastFrame);

			m_LastFrame = now;
			m_DeltaTime = elapsed.count();
			engine.m_FPSCounter.AddFrame();
		}

		void Engine::UpdateScenes()
		{
			for (auto* scene : m_ActiveScenes)
			{
				if (scene)
				{
					if (scene->m_IsPaused)
					{
						break;
					}
					scene->Update();
					scene->Render();
				}
			}
		}
	}
}