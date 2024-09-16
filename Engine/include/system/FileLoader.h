#pragma once

#include "Data.h"
#include "DataStream.h"

namespace oasis
{
	namespace file
	{
		class FileLoader
		{
		public:
			static bool LoadFile(const std::string& a_Path, DataStream& a_Data);
			static bool SaveFile(const std::string& a_Path, const Data& a_Data);
		};
	}
}