#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "DataStream.h"

namespace oasis
{
	namespace asset
	{
#pragma pack(push, 1)
		constexpr uint32_t CHUNK_ID_SIZE = 4;

		// TODO: This is called an AssetPair but it is not a pair...
		class AssetPair
		{
		public:
			uint64_t hash;
			uint64_t position;
			uint64_t size;

			AssetPair() = default;
			AssetPair(uint64_t a_Hash, uint64_t a_Position, uint64_t a_Size);

			DataStream Serialize() const;
		};

		class IndexHeader
		{
		public:
			IndexHeader(const IndexHeader& rhs);
			IndexHeader(const char* a_ChunkID);

			unsigned char chunk_id[CHUNK_ID_SIZE] = {};
			std::vector<AssetPair> pairs;
			std::vector<Data> data;

			bool operator<(const IndexHeader& other) const
			{
				int cmp = std::memcmp(this->chunk_id, other.chunk_id, CHUNK_ID_SIZE);
				return cmp < 0;
			}

			DataStream Serialize() const;
			DataStream SerializeData() const;
		};

		constexpr auto CFG_HEADER_ID = "CFG ";
		constexpr auto MAP_HEADER_ID = "MAP ";
		constexpr auto MAT_HEADER_ID = "MAT ";
		constexpr auto TEX_HEADER_ID = "TEX ";
		constexpr auto SPR_HEADER_ID = "SPRT";
		constexpr auto FONT_HEADER_ID = "FONT";
		constexpr auto SND_HEADER_ID = "SND ";
		constexpr auto SONG_HEADER_ID = "SONG";
		constexpr auto VO_HEADER_ID = "VO ";
		constexpr auto ANIM_HEADER_ID = "ANIM";
		constexpr auto LOC_HEADER_ID = "LOC ";
		constexpr auto MDL_HEADER_ID = "MDL ";

#pragma pack(pop)
		class AssetData
		{
		public:
			AssetData(const std::string a_Path);

			std::string path = "";
			uint64_t hash = 0;

			virtual DataStream Serialize() const;
		};

		class TextureAssetData : public AssetData
		{
		public:
			TextureAssetData(const std::string a_Path);
			uint32_t 
				width = 0, 
				height = 0;
			uint16_t channels = 0;
			Data data;

			DataStream Serialize() const override;
		};

		class ModelAssetData : public AssetData
		{
		public:
			ModelAssetData(const std::string a_Path);
			Data data;

			DataStream Serialize() const override;
		};
	}
}