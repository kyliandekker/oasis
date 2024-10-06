#include "assets/MetaData.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include "system/FileLoader.h"
#include "system/Logger.h"
#include "utils/RapidJsonUtils.h"
#include "DataStream.h"

#define JSON_ASSETTYPE_VAR "assetType"

namespace oasis
{
	namespace assets
	{
        bool MetaData::Save(const std::string& a_Path) const
        {
			rapidjson::Document document;
			document.SetObject();
			rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

			int assetType = (int)m_AssetType;
			document.AddMember(JSON_ASSETTYPE_VAR, assetType, allocator);

			rapidjson::StringBuffer buffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
			document.Accept(writer);

			return file::FileLoader::SaveFile(a_Path, Data(buffer.GetString(), buffer.GetSize()));
        }

		bool MetaData::Load(const std::string& a_Path)
		{
			DataStream data;
			if (!file::FileLoader::LoadFile(a_Path, data))
			{
				Save(a_Path);
				return false;
			}

			rapidjson::Document document;
			document.Parse(reinterpret_cast<char*>(data.data()), data.size());

			if (document.HasParseError())
			{
				LOGF(logger::LOGSEVERITY_ERROR, "Something went wrong when trying to load meta file '%s'.", a_Path.c_str());
				Save(a_Path);
				return false;
			}
			int assetType = 0;
			rapidjson::GetInt(document, JSON_ASSETTYPE_VAR, assetType);
			m_AssetType = (AssetType)assetType;

			return false;
		}
	}
}