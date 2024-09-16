#include "Chunks.h"

#include <corecrt_memory.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "ReserveDataStream.h"
#include "utils/string_extensions.h"

namespace oasis
{
	namespace asset
	{
		AssetPair::AssetPair(uint64_t a_Hash, uint64_t a_Position, uint64_t a_Size) : hash(a_Hash), position(a_Position), size(a_Size)
		{ }

		DataStream AssetPair::Serialize() const
		{
			ReserveDataStream serializedData;
			serializedData.Write(&hash, sizeof(hash));
			serializedData.Write(&position, sizeof(position));
			serializedData.Write(&size, sizeof(size));

			return serializedData;
		}

		IndexHeader::IndexHeader(const IndexHeader& rhs) : pairs(rhs.pairs), data(data)
		{
			memcpy(chunk_id, rhs.chunk_id, CHUNK_ID_SIZE);
		}

		IndexHeader::IndexHeader(const char* a_ChunkID)
		{
			memcpy(chunk_id, a_ChunkID, CHUNK_ID_SIZE);
		}

		DataStream IndexHeader::Serialize() const
		{
			ReserveDataStream serializedData;
			serializedData.Write(chunk_id, sizeof(chunk_id));
			uint8_t size = static_cast<uint8_t>(pairs.size()); // TODO: The max amount of assets is 255.
			serializedData.Write(&size, sizeof(size));
			for (auto& pair : pairs)
			{
				Data pairData = pair.Serialize();
				serializedData.Write(pairData.data(), pairData.size());
			}

			return serializedData;
		}

		DataStream IndexHeader::SerializeData() const
		{
			ReserveDataStream serializedData;
			for (auto& pair : data)
			{
				serializedData.Write(pair.data(), pair.size());
			}

			return serializedData;
		}

		AssetData::AssetData(const std::string a_Path) : path(a_Path), hash(std::hash<std::string>{}(path))
		{ }

		DataStream AssetData::Serialize() const
		{
			ReserveDataStream serializedData(sizeof(path.size() + 1 + sizeof(hash)));
			serializedData.Write(path.c_str(), path.size());
			serializedData.Write("\0", 1);
			serializedData.Write(&hash, sizeof(hash));

			return serializedData;
		}
	
		TextureAssetData::TextureAssetData(const std::string a_Path) : AssetData(string_extensions::getFileWithoutExtension(string_extensions::getFileName(a_Path)))
		{
			int width = 0, height = 0, channels = 0;
			unsigned char* data = stbi_load(a_Path.c_str(), &width, &height, &channels, 0);

			this->width = width;
			this->height = height;
			this->channels = channels;

			this->data = Data(data, width * height);
		}

		DataStream TextureAssetData::Serialize() const
		{
			ReserveDataStream serializedData(AssetData::Serialize());
			serializedData.Write(&width, sizeof(width));
			serializedData.Write(&height, sizeof(height));
			serializedData.Write(&channels, sizeof(channels));
			size_t size = data.size();
			serializedData.Write(&size, sizeof(size));
			serializedData.Write(data.data(), data.size());

			return serializedData;
		}
	}
}