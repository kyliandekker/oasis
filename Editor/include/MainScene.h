#pragma once

#include <core/Scene.h>

namespace oasis
{
	namespace gameplay
	{
		class MainScene : core::Scene
		{
			void Start() override;
			void Update() override;
			void Render() override;
		};
	}
}