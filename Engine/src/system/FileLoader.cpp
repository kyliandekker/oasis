#include "system/FileLoader.h"

#include <filesystem>
#include <ShlObj_core.h>

namespace fs = std::filesystem;

namespace oasis
{
	namespace file
	{
		bool FileLoader::LoadFile(const std::string& a_Path, DataStream& a_Data)
		{
			FILE* file = nullptr;
			fopen_s(&file, a_Path.c_str(), "rb");
			if (!file)
			{
				return false;
			}

			fseek(file, 0, SEEK_END);
			size_t fileSize = ftell(file);
			rewind(file);

			a_Data = DataStream(fileSize);
			fread(a_Data.data(), fileSize, 1, file);

			fclose(file);

			return true;
		}

		bool FileLoader::SaveFile(const std::string& a_Path, const Data& a_Data)
		{
			FILE* file = nullptr;
			fopen_s(&file, a_Path.c_str(), "wb");
			if (!file)
			{
				return false;
			}

			fwrite(a_Data.data(), a_Data.size(), 1, file);

			fclose(file);

			return true;
		}

        bool FileLoader::CreateFolder(const std::string& a_Path)
        {
			bool success = fs::create_directories(a_Path);
            return false;
        }
		bool FileLoader::OpenInExplorer(const std::string& a_Path)
		{
			ShellExecuteA(NULL, "open", a_Path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
			return false;
		}
	}
}