#pragma once

#include "imgui/tools/BaseWindow.h"

#include <vector>

#include "system/Logger.h"

namespace oasis
{
	namespace imgui
	{
		class ConsoleWindow : public BaseWindow
		{
		public:
			ConsoleWindow();
			void Render() override;

			void Clear();
			void AddMessage(const logger::Message& a_Message);
			static void LoggerCallback(const logger::Message& a_Message);
		private:
			bool m_NeedsRefresh = true;
			std::vector<logger::Message> m_Messages;
			std::vector<logger::Message*> m_FilteredMessages;
		};
		inline extern ConsoleWindow consoleWindow = {};
	}
}