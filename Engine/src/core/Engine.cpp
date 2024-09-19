#include "core/Engine.h"

#include <windows.h>

#include "graphics/Window.h"
#include "system/Logger.h"
#include "core/Scene.h"

namespace oasis
{
	namespace engine
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
		bool Engine::Initialize(HINSTANCE a_hInstance, uint32_t a_Width, uint32_t a_Height)
		{
#ifdef _DEBUG
			AllocConsole();
			freopen_s(&console, "CONOUT$", "w", stdout);

			HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD dwMode = 0;
			GetConsoleMode(hOut, &dwMode);
			dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			SetConsoleMode(hOut, dwMode);
#endif
			std::thread windowThread(&graphics::ThreadedWindowCreation, &m_Window, a_hInstance, a_Width, a_Height);
			windowThread.detach();

			while (!m_Window.Ready())
			{
				// Wait...
				std::this_thread::yield();
			}
			LOGF(logger::LOGSEVERITY_SUCCESS, "Engine initialized.");

			return true;

//			CopyWindowNameToString(windowTitle);
//			logger::LogInfo("RenderSystem.Initialize is called.");
//			ECS.GetSystem<RenderSystem>().Initialize(m_Window->GetHWnd(), width, height, false);
//#ifdef __ENGINE__
//			logger::LogInfo("GUISystem.Initialize is called.");
//			ECS.GetSystem<GUISystem>().Initialize(m_Window->GetHWnd());
//#endif
//			logger::LogInfo("ResourceManager.LoadAllResources is called.");
//			ECS.GetSystem<ResourceManager>().LoadAllResources();
		}

		void Engine::PollScreen()
		{
		}

		void Engine::Finalize()
		{
			FinalizeBase();

#ifdef __ENGINE__
			//ECS.GetSystem<GUISystem>().Finalize();
#endif
			//ECS.GetSystem<RenderSystem>().Finalize();
			//m_Window->Finalize();

#ifdef _DEBUG
			fclose(console);
#endif
		}

		void Engine::FinalizeBase()
		{
			for (Scene* scene : m_ActiveScenes)
			{
				RemoveScene(scene);
			}
			m_ActiveScenes.clear();
			m_Update = false;
		}

		void Engine::AddScene(Scene* a_Scene)
		{
			m_ActiveScenes.push_back(a_Scene);
			a_Scene->Start();
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
					delete toBeDeleted;
					break;
				}
			}
			if (a_Scene != nullptr)
			{
				LOGF(logger::LOGSEVERITY_ERROR, "Could not find scene when trying to remove scene.");
			}
		}

		Scene* Engine::GetSceneById(uint64_t a_Index) const
		{
			return m_ActiveScenes.at(a_Index);
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
			for (auto& iter : m_ActiveScenes)
			{
				if (iter->m_IsPaused)
				{
					break;
				}
				iter->Update();
				iter->Render();
			}
		}
	}
}