#include "Logger.h"

#include <chrono>

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
	
	VppLog() << ss.GetResult() << EOL;
}

String Logger::GetCurrentTime()
{
	using namespace std::chrono;
	using days = duration<int, std::ratio_multiply<hours::period, std::ratio<24>>::type>;
	
	auto tp = system_clock::now().time_since_epoch();
	tp += minutes(GetTimeZone());
	
	auto d  = duration_cast<days>(tp);    tp -= d;
	auto h  = duration_cast<hours>(tp);   tp -= h;
	auto m  = duration_cast<minutes>(tp); tp -= m;
	auto s  = duration_cast<seconds>(tp); tp -= s;
	auto ms = duration_cast<milliseconds>(tp);
	
	return Format("%02d:%02d:%02d:%03d", (int)h.count(), (int)m.count(), (int)s.count(), (int)ms.count());
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
