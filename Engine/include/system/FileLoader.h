#pragma once

#include <string>

namespace oasis
{
	class Data;
	class DataStream;

	namespace file
	{
		class FileLoader
		{
		public:
			static bool LoadFile(const std::string& a_Path, DataStream& a_Data);
			static bool SaveFile(const std::string& a_Path, const Data& a_Data);
			static bool DoesFileExist(const std::string& a_Path);
			static bool CreateFolder(const std::string& a_Path);
			static bool OpenInExplorer(const std::string& a_Path);
		};
	}
}