#pragma once

#include <map>
#include <string>
#include <ShlObj_core.h>
#include <vector>

#include "AssetType.h"

namespace oasis
{
	//inline std::string AssetTypeToChunkId(AssetType assetType)
	//{
	//	switch (assetType)
	//	{
	//		case AssetType::Cfg:
	//		{
	//			return CFG_HEADER_ID;
	//		}
	//		case AssetType::Map:
	//		{
	//			return MAP_HEADER_ID;
	//		}
	//		case AssetType::Material:
	//		{
	//			return MAT_HEADER_ID;
	//		}
	//		case AssetType::Texture:
	//		{
	//			return TEX_HEADER_ID;
	//		}
	//		case AssetType::Sprite:
	//		{
	//			return SPR_HEADER_ID;
	//		}
	//		case AssetType::Font:
	//		{
	//			return FONT_HEADER_ID;
	//		}
	//		case AssetType::Sound:
	//		{
	//			return SND_HEADER_ID;
	//		}
	//		case AssetType::Song:
	//		{
	//			return SONG_HEADER_ID;
	//		}
	//		case AssetType::VO:
	//		{
	//			return VO_HEADER_ID;
	//		}
	//		case AssetType::Animation:
	//		{
	//			return ANIM_HEADER_ID;
	//		}
	//		case AssetType::Localization:
	//		{
	//			return LOC_HEADER_ID;
	//		}
	//		case AssetType::Model:
	//		{
	//			return MDL_HEADER_ID;
	//		}
	//	}
	//	return "";
	//}

	struct AssetTypeInfo
	{
		asset::AssetType m_AssetType;
		std::vector<std::string> m_Extensions;
		Data(*m_Function)(const std::string&);
	};

	namespace asset
	{
		Data ConstructCfg(const std::string&);
		Data ConstructMap(const std::string&);
		Data ConstructMaterial(const std::string&);
		Data ConstructTexture(const std::string&);
		Data ConstructSprite(const std::string&);
		Data ConstructFont(const std::string&);
		Data ConstructSound(const std::string&);
		Data ConstructSong(const std::string&);
		Data ConstructVO(const std::string&);
		Data ConstructAnimation(const std::string&);
		Data ConstructLocalization(const std::string&);
		Data ConstructModel(const std::string&);
	}

	// This is used for loading assets in the actual asset folder and packaging them.
	const std::map<std::string, AssetTypeInfo> STRUCTURE =
	{
		{ "cfg", { asset::AssetType::Cfg, { "cfg" }, &asset::ConstructCfg }},
		{ "maps", { asset::AssetType::Map, { "map" }, &asset::ConstructMap }},
		{ "materials", { asset::AssetType::Material, { "mat" }, &asset::ConstructMaterial }},
		{ "textures", { asset::AssetType::Texture, { "png","bmp","jpg","jpeg" }, &asset::ConstructTexture }},
		{ "sprites", { asset::AssetType::Sprite, { "png","bmp","jpg","jpeg" }, &asset::ConstructSprite }},
		{ "fonts", { asset::AssetType::Font, { "png","bmp","jpg","jpeg" }, &asset::ConstructFont }},
		{ "sounds", { asset::AssetType::Sound, { "wav" }, &asset::ConstructSound }},
		{ "music", { asset::AssetType::Song, { "wav" }, &asset::ConstructSong }},
		{ "vo", { asset::AssetType::VO, { "wav" }, &asset::ConstructVO }},
		{ "animations", { asset::AssetType::Animation, { "anim" }, &asset::ConstructAnimation }},
		{ "localization", { asset::AssetType::Localization, { "loc" }, &asset::ConstructLocalization }},
		{ "models", { asset::AssetType::Model, { "gltf" }, &asset::ConstructModel }},
	};

	const std::vector< COMDLG_FILTERSPEC> FILTERS =
	{
		{ L"Config File (*.cfg)", L"*.cfg" },
		{ L"Map File (*.map)", L"*.map" },
		{ L"Material (*.mat)", L"*.mat" },
		{ L"Texture (*.png, *.bmp, *.jpg, *.jpeg)", L"*.png;*.bmp;*.jpg;*.jpeg" },
		{ L"Sprite (*.png, *.bmp, *.jpg, *.jpeg)", L"*.png;*.bmp;*.jpg;*.jpeg" },
		{ L"Font (*.png, *.bmp, *.jpg, *.jpeg)", L"*.png;*.bmp;*.jpg;*.jpeg" },
		{ L"Sound (*.wav)", L"*.wav" },
		{ L"Music (*.wav)", L"*.wav" },
		{ L"VO (*.wav)", L"*.wav" },
		{ L"Animation (*.anim)", L"*.anim" },
		{ L"Localization File (*.loc)", L"*.loc" },
		{ L"Model (*.gltf)", L"*.gltf" },
	};
}