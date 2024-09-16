#pragma once

#include <chrono>
#include <stdint.h>
#include <vector>

namespace oasis
{
	namespace engine
	{
		class Scene;

		using SecondsDuration = std::chrono::duration<float, std::ratio<1>>;

		class FPSCounter
		{
		public:
			float GetFPS();
			void AddFrame();
		private:
			uint64_t m_Frames = 0;
			float m_TimeAccumulation = 0.0f;
		};

		class Engine
		{
		public:
			void Initialize(int argsN, va_list& list);
			void PollScreen();

			void Finalize();
			void FinalizeBase();

			void AddScene(Scene* a_Scene);
			void RemoveScene(Scene* a_Scene);

			Scene* GetSceneById(uint64_t a_Index) const;

			float GetDeltaTime();
			float GetTime();
			std::chrono::steady_clock::time_point GetStartTime();

			void UpdateDeltaTime();
			void UpdateScenes();

			void UpdatePlayer();
			FPSCounter m_FPSCounter;

			std::vector<Scene*> m_ActiveScenes;
			float m_DeltaTime = 0.0f;
			bool m_Update = true;

			std::chrono::steady_clock::time_point m_TimeSinceAppStart = std::chrono::steady_clock::now();
			std::chrono::steady_clock::time_point m_LastFrame = std::chrono::steady_clock::now();
		};

		extern Engine engine;
	}
}