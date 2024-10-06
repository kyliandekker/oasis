#pragma once

#include "assets/Resource.h"

namespace oasis
{
	namespace assets
	{
		class AssetDatabase
		{
		public:
			AssetDatabase();
			static void ScanAssetDatabase();
		};
		inline extern Resource ASSETDATABASE_ROOT = Resource();
	}
}