#include "MainScene.h"

#include <gameplay/gameplay_systems/TransformSystem.h>
#include <gameplay/EntityComponentSystem.h>

namespace oasis
{
	namespace gameplay
	{
		EntityID id;
		void MainScene::Start()
		{
			id = ECS.CreateEntity();
			auto& transComp = ECS.GetSystem<TransformSystem>().CreateComponent(id);
		}

		void MainScene::Update()
		{
		}

		void MainScene::Render()
		{
		}
	}
}