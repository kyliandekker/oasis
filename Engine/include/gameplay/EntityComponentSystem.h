#pragma once

#include <vector>
#include <algorithm>

#include "gameplay/EntityID.h"
#include "gameplay/System.h"

namespace oasis
{
	namespace gameplay
	{
		class EntityComponentSystem
		{
		public:
			EntityComponentSystem();
			~EntityComponentSystem();
			void Update(const float& a_DeltaTime);

			bool IsPaused() const;
			void SetPaused(bool a_Paused);

			EntityID& CreateEntity();
			void DeleteEntity(EntityID a_ID);
			bool IsEntityValid(EntityID& a_ID) const;

			template <class T, typename... Args>
			T& CreateSystem(unsigned int priority, Args... args)
			{
				// Todo: Find out why this does not work with NewObject<>.
				T* system = new T(priority, args...);
				m_Systems.push_back(system);
				std::sort(m_Systems.begin(), m_Systems.end(), [](const GameplaySystem* sl, const GameplaySystem* sr)
					{ return sl->Priority > sr->Priority; });
				return *system;
			}
			template <class T>
			T& GetSystem()
			{
				for (GameplaySystem* sys : m_Systems)
				{
					T* result = dynamic_cast<T*>(sys);
					if (result)
						return *result;
				}
				assert(false);
				return *dynamic_cast<T*>(m_Systems[0]);
			};

			std::vector<GameplaySystem*>& GetSystems();

			template <class... Ts>
			std::vector<EntityID>& GetView()
			{
				m_View.clear();

				std::vector<GameplaySystem*> list;
				constexpr auto size = sizeof...(Ts);

				GetNextSystem<Ts...>(list);

				bool result{ false };
				for (auto& id : m_Entities)
				{
					for (auto& sys : list)
					{
						result = sys->IsIdHere(id);
						if (!result)
							break;
					}
					if (result)
						m_View.push_back(id);
				}
				return m_View;
			};

		private:
			template <class Last>
			void GetNextSystem(std::vector<GameplaySystem*>& list)
			{
				list.push_back(&GetSystem<Last>());
			}

			template <class First, class Second, class... Others>
			void GetNextSystem(std::vector<GameplaySystem*>& list)
			{
				list.push_back(&GetSystem<First>());
				GetNextSystem<Second, Others...>(list);
			}

		private:
			std::vector<GameplaySystem*> m_Systems;
			std::vector<EntityID> m_Entities;
			std::vector<EntityID> m_View;
			unsigned int m_NextID = 0;
			bool m_Paused = false;
		};
		inline extern EntityComponentSystem ECS = {};
	}
}