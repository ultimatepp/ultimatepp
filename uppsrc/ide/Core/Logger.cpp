#include "Logger.h"

using namespace Upp;

Logger::Logger(LoggingLevel level, const String& tag)
	: level(level)
	, tag(tag)
{
}

Logger::~Logger()
{
	Log();
}

void Logger::Log()
{
	StringStream ss;
	
	String output = outputStream.GetResult();
	if (output.IsEmpty())
		return;
	
	ss << GetCurrentTime();
	ss << " " << level;
	if (!tag.IsEmpty())
	{
		ss << "/" << tag;
	}
	ss << " " << output;
	
	LOG(ss.GetResult());
}

String Logger::GetCurrentTime()
{
	Time tm = GetSysTime();
	return Format("%02d:%02d:%02d", (int)tm.hour, (int)tm.minute, (int)tm.second);
}

Stream& Upp::operator<<(Stream& s, Logger::LoggingLevel level)
{
	switch(level)
	{
		case (Logger::LoggingLevel::DEBUG):
			return s << "DEBUG";
		case (Logger::LoggingLevel::INFO):
			return s << "INFO ";
		case (Logger::LoggingLevel::WARN):
			return s << "WARN ";
		case (Logger::LoggingLevel::ERROR_LEVEL):
			return s << "ERROR";
		case (Logger::LoggingLevel::NONE):
			return s << "NONE ";
	}
	return s;
}
