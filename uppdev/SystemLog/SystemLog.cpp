#include "SystemLog.h"

NAMESPACE_UPP

SystemLog::SystemLog() {}

SystemLog::SystemLog(String name) {
	String dllPath;
#ifdef PLATFORM_WIN32
	dllPath=GetExeDirFile("upplog.dll");
	if(FileExists(dllPath))
		Init(name,0,dllPath);
	else
#endif
	Init(name);
}

bool SystemLog::Init(String name, int facility, String dllPath) {
	SetLevel();
#ifdef PLATFORM_POSIX
	openlog(name,LOG_CONS | LOG_PERROR,LOG_USER);
#elif defined(PLATFORM_WIN32)
	String path;
	dword eventTypes = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE; 
	dword categoryCount=0;
    log=NULL;
       
	path << "SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\"<<name;
	if(!SetWinRegExpandString(dllPath,"EventMessageFile",path,HKEY_LOCAL_MACHINE))
		return false;
	if(!SetWinRegInt(eventTypes,"TypesSupported",path,HKEY_LOCAL_MACHINE))
		return false;
	/*
	if(!SetWinRegString(dllPath,"CategoryMessageFile",path,HKEY_LOCAL_MACHINE))
		return false;
	if(!SetWinRegInt(categoryCount,"CategoryCount",path,HKEY_LOCAL_MACHINE))
		return false;
	*/
	
	log=RegisterEventSource(NULL,name);
	if(log==NULL)
		return false;
#endif
	return true;
}

SystemLog::~SystemLog() {
#ifdef PLATFORM_POSIX
	closelog();
#elif defined(PLATFORM_WIN32)
	if(log!=NULL)
		DeregisterEventSource(log);
#endif
}

bool SystemLog::WriteLog(int _level, String message) {
#ifdef PLATFORM_POSIX
	syslog(level[_level],message);
#elif defined(PLATFORM_WIN32)	
	const char *array[2];
	array[0] = (const char *)message;
	if(log==NULL)
		return false;
	if(!ReportEvent(log,level[_level],0,0x00000001L,NULL,1,0,&array[0],NULL))
		return false;
#else
	LOG(message);
#endif
	return true;
}


void SystemLog::SetLevel() {
#ifdef PLATFORM_POSIX
	level[SYSLOG_EMERGENCY]=LOG_EMERG;
	level[SYSLOG_ALERT]=LOG_ALERT;
	level[SYSLOG_CRITICAL]=LOG_CRIT;
	level[SYSLOG_ERROR]=LOG_ERR;
	level[SYSLOG_WARNING]=LOG_WARNING;
	level[SYSLOG_NOTICE]=LOG_NOTICE;
	level[SYSLOG_INFO]=LOG_INFO;
	level[SYSLOG_DEBUG]=LOG_DEBUG;
#elif defined(PLATFORM_WIN32)
	level[SYSLOG_EMERGENCY]=EVENTLOG_ERROR_TYPE;
	level[SYSLOG_ALERT]=EVENTLOG_ERROR_TYPE;
	level[SYSLOG_CRITICAL]=EVENTLOG_ERROR_TYPE;
	level[SYSLOG_ERROR]=EVENTLOG_ERROR_TYPE;
	level[SYSLOG_WARNING]=EVENTLOG_WARNING_TYPE;
	level[SYSLOG_NOTICE]=EVENTLOG_INFORMATION_TYPE;
	level[SYSLOG_INFO]=EVENTLOG_INFORMATION_TYPE;
	level[SYSLOG_DEBUG]=EVENTLOG_INFORMATION_TYPE;
#endif
}
#ifdef PLATFORM_WIN32
bool SetWinRegExpandString(const String& string, const char *value, const char *path, HKEY base_key) {
	HKEY key = 0;
	if(RegCreateKeyEx(base_key, path, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &key, NULL) != ERROR_SUCCESS)
		return false;
	bool ok = (RegSetValueEx(key, value, 0,	REG_EXPAND_SZ, string, string.GetLength() + 1) == ERROR_SUCCESS);
	RegCloseKey(key);
	return ok;
}
#endif
END_UPP_NAMESPACE
