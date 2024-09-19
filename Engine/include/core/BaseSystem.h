#pragma once

namespace oasis
{
	namespace core
	{
		struct EntityID
		{
			EntityID(unsigned int id) : m_ID(id) {};
			EntityID() {};
			~EntityID() = default;

			bool IsValid() const { return m_ID != INVALID; };
			void SetInvalid() { m_ID = INVALID; }

			unsigned int GetID() const { return m_ID; }

			bool operator==(const EntityID& other) const
			{
				return m_ID == other.m_ID;
			}

			bool operator!=(const EntityID& other) const
			{
				return m_ID != other.m_ID;
			}

			bool operator<(const EntityID& other) const
			{
				return m_ID < other.m_ID;
			}

		protected:
			enum ID_State : unsigned int
			{
				INVALID = 0
			};
			unsigned int m_ID = INVALID;
		};

		class BaseSystem
		{
		public:
			BaseSystem(unsigned int priority) : Priority(priority) {}

			virtual void Destroy() = 0;
			virtual void DeleteComponent(EntityID id) { (void)id; };
			virtual void Update(float deltaTime) = 0;
			virtual bool HasComponents() { return false; };
			virtual void* GetComponentData(EntityID id, size_t& size) { (void)id; size = 0; return nullptr; };
			const unsigned int Priority = 0;
		};
	}
}