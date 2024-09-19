#pragma once

#ifdef __EDITOR__

#include "editor/imgui/tools/BaseWindow.h"

#include <vector>
#include <mutex>

#include "system/Logger.h"

namespace oasis
{
	namespace imgui
	{
		inline std::mutex MESSAGE_MUTEX;

		class ImGuiWindow;

		class ConsoleWindow : public BaseWindow
		{
		public:
			ConsoleWindow(ImGuiWindow& a_Window);
			~ConsoleWindow();
			void Render() override;

			void Clear();
			void AddMessage(const logger::Message& a_Message);

			void LoggerCallback(const logger::Message& a_Message);
		private:
			bool m_NeedsRefresh = true;
			std::vector<logger::Message> m_Messages;
			std::vector<int> m_FilteredMessages;
		};
	}
}

#endif // __EDITOR__