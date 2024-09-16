#pragma once

#include <vector>
#include <string>

#include "AssetType.h"
#include "Data.h"

namespace oasis
{
	namespace asset
	{
		Data ConstructCfg(const std::string& a_Path);
		Data ConstructMap(const std::string& a_Path);
		Data ConstructMaterial(const std::string& a_Path);
		Data ConstructTexture(const std::string& a_Path);
		Data ConstructSprite(const std::string& a_Path);
		Data ConstructFont(const std::string& a_Path);
		Data ConstructSound(const std::string& a_Path);
		Data ConstructSong(const std::string& a_Path);
		Data ConstructVO(const std::string& a_Path);
		Data ConstructAnimation(const std::string& a_Path);
		Data ConstructLocalization(const std::string& a_Path);
		Data ConstructModel(const std::string& a_Path);
	}
}