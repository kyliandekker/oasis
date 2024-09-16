#include "imgui/ImGuiDefines.h"

#include <cstdarg>
#include <format>

#include "Data.h"

namespace oasis
{
	namespace imgui
	{
		std::string IMGUI_FORMAT_ID(std::string a_Text, const char* a_ID, ...)
		{
			va_list va_format_list;
			va_start(va_format_list, a_ID);

			const size_t buffersize = vsnprintf(NULL, 0, a_ID, va_format_list) + 1;
			Data formatted_message = Data(buffersize);
			vsnprintf(reinterpret_cast<char*>(formatted_message.data()), buffersize, a_ID, va_format_list);

			std::string full_id = a_Text + std::string(reinterpret_cast<char*>(formatted_message.data()));

			return full_id;
		}
	}
}