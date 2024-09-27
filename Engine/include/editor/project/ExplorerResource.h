#pragma once

#ifdef __EDITOR__

#include <vector>
#include <string>

#include "AssetType.h"

namespace oasis
{
	namespace imgui
	{
		enum class ExplorerResourceEditMode
		{
			None,
			Rename
		};

		enum class ExplorerResourceType
		{
			Folder,
			File,
		};

		class ExplorerResource
		{
		public:
			std::vector<ExplorerResource> m_ExplorerResources;
			std::string m_Path;
			std::string m_Name;
			bool m_FoldedOut = false;
			bool m_Show = true;
			bool m_HasChildren = false;
			ExplorerResourceType m_ExplorerResourceType = ExplorerResourceType::Folder;
			asset::AssetType m_AssetType;
			ExplorerResource* m_Parent = nullptr;
			std::vector<std::string> m_ParentNames;
			ExplorerResourceEditMode m_ResourceEditMode = ExplorerResourceEditMode::None;

			void Scan();
			bool Rename(const std::string& a_Name);
			void Delete();
			static std::string GetUniqueName(const ExplorerResource& a_Resource, const std::string& a_Name);
		};
	}
}

#endif // __EDITOR__