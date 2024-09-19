//#pragma once
//
//#include <string>
//
//#include "AssetType.h"
//#include "DataStream.h"
//
//namespace oasis
//{
//	namespace asset
//	{
//		class Resource
//		{
//		public:
//			DataStream m_Data;
//			AssetType m_AssetType;
//			std::string m_AssetName;
//		};
//
//		class ResourceLoader
//		{
//			bool GetResource(AssetType a_AssetType, const std::string& a_AssetName, Resource& a_Resource);
//			bool UnloadResource(const std::string& a_AssetName);
//#ifndef __EDITOR__
//			// TODO: Store the paths for the models, music, tex, etc.
//#endif
//		};
//	}
//}