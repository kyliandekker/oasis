#include "settings/StaticSettings.h"

#include <ShlObj_core.h>
#include <filesystem>

// # Rapidjson
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include "DataStream.h"
#include "system/FileLoader.h"

#include "utils/RapidJsonUtils.h"
#include "imgui/ImGuiDefines.h"

namespace fs = std::filesystem;

#define SETTINGS_FOLDER "/oasis/"
#define SETTINGS_PATH "settings.json"

#define JSON_CONSOLE_VAR "console"
#define JSON_CONSOLE_SCROLLTOBOTTOM_VAR "scrollToBottom"
#define JSON_CONSOLE_INFO_VAR "info"
#define JSON_CONSOLE_WARNING_VAR "warning"
#define JSON_CONSOLE_ERROR_VAR "error"
#define JSON_CONSOLE_ASSERT_VAR "assert"
#define JSON_CONSOLE_SUCCESS_VAR "success"
#define JSON_CONSOLE_INFOSUCCESS_VAR "infosuccess"
#define JSON_CONSOLE_AWESOME_VAR "awesome"

#define JSON_WINDOW_SIZE_VAR "windowSize"
#define JSON_WINDOW_SIZE_X_VAR "x"
#define JSON_WINDOW_SIZE_Y_VAR "y"
#define JSON_PREVIOUS_PROJECTS_VAR "previousProjects"

namespace oasis
{
	bool project::StaticSettings::Load()
	{
		std::string path = std::string(GetAppDataPath() + std::string(SETTINGS_PATH));

		DataStream data;
		if (!file::FileLoader::LoadFile(path, data))
		{
			return false;
		}

		m_Size = ImVec2(DEFAULT_SIZE_X, DEFAULT_SIZE_Y);

		rapidjson::Document document;
		document.Parse(reinterpret_cast<char*>(data.data()), data.size());

		if (document.HasParseError())
		{
			Save();
			return false;
		}

		if (document.HasMember(JSON_CONSOLE_VAR) && document[JSON_CONSOLE_VAR].IsObject())
		{
			// Scroll to Bottom
			GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_SCROLLTOBOTTOM_VAR, m_ScrollToBottom);

			GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_INFO_VAR, m_Info);
			GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_WARNING_VAR, m_Warning);
			GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_ERROR_VAR, m_Error);
			GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_ASSERT_VAR, m_Assert);
			GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_SUCCESS_VAR, m_Success);
			GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_INFOSUCCESS_VAR, m_InfoSuccess);
			GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_AWESOME_VAR, m_Awesome);
		}

		// Window size
		{
			ImVec2 temp;
			GetFloat(document[JSON_WINDOW_SIZE_VAR], JSON_WINDOW_SIZE_X_VAR, temp.x);
			GetFloat(document[JSON_WINDOW_SIZE_VAR], JSON_WINDOW_SIZE_Y_VAR, temp.y);

			if (temp.x != 0)
			{
				m_Size.x = temp.x;
			}
			if (temp.y != 0)
			{
				m_Size.y = temp.y;
			}
		}
		// Previous projects array
		if (document.HasMember(JSON_PREVIOUS_PROJECTS_VAR) && document[JSON_PREVIOUS_PROJECTS_VAR].IsArray())
		{
			for (auto& element : document[JSON_PREVIOUS_PROJECTS_VAR].GetArray())
			{
				if (!element.IsString())
				{
					continue;
				}
				m_PreviousProjects.insert(element.GetString());
			}
		}

		return true;
	}

	bool project::StaticSettings::Save() const
	{
		rapidjson::Document document;
		document.SetObject();
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

		document.AddMember(JSON_CONSOLE_VAR, rapidjson::Value().SetObject(), allocator);
		document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_SCROLLTOBOTTOM_VAR, m_ScrollToBottom, allocator);
		document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_INFO_VAR, m_Info, allocator);
		document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_WARNING_VAR, m_Warning, allocator);
		document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_ERROR_VAR, m_Error, allocator);
		document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_ASSERT_VAR, m_Assert, allocator);
		document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_SUCCESS_VAR, m_Success, allocator);
		document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_INFOSUCCESS_VAR, m_InfoSuccess, allocator);
		document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_AWESOME_VAR, m_Awesome, allocator);

		document.AddMember(JSON_WINDOW_SIZE_VAR, rapidjson::Value().SetObject(), allocator);
		document[JSON_WINDOW_SIZE_VAR].AddMember(JSON_WINDOW_SIZE_X_VAR, m_Size.x, allocator);
		document[JSON_WINDOW_SIZE_VAR].AddMember(JSON_WINDOW_SIZE_Y_VAR, m_Size.y, allocator);

		document.AddMember(JSON_PREVIOUS_PROJECTS_VAR, rapidjson::Value().SetArray(), allocator);
		for (auto& previousProject : m_PreviousProjects)
		{
			document[JSON_PREVIOUS_PROJECTS_VAR].PushBack(rapidjson::Value().SetString(previousProject.c_str(), previousProject.size()), allocator);
		}

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);

		std::string path = std::string(GetAppDataPath() + std::string(SETTINGS_PATH));

		return file::FileLoader::SaveFile(path, Data(buffer.GetString(), buffer.GetSize()));

		return true;
	}

	std::string project::StaticSettings::GetAppDataPath() const
	{
		PWSTR path_tmp;
		std::filesystem::path path;
		SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path_tmp);
		path = path_tmp;

		std::string appDataPath = std::string(path.generic_string() + SETTINGS_FOLDER);
		fs::create_directories(appDataPath);

		return appDataPath;
	}

	void project::StaticSettings::SetSize(const ImVec2& a_Size)
	{
		m_Size = a_Size;

		Save();
	}

	ImVec2 project::StaticSettings::Size() const
	{
		return m_Size;
	}

	void project::StaticSettings::SetScrollToBottom(bool a_ScrollToBottom)
	{
		m_ScrollToBottom = a_ScrollToBottom;

		Save();
	}

	bool project::StaticSettings::ScrollToBottom() const
	{
		return m_ScrollToBottom;
	}

	void project::StaticSettings::SetInfo(bool a_Info)
	{
		m_Info = a_Info;

		Save();
	}

	bool project::StaticSettings::Info() const
	{
		return m_Info;
	}

	void project::StaticSettings::SetWarning(bool a_Warning)
	{
		m_Warning = a_Warning;

		Save();
	}

	bool project::StaticSettings::Warning() const
	{
		return m_Warning;
	}

	void project::StaticSettings::SetError(bool a_Error)
	{
		m_Error = a_Error;

		Save();
	}

	bool project::StaticSettings::Error() const
	{
		return m_Error;
	}

	void project::StaticSettings::SetAssert(bool a_Assert)
	{
		m_Assert = a_Assert;

		Save();
	}

	bool project::StaticSettings::Assert() const
	{
		return m_Assert;
	}

	void project::StaticSettings::SetSuccess(bool a_Success)
	{
		m_Success = a_Success;

		Save();
	}

	bool project::StaticSettings::Success() const
	{
		return m_Success;
	}

	void project::StaticSettings::SetInfoSuccess(bool a_InfoSuccess)
	{
		m_InfoSuccess = a_InfoSuccess;

		Save();
	}

	bool project::StaticSettings::InfoSuccess() const
	{
		return m_InfoSuccess;
	}

	void project::StaticSettings::SetAwesome(bool a_Awesome)
	{
		m_Awesome = a_Awesome;

		Save();
	}

	bool project::StaticSettings::Awesome() const
	{
		return m_Awesome;
	}

	void project::StaticSettings::SetPreviousProjects(const std::unordered_set<std::string>& a_PreviousProjects)
	{
		m_PreviousProjects = a_PreviousProjects;

		Save();
	}

	const std::unordered_set<std::string>& project::StaticSettings::GetPreviousProjects() const
	{
		return m_PreviousProjects;
	}

	void project::StaticSettings::AddToPreviousProjects(const std::string& a_PreviousProject)
	{
		m_PreviousProjects.insert(a_PreviousProject);

		Save();
	}

	void project::StaticSettings::ErasePreviousProject(const std::string& a_PreviousProject)
	{
		m_PreviousProjects.erase(a_PreviousProject);

		Save();
	}
}