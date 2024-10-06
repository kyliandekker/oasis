#pragma once

#include <vector>
#include <string>

#include "assets/AssetType.h"

namespace oasis
{
	namespace assets
	{
		class MetaData;

#ifdef __EDITOR__
		enum class ResourceEditMode
		{
			None,
			Rename
		};
#endif // __EDITOR__

		enum class ResourceType
		{
			Folder,
			File,
		};

		class Resource
		{
		public:
			std::vector<Resource> m_Resources;
			std::string m_Path;
			std::string m_Name;
			std::string m_NameWithoutExtension;
			ResourceType m_ResourceType = ResourceType::Folder;
#ifdef __EDITOR__
			bool m_FoldedOut = false;
			bool m_Show = true;
			std::vector<std::string> m_ParentNames;
			ResourceEditMode m_ResourceEditMode = ResourceEditMode::None;
#endif // __EDITOR__
			Resource* m_Parent = nullptr;

			void Scan();
#ifdef __EDITOR__
			bool Rename(const std::string& a_Name);
			void Delete();
			static std::string GetUniqueName(const Resource& a_Resource, const std::string& a_Name);
			virtual bool GetMetaData(MetaData& a_MetaData) const;
			virtual bool LoadMetaData(MetaData& a_MetaData);
			virtual bool SaveMetaData() const;

			AssetType GetAssetType() const;
			void SetAssetType(AssetType a_AssetType);
#endif // __EDITOR__
		private:
			AssetType m_AssetType;
		};
	}
}