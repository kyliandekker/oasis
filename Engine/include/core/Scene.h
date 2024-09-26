#pragma once

namespace oasis
{
	namespace core
	{
		class Scene
		{
		public:
			virtual ~Scene() {}
			virtual void Start() = 0;
			virtual void Update() = 0;
			virtual void Render() = 0;
			bool m_IsPaused = false;
		};
	}
}