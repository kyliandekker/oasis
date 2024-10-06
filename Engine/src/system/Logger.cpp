#include "system/Logger.h"

#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <format>
#include <windows.h>

oasis::logger::Logger oasis::logger::logger;

namespace oasis
{
	namespace logger
	{
		Logger::Logger()
		{
			m_Thread = std::thread(&Logger::MessageQueue, this);
		}

		Logger::~Logger()
		{
			m_MessagesMutex.lock();
			while (!m_Messages.empty())
			{
				m_Messages.pop();
			}
			m_MessagesMutex.unlock();
			m_Running = false;
			m_Thread.join();
		}

		void Logger::Log(LogSeverity a_Severity, const char* a_Message, const char* a_File, int a_Line)
		{
			PrintMessage(a_Severity, a_Message, a_File, a_Line);
		}

		void Logger::LogF(LogSeverity a_Severity, const char* a_Message, const char* a_File, int a_Line, ...)
		{
			va_list va_format_list;
			va_start(va_format_list, a_Line);

			const size_t buffersize = vsnprintf(NULL, 0, a_Message, va_format_list) + 1;
			char* formatted_message = (char*)malloc(buffersize);
			vsnprintf(formatted_message, buffersize, a_Message, va_format_list);

			PrintMessage(a_Severity, formatted_message, a_File, a_Line);

			free(formatted_message);
		}

		void Logger::PrintMessage(LogSeverity a_Severity, const char* a_Message, const char* a_File, int a_Line)
		{
			struct tm lt;
			time_t t;
			t = time(NULL);
			localtime_s(&lt, &t);

			std::string message = std::format("\"{0}\" on line {1}",
				a_File,
				a_Line);

			m_MessagesMutex.lock();
			m_Messages.push({ a_Message, message, a_Severity, std::chrono::system_clock::now() });
			m_MessagesMutex.unlock();
		}

		char arr[7][15] =
		{
			"INFO",
			"WARNING",
			"ERROR",
			"ASSERT",
			"SUCCESS",
			"INFO SUCCESS",
			"AWESOME",
		};

		void Logger::MessageQueue()
		{
			m_Running = true;
			while (m_Running)
			{
				m_MessagesMutex.lock();
				if (m_Messages.size() > 0)
				{
					const Message lm = m_Messages.front();
					m_Messages.pop();

					DWORD written = 0;

					std::string formatted_message = std::format("[{0}] \"{1}\" in file {2}.\n",
						arr[lm.a_Severity],
						lm.m_RawMessage,
						lm.m_Location);

					WriteConsoleA(
						GetStdHandle(STD_OUTPUT_HANDLE),
						formatted_message.c_str(),
						static_cast<DWORD>(formatted_message.size()),
						&written,
						nullptr
					);

					OnMessageLogged(lm);
				}
				m_MessagesMutex.unlock();
			}
		}

		bool Logger::Running() const
		{
			return m_Running;
		}
	}
}