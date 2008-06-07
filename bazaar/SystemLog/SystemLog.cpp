#include "SystemLog.h"

NAMESPACE_UPP

////////////////////////////////////////////////////////////////////////////////////////////////
// Global log object
SystemLog SysLog;

// gets platfor-dependent log level
#ifdef PLATFORM_POSIX
int SystemLog::platform(Levels level)
#elif defined(PLATFORM_WIN32)
dword SystemLog::platform(Levels level)
#endif
{
#ifdef PLATFORM_POSIX
	if(level & LEMERGENCY)	return LOG_EMERG;
	if(level & LALERT)		return LOG_ALERT;
	if(level & LCRITICAL)	return LOG_CRIT;
	if(level & LERROR)		return LOG_ERR;
	if(level & LWARNING)	return LOG_WARNING;
	if(level & LNOTICE)		return LOG_NOTICE;
	if(level & LINFO)		return LOG_INFO;
	if(level & LDEBUG)		return LOG_DEBUG;
#elif defined(PLATFORM_WIN32)
	if(level & LEMERGENCY)	return EVENTLOG_ERROR_TYPE;
	if(level & LALERT)		return EVENTLOG_ERROR_TYPE;
	if(level & LCRITICAL)	return EVENTLOG_ERROR_TYPE;
	if(level & LERROR)		return EVENTLOG_ERROR_TYPE;
	if(level & LWARNING)	return EVENTLOG_WARNING_TYPE;
	if(level & LNOTICE)		return EVENTLOG_INFORMATION_TYPE;
	if(level & LINFO)		return EVENTLOG_INFORMATION_TYPE;
	if(level & LDEBUG)		return EVENTLOG_INFORMATION_TYPE;
#endif

	// should not happen....
	return 0;
	
} // END SystemLog::platform()

////////////////////////////////////////////////////////////////////////////////////////////////
// constructor
SystemLog::SystemLog()
{
	// log not opened upon creation
	FOpened = false;
	
	// disables syslog, disables console, enables CErr() and enables Upp log output
	FCoutEnabled = false;
	FCerrEnabled = true;
	FSysLogEnabled = false;
	FUppLogEnabled = true;
	
	// initializes log buffer
	FBuffer = "";
	
	// initializes starting log level
	FLastLevel = LERROR;
	
} // END Constructor class SystemLog
		
////////////////////////////////////////////////////////////////////////////////////////////////
// destructor
SystemLog::~SystemLog()
{
	// closes the active log
	Close();
	
} // END Destructor class SystemLog

////////////////////////////////////////////////////////////////////////////////////////////////
// closes the active log
void SystemLog::Close()
{
	if (FOpened)
	{
		// flushes log buffer
		Flush();
		
#ifdef PLATFORM_POSIX
		closelog();
#elif defined(PLATFORM_WIN32)
		DeregisterEventSource(log);
#endif
		FOpened = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// opens and initializes log stream
SystemLog &SystemLog::Open(String const &name, String const &dllPath)
{
	// closes, if already opened
	Close();
	
	// stores the user log name
	FLogName = name;
	
#ifdef PLATFORM_POSIX
	openlog(FLogName, LOG_PID, LOG_USER);
#elif defined(PLATFORM_WIN32)
	String path;
	dword eventTypes = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
	dword categoryCount = 0;
	log = NULL;

	path << "SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\" << FLogName;
	if (GetWinRegString("EventMessageFile", path, HKEY_LOCAL_MACHINE) != dllPath)
	{
		if (!SetWinRegExpandString(dllPath, "EventMessageFile", path, HKEY_LOCAL_MACHINE))
			return *this;
		if (!SetWinRegInt(eventTypes, "TypesSupported", path, HKEY_LOCAL_MACHINE))
			return *this;
	}
	log = RegisterEventSource(NULL, FLogName);
	if (log == NULL)
		return *this;
#endif

	// marks log stream as opened
	FOpened = true;

	return *this;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// flushes log buffer
void SystemLog::Flush()
{
	// if no log opened, or buffer empty, does nothing
	if (!FOpened || FBuffer == "" || !(FEnabledLevels & FLastLevel))
	{
		FBuffer = "";
		return;
	}
	
	// Inserts log level name on buffer's head
	String msg = LevelName(FLastLevel) + FBuffer;
	
	// wipes buffer
	FBuffer = "";
	
	// outputs buffer to enabled log streams

	// if upp log enabled, log on upp app log file
	if(FUppLogEnabled)
		LOG(msg);
	
	// if CErr is enabled and message is an error one, logs to CErr()
	if(FCerrEnabled && (FLastLevel & LALLERR))
//		Cerr() << FLogName << " : " << msg << "\n";
		fprintf(stderr, "%s : %s\n", (const char *)FLogName, (const char *)msg);
	else if(FCoutEnabled)
		fprintf(stdout, "%s : %s\n", (const char *)FLogName, (const char *)msg);
//		Cout() << FLogName << " : " << msg << "\n";

	// if syslog enabled, logs on syslog
	if(FSysLogEnabled)
	{

#ifdef PLATFORM_POSIX
		syslog(platform(FLastLevel), msg);
#elif defined(PLATFORM_WIN32)
		const char *array[2];
		array[0] = (const char *)msg;
		if (log == NULL)
			return;
		if (!ReportEvent(log, (WORD)platform(FLastLevel), 0, 0x00000001L, NULL, 1, 0, &array[0], NULL))
			return;
#endif
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// enable specified level(s)
SystemLog &SystemLog::EnableLevels(byte Levels)
{
	// flushes log buffer if needed
	Flush();
	
	FEnabledLevels |= Levels;
	
	return *this;

} // END SystemLog::EnableLevels()

////////////////////////////////////////////////////////////////////////////////////////////////
// disable specified level(s)
SystemLog &SystemLog::DisableLevels(byte Levels)
{
	// flushes log buffer if needed
	Flush();
	
	FEnabledLevels &= ~Levels;
	
	return *this;
	
} // END SystemLog::DisableLevels()

////////////////////////////////////////////////////////////////////////////////////////////////
// sets enabled lebels to 'Levels'
SystemLog &SystemLog::SetLevels(byte Levels)
{
	// flushes log buffer if needed
	Flush();
	
	FEnabledLevels = Levels;
	
	return *this;
	
} // END SystemLog::SetLevels()

////////////////////////////////////////////////////////////////////////////////////////////////
// gets level name from level
String SystemLog::LevelName(Levels level)
{
	switch(level)
	{
		case LEMERGENCY :
			return "EMERGENCY:: ";
		case LALERT :
			return "ALERT    :: ";
		case LCRITICAL :
			return "CRITICAL :: ";
		case LERROR :
			return "ERROR    :: ";
		case LWARNING :
			return "WARNING  :: ";
		case LNOTICE :
			return "NOTICE   :: ";
		case LINFO :
			return "INFO     :: ";
		case LDEBUG :
			return "DEBUG    :: ";
		default:
			return "UNKNOWN  :: ";
	}
	
} // END SystemLog::LevelName()

////////////////////////////////////////////////////////////////////////////////////////////////
// sets current use level
SystemLog &SystemLog::SetLevel(Levels level)
{
	if(FLastLevel != level)
	{
		Flush();
		FLastLevel = level;
	}
	return *this;

} // END SystemLog::SetLevel()

SystemLog &SystemLog::operator()(Levels level)
{
	if(FLastLevel != level)
	{
		Flush();
		FLastLevel = level;
	}
	return *this;

} // END SystemLog::operator()
	
////////////////////////////////////////////////////////////////////////////////////////////////
// writes log message at required level
SystemLog &SystemLog::WriteLog(Levels level, String const &message)
{
	// nothing to do if log is not opened
	if(!FOpened)
		return *this;

	// checks if level is enabled
	if(!(FEnabledLevels & level))
		return *this;
	
	// if changing log level, flushes previous buffer
	if(FLastLevel != level)
		Flush();
	
	// sets last level as current
	FLastLevel = level;
	
	// appends current message to buffer
	int start = 0;
	int nl = 0;
	while( nl < message.GetCount() && (nl = message.Find('\n', start)) >= 0)
	{
		FBuffer += message.Mid(start, nl-start);
		Flush();
		start = nl+1;
	}
	if(start < message.GetCount())
		FBuffer += message.Mid(start);
	
	return *this;
}

END_UPP_NAMESPACE
