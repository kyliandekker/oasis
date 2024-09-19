#pragma once

#include <string>
#include <rapidjson/document.h>

namespace rapidjson
{
	bool GetString(rapidjson::Value& a_Document, const std::string& a_Key, std::string& a_Value);

	bool GetFloat(rapidjson::Value& a_Document, const std::string& a_Key, float& a_Value);

	bool GetBool(rapidjson::Value& a_Document, const std::string& a_Key, bool& a_Value);

	bool GetLong(rapidjson::Value& a_Document, const std::string& a_Key, long long& a_Value);
}