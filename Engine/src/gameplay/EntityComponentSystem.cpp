#include "gameplay/EntityComponentSystem.h"

namespace oasis
{
	namespace gameplay
	{
		EntityComponentSystem::EntityComponentSystem()
		{
		}

		EntityComponentSystem::~EntityComponentSystem()
		{
		}

		void EntityComponentSystem::Update(const float& a_DeltaTime)
		{ }

		bool EntityComponentSystem::IsPaused() const
		{
			return m_Paused;
		}

		void EntityComponentSystem::SetPaused(bool a_Paused)
		{
			m_Paused = a_Paused;
		}

		EntityID& EntityComponentSystem::CreateEntity()
		{
			const size_t SizeVector = m_Entities.size();
			EntityID id(++m_NextID);
			m_Entities.push_back(id);
			return m_Entities[SizeVector];
		}

		void EntityComponentSystem::DeleteEntity(EntityID a_ID)
		{
			a_ID.SetInvalid();
			for (auto& sys : m_Systems)
			{
				sys->DeleteComponent(a_ID);
			}
		}

		bool EntityComponentSystem::IsEntityValid(EntityID& a_ID) const
		{
			return a_ID.IsValid();
		}

		std::vector<GameplaySystem*>& EntityComponentSystem::GetSystems()
		{
			return m_Systems;
		}
	}
}