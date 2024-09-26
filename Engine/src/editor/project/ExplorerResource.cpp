#ifdef __EDITOR__

#include "editor/project/ExplorerResource.h"
#include "utils/string_extensions.h"

#include <filesystem>
#include <iostream>
#include <wtypes.h>

namespace fs = std::filesystem;

namespace oasis
{
	namespace imgui
	{
		std::string ReplaceSlashes(std::string path)
		{
			std::replace(path.begin(), path.end(), '\\', '/');
			return path;
		}

		std::string GetName(std::string path)
		{
			return path.substr(path.find_last_of("/\\") + 1);
		}

		inline std::string getExtensionFromPath(std::string path, bool include_dot = false)
		{
			return path.substr(path.find_last_of(".") + (include_dot ? 0 : 1));
		}

		void ExplorerResource::Scan()
		{
			m_Path = ReplaceSlashes(m_Path);
			m_Name = GetName(m_Path);

			if (m_Name.empty())
			{
				m_Name = m_Path;
			}

			if (fs::is_directory(m_Path))
			{
				m_ExplorerResourceType = ExplorerResourceType::Folder;
				fs::directory_iterator ds = fs::directory_iterator(m_Path, std::filesystem::directory_options::skip_permission_denied);
				for (const auto& dirEntry : ds)
				{
					m_HasChildren = true;
				}
			}

			if (!fs::exists(m_Path))
			{
				return;
			}

			if (m_ExplorerResourceType == ExplorerResourceType::Folder)
			{
				m_ExplorerResources.clear();

				fs::directory_iterator ds = fs::directory_iterator(m_Path, std::filesystem::directory_options::skip_permission_denied);
				for (const auto& dirEntry : ds)
				{
					std::string pa = dirEntry.path().string().c_str();
					std::wstring s = std::wstring(pa.begin(), pa.end());

					// Check for hidden or system files.
					DWORD attributes = GetFileAttributes(s.c_str());
					if (attributes & FILE_ATTRIBUTE_HIDDEN || attributes & FILE_ATTRIBUTE_SYSTEM)
					{
						continue;
					}

					ExplorerResource ExplorerResource;
					ExplorerResource.m_Path = pa;
					ExplorerResource.m_Parent = this;

					if (!fs::is_directory(pa))
					{
						std::string extension = getExtensionFromPath(pa);
						// Check if it is a MD file.
						if (extension.compare("MD") == 0 || extension.compare("md") == 0)
						{
							ExplorerResource.m_ExplorerResourceType = ExplorerResourceType::File;
						}
						else
						{
							continue;
						}
					}

					m_ExplorerResources.push_back(ExplorerResource);
				}
			}

			for (auto& ExplorerResource : m_ExplorerResources)
			{
				ExplorerResource.Scan();
			}

			m_HasChildren = m_ExplorerResources.size() != 0;
		}

		bool ExplorerResource::Rename(const std::string& a_Name)
		{
			std::string complete_name = a_Name;
			if (!string_extensions::ends_with(a_Name, ".MD"))
			{
				complete_name = a_Name + ".MD";
			}
			std::string new_path = string_extensions::getPath(m_Path) + "/" + complete_name;
			return std::rename(m_Path.c_str(), new_path.c_str());
		}

		void ExplorerResource::Delete()
		{
			fs::remove(m_Path.c_str());
		}

		std::string ExplorerResource::GetUniqueName(const ExplorerResource& a_Resource, const std::string& a_Name)
		{
			std::string name = a_Name;
			int i = 0;
			bool found = true;
			while (found)
			{
				found = false;
				for (auto& link : a_Resource.m_ExplorerResources)
				{
					if (link.m_Name == a_Name && link.m_ExplorerResourceType == a_Resource.m_ExplorerResourceType && link.m_AssetType == a_Resource.m_AssetType)
					{
						found = true;
						i++;
					}
				}
				if (found)
				{
					name = a_Name + " (" + std::to_string(i) + ")";
				}
			}
			return name;
		}
	}
}

#endif // __EDITOR__