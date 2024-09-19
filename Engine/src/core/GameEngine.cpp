#include "core/GameEngine.h"

#include "system/Logger.h"

namespace oasis
{
	namespace gameplay
	{
		GameEngine::GameEngine()
		{
			LOG("Initializing GameEngine.");
			InitializeECS();

			//ECS.GetSystem<LifeSystem>().Initialize();
		}
	}
}