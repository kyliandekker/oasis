#pragma once

#include <string>
#include <algorithm>

namespace oasis
{
	namespace string_extensions
	{
		// TODO: Move into cpp.

		inline std::string ReplaceSlashes(std::string path)
		{
			std::replace(path.begin(), path.end(), '\\', '/');
			return path;
		}

		inline std::string getExtensionFromPath(std::string a_Path, bool a_IncludeDot = false)
		{
			return a_Path.substr(a_Path.find_last_of(".") + (a_IncludeDot ? 0 : 1));
		}

		inline std::string getFileWithoutExtension(std::string a_Path)
		{
			return a_Path.substr(0, ReplaceSlashes(a_Path).find_last_of("."));
		}

		inline std::string getFileName(std::string a_Path)
		{
			return a_Path.substr(a_Path.find_last_of("/") + 1);
		}

		inline int strpos(const char* haystack, const char* needle, int nth)
		{
			const char* res = haystack;
			for (int i = 1; i <= nth; i++)
			{
				res = strstr(res, needle);
				if (!res)
					return -1;
				else if (i != nth)
					res++;
			}
			return res - haystack;
		}

		inline std::string getRoot(std::string a_Path)
		{
			return a_Path.substr(strpos(a_Path.c_str(), "/", 2) + 1);
		}

		inline bool ends_with(std::string const& value, std::string const& ending)
		{
			if (ending.size() > value.size()) return false;
			return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
		}

		inline bool starts_with(std::string const& value, std::string const& starting)
		{
			return (value.rfind(starting, 0) == 0);
		}

		inline std::string StringToUpper(std::string strToConvert)
		{
			std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);

			return strToConvert;
		}

		inline std::string StringToLower(std::string strToConvert)
		{
			std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::tolower);

			return strToConvert;
		}

		inline std::string getPath(std::string a_Path)
		{
			std::string f = a_Path.substr(0, a_Path.find_last_of("\\"));
			return f.substr(0, a_Path.find_last_of("/"));
		}
	}
}