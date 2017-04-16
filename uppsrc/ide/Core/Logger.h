#ifndef _Logger_Logger_h
#define _Logger_Logger_h

#include <Core/Core.h>

#define UPP_FUNCTION_NAME String(__func__)

namespace Upp {

// TODO: Add support for thread names ids.
// TODO: Add logger config to select global logging level.
class Logger {
public:
	enum class LoggingLevel {
		DEBUG,
		INFO,
		WARN,
		ERROR,
		NONE
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

#define LOGGER(CLASS_NAME, LEVEL) \
class CLASS_NAME : public Logger { \
public: \
	CLASS_NAME(const String& tag = "") \
		: Logger(LEVEL, tag) \
	{} \
};

LOGGER(Logd, Logger::LoggingLevel::DEBUG)
LOGGER(Logi, Logger::LoggingLevel::INFO)
LOGGER(Logw, Logger::LoggingLevel::WARN)
LOGGER(Loge, Logger::LoggingLevel::ERROR)

#undef LOGGER

}

#endif
