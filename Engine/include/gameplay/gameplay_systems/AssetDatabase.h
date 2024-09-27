#pragma once

#include <vector>
#include <string>

namespace oasis
{
	namespace asset
	{
		class AssetDatabase
		{
		public:
			static void ScanAssetDatabase();
			static std::vector<std::string> m_Assets;
		};
	}
}