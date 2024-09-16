#pragma once

#include <string>

namespace oasis
{
	namespace project
	{
		class ProjectInfo
		{
		private:
			std::string m_ProjectName;
			long long m_Date;

			std::string GetProjectInfoPath(const std::string& a_Path) const;
		public:
			bool LoadProjectInfo(const std::string& a_Path);
			bool Save(const std::string& a_Path);

			std::string ProjectName() const;
			long long Date() const;
		};

		class Project
		{
		private:
			std::string m_Path;
			ProjectInfo m_ProjectInfo;
		public:
			bool Load(const std::string& a_Path);

			std::string Path() const;

			const ProjectInfo& GetProjectInfo() const;
		};
		inline extern Project project = {};

		bool LoadProject(const std::string& a_Path);
	}
}