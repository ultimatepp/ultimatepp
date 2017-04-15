#ifndef _Logger_Logger_h
#define _Logger_Logger_h

#include <Core/Core.h>

// Provides common way to print function name between compilers.
// TODO: Check MSVC compatibility
#define UPP_FUNCTION_NAME String(__func__)

namespace Upp {

// TODO: Add support for thread names ids
class Logger {
public:
	enum class LoggingLevel {
		DEBUG,
		INFO,
		WARN,
		ERROR
	};

	Logger(LoggingLevel level, const String& tag = "");
	virtual ~Logger();
	
	template <typename T>
	Logger& operator<<(T const& value) {
		outputStream << value;
		return *this;
	}
	
private:
	void Log();
	
	String GetCurrentTime();
	String AlignTime(int timeValue, int level = 2);
	
private:
	StringStream outputStream;
	
	LoggingLevel level;
	String       tag;
};

Stream& operator<<(Stream& s, Logger::LoggingLevel level);

class LoggerDebug final : public Logger {
public:
	LoggerDebug(const String& tag = "")
		: Logger(Logger::LoggingLevel::DEBUG, tag)
	{}
};

class LoggerInfo final : public Logger {
public:
	LoggerInfo(const String& tag = "")
		: Logger(Logger::LoggingLevel::INFO, tag)
	{}
};

class LoggerWarn final : public Logger {
public:
	LoggerWarn(const String& tag = "")
		: Logger(Logger::LoggingLevel::WARN, tag)
	{}
};

class LoggerError final : public Logger {
public:
	LoggerError(const String& tag = "")
		: Logger(Logger::LoggingLevel::ERROR, tag)
	{}
};

}

#endif
