#pragma once

namespace oasis
{
	namespace gameplay
	{
		class GameEngine
		{
		public:
			GameEngine();
			~GameEngine() = default; //default for now

			void Initialize(int nArgs, ...);
			void Start();
			void Run();
			void Finalize();
		private:
			void InitializeECSShared();
			void InitializeECS();

			//Platform specific function call
			void RenderFrame();
			void LoadEntityTags();
		};
		extern GameEngine gameEngine;
	}
}