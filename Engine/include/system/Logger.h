#pragma once

#include <stdint.h>
#include <assert.h>
#include <queue>
#include <thread>
#include <string>
#include <functional>
#include <mutex>

namespace oasis
{
	namespace logger
	{
#define ASSERT_ON_ASSERT false

		typedef enum LogSeverity {
			LOGSEVERITY_INFO,
			LOGSEVERITY_WARNING,
			LOGSEVERITY_ERROR,
			LOGSEVERITY_ASSERT,
			LOGSEVERITY_SUCCESS,
			LOGSEVERITY_INFO_SUCCESS,
			LOGSEVERITY_AWESOME,
		} LogSeverity;

		struct Message
		{
			std::string m_RawMessage;
			std::string m_Location;
			LogSeverity a_Severity;
			std::chrono::system_clock::time_point m_Time;
		};

		class Logger
		{
		public:
			Logger();
			~Logger();
			void Log(LogSeverity a_Severity, const char* a_Message, const char* a_File, int a_Line);
			void LogF(LogSeverity a_Severity, const char* a_Message, const char* a_File, int a_Line, ...);
			void PrintMessage(LogSeverity a_Severity, const char* a_Message, const char* a_File, int a_Line);
			void MessageQueue();

			bool Running() const;

			std::function<void(const Message&)> m_LoggerCallback;
		private:
			bool m_Running = false;
			std::queue<Message> m_Messages;
			std::thread m_Thread;
			std::mutex m_MessagesMutex;
		};
		extern Logger logger;
	}
}

#define LOGF(a_Severity, a_Message, ...)\
do{\
	if (ASSERT_ON_ASSERT && a_Severity == logger::LogSeverity::LOGSEVERITY_ASSERT)\
		assert(false);\
	logger::logger.LogF(a_Severity, a_Message, __FILE__, __LINE__, __VA_ARGS__);\
} while (0)

#define LOG(a_Message, ...)\
do{\
	logger::logger.LogF(logger::LOGSEVERITY_INFO, a_Message, __FILE__, __LINE__, __VA_ARGS__);\
} while (0)
