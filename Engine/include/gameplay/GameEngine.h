#pragma once

#include "core/BaseSystem.h"

namespace oasis
{
	namespace gameplay
	{
		// Fix for update order.
		enum priorityList : unsigned int
		{
			render,
			resource,
			transform,
			animation,
			collision,
			audio,
			pathfinding,
			BasicAI,
			userinterface
		};

		class GameEngine : public core::BaseSystem
		{
		public:
			GameEngine();
			~GameEngine() = default; //default for now

			bool Initialize(int nArgs, ...) override;
			bool Destroy() override;

			void Start();
			void Run();
		private:
			void InitializeECS();

			//Platform specific function call
			void RenderFrame();
			void LoadEntityTags();
		};
		inline extern GameEngine gameEngine = {};
	}
}