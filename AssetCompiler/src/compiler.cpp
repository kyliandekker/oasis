#include "compiler.h"

#include <filesystem>
#include <wtypes.h>
#include <map>

#include "FileStructure.h"
#include "utils/string_extensions.h"
#include "Data.h"
#include "ReserveDataStream.h"
#include "Hash.h"

namespace fs = std::filesystem;

namespace oasis
{
	namespace compiler
	{
		void ScanFolder(std::vector<asset::IndexHeader>& a_Files, const fs::directory_entry& a_Folder)
		{
			fs::directory_iterator ds = fs::directory_iterator(a_Folder, std::filesystem::directory_options::skip_permission_denied);
			for (const auto& dirEntry : ds)
			{
				std::string pa = string_extensions::ReplaceSlashes(dirEntry.path().string().c_str());
				std::wstring s = std::wstring(pa.begin(), pa.end());

				// Check for hidden or system files.
				DWORD attributes = GetFileAttributes(s.c_str());
				if (attributes & FILE_ATTRIBUTE_HIDDEN || attributes & FILE_ATTRIBUTE_SYSTEM)
				{
					continue;
				}

				std::string folder = string_extensions::getRoot(pa);
				folder = folder.substr(0, folder.find_last_of("/"));

				if (fs::is_directory(dirEntry))
				{
					ScanFolder(a_Files, dirEntry);
				}
				else
				{
					Data data = STRUCTURE[folder].m_Function(pa);
					AssetTypeInfo& assetTypeInfo = STRUCTURE[folder];
					std::string name = string_extensions::getFileWithoutExtension(string_extensions::getFileName(pa));
					// Initialize pair (add the hash already)
					a_Files[(int)assetTypeInfo.m_AssetType].pairs.push_back(
						asset::AssetPair(
							utils::GetHash(name.c_str()),
							0, 
							0
						));
					// Add the data.
					a_Files[(int)assetTypeInfo.m_AssetType].data.push_back(data);
				}
			}
		}

		bool Compiler::Compile(const std::string& a_AssetDir)
		{
			if (!fs::exists(a_AssetDir))
			{
				return false;
			}

			if (!fs::is_directory(a_AssetDir))
			{
				return false;
			}

			std::vector<asset::IndexHeader> assets =
			{
				asset::IndexHeader(asset::CFG_HEADER_ID),
				asset::IndexHeader(asset::MAP_HEADER_ID),
				asset::IndexHeader(asset::MAT_HEADER_ID),
				asset::IndexHeader(asset::TEX_HEADER_ID),
				asset::IndexHeader(asset::SPR_HEADER_ID),
				asset::IndexHeader(asset::FONT_HEADER_ID),
				asset::IndexHeader(asset::SND_HEADER_ID),
				asset::IndexHeader(asset::SONG_HEADER_ID),
				asset::IndexHeader(asset::VO_HEADER_ID),
				asset::IndexHeader(asset::ANIM_HEADER_ID),
				asset::IndexHeader(asset::LOC_HEADER_ID),
				asset::IndexHeader(asset::MDL_HEADER_ID),
			};

			ScanFolder(assets, fs::directory_entry(a_AssetDir));

			ReserveDataStream data;

			// Determine the position of the actual game data.
			// Go through all headers and add each header size to the data pos.
			size_t dataPos = 0;
			for (auto& assetCollection : assets)
			{
				Data indexData = assetCollection.Serialize();
				dataPos += indexData.size();
			}

			// Update all pairs.
			for (auto& assetCollection : assets)
			{
				for (size_t i = 0; i < assetCollection.data.size(); i++)
				{
					auto& pair = assetCollection.pairs[i];
					pair.position = dataPos;
					pair.size = assetCollection.data[i].size();

					dataPos += pair.size;
				}
			}

			// Write all headers + pairs.
			for (auto& assetCollection : assets)
			{
				Data indexData = assetCollection.Serialize();
				data.Write(indexData.data(), indexData.size());
			}
			
			// Write all data.
			for (auto& assetCollection : assets)
			{
				Data indexData = assetCollection.SerializeData();
				data.Write(indexData.data(), indexData.size());
			}

			data.Save("D:/ekkes/test");

			return true;
		}
	}
}