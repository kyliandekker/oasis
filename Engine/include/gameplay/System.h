#pragma once

#include <map>

#include "gameplay/EntityID.h"
#include "system/Logger.h"

namespace oasis
{
	namespace gameplay
	{
		class GameplaySystem
		{
		public:
			GameplaySystem(unsigned int priority) : Priority(priority) {}

			virtual void DeleteComponent(EntityID& id) { (void)id; };
			virtual void Update(float deltaTime) = 0;
			virtual bool IsIdHere(EntityID& id)
			{
				(void)id;
				return false;
			};
			virtual bool HasComponents() { return false; };
			virtual void* GetComponentData(EntityID& id, size_t& size) { (void)id; size = 0; return nullptr; };
			const unsigned int Priority = 0;
		};

		template <class ComponentType>
		class System : public GameplaySystem
		{
		public:
			System(unsigned int priority) : GameplaySystem(priority) {}
			virtual ~System()
			{
				m_Components.clear();
			};
			template <typename... Args>
			ComponentType& CreateComponent(EntityID& id, Args... args)
			{
				assert(!IsIdHere(id));
				ComponentType t(args...);
				m_Components.insert(std::make_pair(id, t));
				return m_Components[id];
			};

			size_t GetSize()
			{
				return m_Components.size();
			}

			bool IsIdHere(EntityID& id)
			{
				return m_Components.count(id) == 1;
			}

			ComponentType& GetComponent(EntityID& id)
			{
				if (!id.IsValid())
				{
					LOGF(logger::LOGSEVERITY_ASSERT, "This entity id could not be found in the system!");
				}
				return m_Components.at(id);
			}

			ComponentType* TryGet(EntityID& id)
			{
				if (!IsIdHere(id))
					return nullptr;
				else
					return &m_Components.at(id);
			};

			void* GetComponentData(EntityID& id, size_t& size)
			{
				ComponentType& comp = GetComponent(id);
				size = sizeof(ComponentType);
				return &comp;
			};
			bool HasComponents() override { return true; };

			virtual void DeleteComponent(EntityID& id)
			{
				if (id.IsValid() && IsIdHere(id))
				{
					m_Components.erase(id);
				}
				return;
			};

		protected:
			std::map<EntityID, ComponentType> m_Components;
		};
	}
}