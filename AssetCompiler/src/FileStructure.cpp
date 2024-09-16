#include "AssetTypeEntry.h"

#include <corecrt_memory.h>

#include "ReserveDataStream.h"
#include "Chunks.h"

namespace oasis
{
	namespace asset
	{
		Data ConstructCfg(const std::string& a_Path)
		{
			return Data();
		}

		Data ConstructMap(const std::string& a_Path)
		{
			return Data();
		}

		Data ConstructMaterial(const std::string& a_Path)
		{
			return Data();
		}

		Data ConstructTexture(const std::string& a_Path)
		{
			TextureAssetData texHeader(a_Path);
			return texHeader.Serialize();
		}

		Data ConstructSprite(const std::string& a_Path)
		{
			return ConstructTexture(a_Path);
		}

		Data ConstructFont(const std::string& a_Path)
		{
			return ConstructTexture(a_Path);
		}

		Data ConstructSound(const std::string& a_Path)
		{
			return Data();
		}

		Data ConstructSong(const std::string& a_Path)
		{
			return ConstructSound(a_Path);
		}

		Data ConstructVO(const std::string& a_Path)
		{
			return ConstructSound(a_Path);
		}

		Data ConstructAnimation(const std::string& a_Path)
		{
			return Data();
		}

		Data ConstructLocalization(const std::string& a_Path)
		{
			return Data();
		}

		Data ConstructModel(const std::string& a_Path)
		{
			return Data();
		}
	}
}