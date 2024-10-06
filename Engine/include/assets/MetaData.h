#pragma once

#include "assets/AssetType.h"

namespace oasis
{
	namespace assets
	{
		class MetaData
		{
		public:
			AssetType m_AssetType;

			virtual bool Save(const std::string& a_Path) const;
			virtual bool Load(const std::string& a_Path);
		};
	}
}