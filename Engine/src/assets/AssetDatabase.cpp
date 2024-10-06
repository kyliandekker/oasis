#include "assets/AssetDatabase.h"

namespace oasis
{
	namespace assets
	{
		AssetDatabase::AssetDatabase()
		{
		}

		void AssetDatabase::ScanAssetDatabase()
		{
			ASSETDATABASE_ROOT.m_Path = "./assets";
			ASSETDATABASE_ROOT.m_FoldedOut = true;
			ASSETDATABASE_ROOT.Scan();
		}
	}
}