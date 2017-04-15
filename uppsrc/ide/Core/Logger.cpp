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
	// TODO: Add support for ms somehow...
	
	time_t currentTime = time(nullptr);
	tm* timeInfo = localtime(&currentTime);
	
	StringStream ss;
	ss << AlignTime(timeInfo->tm_hour) << ":"
	   << AlignTime(timeInfo->tm_min)  << ":"
	   << AlignTime(timeInfo->tm_sec);
	
	return ss.GetResult();
}

String Logger::AlignTime(int timeValue, int level)
{
	String alignedTime = IntStr(timeValue);
	for (int i = 0; i < level - alignedTime.GetLength(); i++) {
		alignedTime = "0" + alignedTime;
	}
	return alignedTime;
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
		case (Logger::LoggingLevel::ERROR):
			return s << "ERROR";
	}
}
