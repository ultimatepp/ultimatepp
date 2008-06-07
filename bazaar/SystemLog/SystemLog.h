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

class SystemLog
{
	public:
		typedef SystemLog CLASSNAME;

		// log levels - in a platform-independent way
		enum Levels
		{
			LEMERGENCY	= 0x01,
			LALERT		= 0x02,
			LCRITICAL	= 0x04,
			LERROR		= 0x08,
			LWARNING	= 0x10,
			LNOTICE		= 0x20,
			LINFO		= 0x40,
			LDEBUG		= 0x80,
			LALLERR		= 0x0f,
			LALL		= 0x7f,
			LALLDEBUG	= 0xff
		};

	private:

		// Flags used to enable/disable log levels
		byte FEnabledLevels;
		
		// true when log opened
		bool FOpened;

		// name of log object
		String FLogName;
		
		// enable/disable states for COut(), CErr(), SysLog() and Upp log file
		bool FCoutEnabled;
		bool FCerrEnabled;
		bool FSysLogEnabled;
		bool FUppLogEnabled;
		
		// last log level used -- uset to allow contatenation of operator <<
		Levels FLastLevel;
		
		// gets platform-dependent log level
#ifdef PLATFORM_POSIX
		int platform(Levels level);
#elif defined(PLATFORM_WIN32)
		dword platform(Levels level);
		HANDLE log;
#endif

		// libe buffer, to overcome limitation of system log calls
		// that appends a newline on each operation, making impossible to concatenate
		// output on a single line with operator <<
		String FBuffer;

	public:

		// constructor
		SystemLog();
		
		// destructor
		~SystemLog();

		// opens and initializes log stream
#ifdef PLATFORM_POSIX
		SystemLog &Open(String const &name, String const &dllPath = "");
#elif defined(PLATFORM_WIN32)
		SystemLog &Open(String const &name, String const &dllPath = GetExeFilePath());
#endif

		// enables/disables console output
		SystemLog &EnableCout(bool enable = true) { FCoutEnabled = enable; return *this; }
		SystemLog &EnableCerr(bool enable = true) { FCerrEnabled = enable; return *this; }
		
		// enables/disables sys log ouptut
		SystemLog &EnableSysLog(bool enable = true) { FSysLogEnabled = enable; return *this; }
		
		// enables/disables standard upp log output
		SystemLog &EnableUppLog(bool enable = true) { FUppLogEnabled = enable; return *this; }

		// closes current log stream
		void Close();
		
		// flushes current log buffer
		void Flush();

		// enables/disables/set log levels
		
		// enable specified level(s)
		SystemLog &EnableLevels(byte Levels);
		
		// disable specified level(s)
		SystemLog &DisableLevels(byte Levels);
		
		// sets enabled lebels to 'Levels'
		SystemLog &SetLevels(byte Levels);
		
		// gets level name from level
		String LevelName(Levels level);
		
		// sets current use level
		SystemLog &SetLevel(Levels level);
		SystemLog &operator()(Levels level);
	
		// writes log message at required level
		SystemLog &WriteLog(Levels level, String const &message);
		SystemLog &WriteLog(String const &message) { return WriteLog(FLastLevel, message); }
		
		// shortcuts for various log levels
		inline SystemLog &Emergency(String const &message)	{ return WriteLog(LEMERGENCY	, message); }
		inline SystemLog &Alert(String const &message)		{ return WriteLog(LALERT		, message); }
		inline SystemLog &Critical(String const &message)	{ return WriteLog(LCRITICAL		, message);	}
		inline SystemLog &Error(String const &message)		{ return WriteLog(LERROR		, message);	}
		inline SystemLog &Warning(String const &message)	{ return WriteLog(LWARNING		, message);	}
		inline SystemLog &Notice(String const &message)		{ return WriteLog(LNOTICE		, message);	}
		inline SystemLog &Info(String const &message)		{ return WriteLog(LINFO			, message); }
		inline SystemLog &Debug(String const &message)		{ return WriteLog(LDEBUG		, message);	}
		
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Streaming-Like operators
inline SystemLog& operator<<(SystemLog& s, const char *x)
{
	return s.WriteLog(x);
}

inline SystemLog& operator<<(SystemLog& s, char *x)
{
	return s.WriteLog(x);
}

inline SystemLog& operator<<(SystemLog& s, const String &x)
{
	return s.WriteLog(x);
}

inline SystemLog& operator<<(SystemLog& s, char x)
{
	return s.WriteLog(String(x, 1));
}

inline SystemLog& operator<<(SystemLog& s, const void *x)
{
	return s.WriteLog(FormatPtr(x));
}

inline SystemLog& operator<<(SystemLog& s, void *x)
{
	return s.WriteLog(FormatPtr(x));
}

template <class T>
inline SystemLog& operator<<(SystemLog& s, const T& x) {
	s << AsString(x);
	return s;
}

// Global log object
extern SystemLog SysLog;

END_UPP_NAMESPACE
#endif
