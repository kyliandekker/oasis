//#include "gameplay_systems/ResourceLoader.h"
//
//#ifdef __EDITOR__
//#include "system/FileLoader.h"
//#include "FileStructure.h"
//#endif
//
//namespace oasis
//{
//	namespace asset
//	{
//		bool ResourceLoader::GetResource(AssetType a_AssetType, const std::string& a_AssetName, Resource& a_Resource)
//		{
//#ifdef __EDITOR__
//			std::string path = ;
//
//			a_Resource.m_AssetName = a_AssetName;
//			a_Resource.m_AssetType = a_AssetType;
//
//			return file::FileLoader::LoadFile(path, a_Resource.m_Data);
//#elif __GAME__
//			return false;
//#endif
//		}
//
//		bool ResourceLoader::UnloadResource(const std::string& a_AssetName)
//		{
//			return false;
//		}
//	}
//}