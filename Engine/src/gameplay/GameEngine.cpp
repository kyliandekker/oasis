#include "gameplay/GameEngine.h"

#include "system/Logger.h"
#include "core/Engine.h"
#include "gameplay/EntityComponentSystem.h"
#include "gameplay/gameplay_systems/TransformSystem.h"

namespace oasis
{
	namespace gameplay
	{
		GameEngine::GameEngine()
		{
			LOG("Initializing GameEngine.");
			InitializeECS();
		}

		bool GameEngine::Initialize(int nArgs, ...)
		{
			return false;
		}

		bool GameEngine::Destroy()
		{
			return false;
		}

		void GameEngine::Start()
		{
		}

		void GameEngine::Run()
		{
			// Update the deltatime at the start.
			core::engine.UpdateDeltaTime();

			while (core::engine.m_Update)
			{
				core::engine.UpdateDeltaTime();

				ECS.Update(core::engine.m_DeltaTime);

				RenderFrame();
			}
		}

		void GameEngine::InitializeECS()
		{
			ECS.CreateSystem<TransformSystem>(transform);
		}

		void GameEngine::RenderFrame()
		{
		}

		void GameEngine::LoadEntityTags()
		{
		}
	}
}