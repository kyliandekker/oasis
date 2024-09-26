#pragma once

namespace oasis
{
	namespace gameplay
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
	}
}