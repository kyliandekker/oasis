#pragma once

#include <cstdint>

namespace low_level
{
	inline void* add(void* a_Ptr, size_t a_Size)
	{
		return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(a_Ptr) + a_Size);
	}

	template <typename T>
	inline T* temp_ptr(T value) {
		return &value;
	}
}