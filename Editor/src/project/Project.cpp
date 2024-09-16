#include "project/Project.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <chrono>

#include "DataStream.h"
#include "utils/string_extensions.h"
#include "settings/StaticSettings.h"
#include "utils/RapidJsonUtils.h"
#include "system/FileLoader.h"
#include "system/Logger.h"
#include "imgui/ImGuiWindow.h"

#define FILE_NAME "project.info"

#define JSON_PROJECT_NAME_VAR "projectName"
#define JSON_PROJECT_DATE_VAR "lastModifiedDate"

namespace oasis
{
	namespace project
	{
		std::string ProjectInfo::GetProjectInfoPath(const std::string& a_Path) const
		{
			return a_Path + "/" + FILE_NAME;
		}

		bool ProjectInfo::LoadProjectInfo(const std::string& a_Path)
		{
			m_ProjectName = string_extensions::getFileName(a_Path);

			DataStream data;
			if (!file::FileLoader::LoadFile(GetProjectInfoPath(a_Path).c_str(), data))
			{
				return false;
			}

			rapidjson::Document document;
			document.Parse(reinterpret_cast<char*>(data.data()), data.size());

			if (document.HasParseError())
			{
				Save(a_Path);
				return false;
			}

			// Project Name
			GetString(document, JSON_PROJECT_NAME_VAR, m_ProjectName);

			// Date
			GetLong(document, JSON_PROJECT_DATE_VAR, m_Date);

			Save(a_Path);

			return true;
		}

		bool ProjectInfo::Save(const std::string& a_Path)
		{
			rapidjson::Document document;
			document.SetObject();
			rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

			auto now = std::chrono::system_clock::now();
			auto now_time_t = std::chrono::system_clock::to_time_t(now);
			auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

			document.AddMember(JSON_PROJECT_NAME_VAR, rapidjson::Value().SetString(m_ProjectName.c_str(), m_ProjectName.size()), allocator);
			document.AddMember(JSON_PROJECT_DATE_VAR, now_ms, allocator);

			rapidjson::StringBuffer buffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
			document.Accept(writer);

			return file::FileLoader::SaveFile(GetProjectInfoPath(a_Path).c_str(), Data(buffer.GetString(), buffer.GetSize()));
		}

		std::string ProjectInfo::ProjectName() const
		{
			return m_ProjectName;
		}

		long long ProjectInfo::Date() const
		{
			return m_Date;
		}

		bool Project::Load(const std::string& a_Path)
		{
			m_Path = a_Path;
			bool success = m_ProjectInfo.LoadProjectInfo(a_Path);
			
			return success;
		}

		std::string Project::Path() const
		{
			return m_Path;
		}

		const ProjectInfo& Project::GetProjectInfo() const
		{
			return m_ProjectInfo;
		}

		bool LoadProject(const std::string& a_Path)
		{
			bool b = project.Load(a_Path);
			if (b)
			{
				settings.AddToPreviousProjects(a_Path);

				imgui::window.SetState(EditorState::Editor);
			}
			return b;
		}
	}
}