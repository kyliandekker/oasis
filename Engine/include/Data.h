#pragma once

#include <cstdint>
#include <string>

namespace oasis
{
	class Data
	{
	public:
		Data() = default;
		Data(const void* a_Data, size_t a_Size);
		Data(const char* a_Data, size_t a_Size);
		Data(size_t a_Size);
		Data(const Data& rhs);
		~Data();

		Data& operator=(const Data& a_Other);

		const size_t size() const
		{
			return m_Size;
		}

		void* data() const
		{
			return m_Data;
		}

		virtual void Free();

		bool Save(const std::string& a_Path) const;

		unsigned char operator [] (int i) const { return reinterpret_cast<unsigned char*>(m_Data)[i]; }
		unsigned char& operator [] (int i) { return reinterpret_cast<unsigned char*>(m_Data)[i]; }
	protected:
		void* m_Data = nullptr;
		size_t m_Size = 0;
	};
}