#ifndef _SystemLog_SystemLog_h_
#define _SystemLog_SystemLog_h_

#include <Core/Core.h>

#ifdef PLATFORM_POSIX
#include <syslog.h>
#endif 

#ifndef LOG_USER
	#define LOG_USER 0
#endif

NAMESPACE_UPP

enum{
	SYSLOG_EMERGENCY=0,
	SYSLOG_ALERT,
	SYSLOG_CRITICAL,
	SYSLOG_ERROR,
	SYSLOG_WARNING,
	SYSLOG_NOTICE,
	SYSLOG_INFO,
	SYSLOG_DEBUG,
	SYSLOG_COUNT
};

class SystemLog {
	private:
#ifdef PLATFORM_POSIX
		int level[SYSLOG_COUNT];
#endif
#ifdef PLATFORM_WIN32
		dword level[SYSLOG_COUNT];
		HANDLE log;
#endif
		void SetLevel();
	public:
		typedef SystemLog CLASSNAME;
		SystemLog();
		SystemLog(String name);
		~SystemLog();
		
		bool Init(String name, int facility=LOG_USER, String dllPath="%SystemRoot%\\system32\\upplog.dll");
		bool WriteLog(int level, String message);
		
		bool Emergency	(String message)			{ return WriteLog(SYSLOG_EMERGENCY,message);	}
		bool Alert		(String message)			{ return WriteLog(SYSLOG_ALERT,message); 		}
		bool Critical	(String message)			{ return WriteLog(SYSLOG_CRITICAL,message); 	}
		bool Error		(String message)			{ return WriteLog(SYSLOG_ERROR,message); 		}
		bool Warning	(String message)			{ return WriteLog(SYSLOG_WARNING,message); 		}
		bool Notice		(String message)			{ return WriteLog(SYSLOG_NOTICE,message); 		}
		bool Info		(String message)			{ return WriteLog(SYSLOG_INFO,message); 		}
		bool Debug		(String message)			{ return WriteLog(SYSLOG_DEBUG,message); 		}
};
#ifdef PLATFORM_WIN32
bool SetWinRegExpandString(const String& string, const char *value, const char *path, HKEY base_key);
#endif
END_UPP_NAMESPACE
#endif
