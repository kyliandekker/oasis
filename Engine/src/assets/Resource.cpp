#include "assets/Resource.h"

#include <filesystem>
#include <wtypes.h>

// TODO: Remove.
#include <unordered_map>

#include "utils/string_extensions.h"
#include "assets/MetaData.h"

namespace fs = std::filesystem;

namespace oasis
{
	namespace assets
	{
		std::vector<std::string> GetParentNames(const std::string& path)
		{
			std::stringstream ss(path);
			std::string token;
			std::vector<std::string> folders;

			while (std::getline(ss, token, '/'))
			{
				if (token != ".")
				{
					folders.push_back(token);
				}
			}
			return folders;
		}

		void Resource::Scan()
		{
			// TODO: Fix
			const std::unordered_map<std::string, std::vector<assets::AssetType>> WAT =
			{
				{ "cfg", { assets::AssetType::Cfg }  },
				{ "scene", { assets::AssetType::Scene }  },
				{ "mat", { assets::AssetType::Material }  },
				{ "png", { assets::AssetType::Texture, assets::AssetType::Sprite, assets::AssetType::Font }  },
				{ "tif", { assets::AssetType::Texture, assets::AssetType::Sprite, assets::AssetType::Font }  },
				{ "bmp", { assets::AssetType::Texture, assets::AssetType::Sprite, assets::AssetType::Font }  },
				{ "wav", { assets::AssetType::Sound, assets::AssetType::Song }  },
				{ "anim", { assets::AssetType::Animation }  },
				{ "loc", { assets::AssetType::Localization }  },
				{ "gltf", { assets::AssetType::Model } },
			};

			// Replace the slashes so that all paths use the same slash.
			m_Path = string_extensions::ReplaceSlashes(m_Path);

			if (!fs::exists(m_Path))
			{
				return;
			}

			m_Name = string_extensions::getFileName(m_Path);
			m_NameWithoutExtension = string_extensions::getFileWithoutExtension(m_Name);

			if (m_Name.empty())
			{
				m_Name = m_Path;
			}

			// This is for the editor so that we can show the path like this: "Root -> Images -> Haan.png"
#ifdef __EDITOR__
			m_ParentNames = GetParentNames(m_Path);
#endif // __EDITOR__

			m_ResourceType = fs::is_directory(m_Path) ? ResourceType::Folder : ResourceType::File;

			// Scan the folder.
			if (m_ResourceType == ResourceType::Folder)
			{
				// Clear all resources first.
				m_Resources.clear();

				// Go through each file/folder and check their status.
				fs::directory_iterator ds = fs::directory_iterator(m_Path, std::filesystem::directory_options::skip_permission_denied);
				for (const auto& dirEntry : ds)
				{
					std::string pa = dirEntry.path().string().c_str();
					std::wstring s = std::wstring(pa.begin(), pa.end());

					// Check for hidden or system files. These should not be added.
					DWORD attributes = GetFileAttributes(s.c_str());
					if (attributes & FILE_ATTRIBUTE_HIDDEN || attributes & FILE_ATTRIBUTE_SYSTEM)
					{
						continue;
					}

					// Create the resource that will be added.
					Resource explorerResource;
					explorerResource.m_Path = pa;
					explorerResource.m_Parent = this;

					// If it is not a directory, it is a file and needs to get past the meta checks.
					if (!fs::is_directory(pa))
					{
						explorerResource.m_ResourceType = ResourceType::File;

						std::string extension = string_extensions::StringToLower(string_extensions::getExtensionFromPath(pa));

						// Get the extension. If the extension is not recognized, it will just be ignored.
						if (WAT.find(extension) == WAT.end())
						{
							continue;
						}

						auto it = WAT.find(extension);

						// Default asset type assignment.
						explorerResource.m_AssetType = it->second[0];

						if (it->second.size() > 1)
						{
							MetaData metaData;
							bool hasMetaData = explorerResource.GetMetaData(metaData);
							if (hasMetaData)
							{
								explorerResource.LoadMetaData(metaData);
							}
							else
							{
								explorerResource.SaveMetaData();
							}
						}
					}

					m_Resources.push_back(explorerResource);
				}
			}

			for (auto& Resource : m_Resources)
			{
				Resource.Scan();
			}
		}

#ifdef __EDITOR__
		bool Resource::Rename(const std::string& a_Name)
		{
			std::string complete_name = a_Name;
			if (!string_extensions::ends_with(a_Name, ".MD"))
			{
				complete_name = a_Name + ".MD";
			}
			std::string new_path = string_extensions::getPath(m_Path) + "/" + complete_name;
			return std::rename(m_Path.c_str(), new_path.c_str());
		}

		void Resource::Delete()
		{
			fs::remove(m_Path.c_str());
		}

		std::string Resource::GetUniqueName(const Resource& a_Resource, const std::string& a_Name)
		{
			std::string name = a_Name;
			int i = 0;
			bool found = true;
			while (found)
			{
				found = false;
				for (auto& link : a_Resource.m_Resources)
				{
					if (link.m_Name == a_Name && link.m_ResourceType == a_Resource.m_ResourceType && link.m_AssetType == a_Resource.m_AssetType)
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

		bool Resource::GetMetaData(MetaData& a_MetaData) const
		{
			return a_MetaData.Load(m_Path + ".meta");
		}

		bool Resource::LoadMetaData(MetaData& a_MetaData)
		{
			m_AssetType = a_MetaData.m_AssetType;
			return true;
		}

		bool Resource::SaveMetaData() const
		{
			MetaData data;
			data.m_AssetType = m_AssetType;
			return data.Save(m_Path + ".meta");
		}
		AssetType Resource::GetAssetType() const
		{
			return m_AssetType;
		}

		void Resource::SetAssetType(AssetType a_AssetType)
		{
			m_AssetType = a_AssetType;

			SaveMetaData();
		}
#endif // __EDITOR__
	}
}