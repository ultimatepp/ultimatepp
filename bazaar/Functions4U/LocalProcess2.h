#ifndef _Functions4U_LocalProcess2_h
#define _Functions4U_LocalProcess2_h

namespace Upp {

class LocalProcess2 : public AProcess {
public:
	virtual void Kill();
	virtual bool IsRunning();
	virtual void Write(String s);
	virtual bool Read(String& s);
	virtual bool Read2(String& os, String &es);
	virtual String GetExitMessage();
	virtual int  GetExitCode();
#ifdef PLATFORM_WIN32	
	void Pause();
	bool IsPaused()		{return paused;}
#endif
	
private:
	virtual void CloseRead();
	virtual void CloseWrite();
	virtual void Detach();

	void         Init();
	void         Free();
#ifdef PLATFORM_POSIX
	bool         DecodeExitCode(int code);
#endif
	bool         convertcharset;
	
#ifdef PLATFORM_WIN32
	HANDLE       hProcess;
	DWORD		 dwProcessId;
	HANDLE       hOutputRead;
	HANDLE       hErrorRead;
	HANDLE       hInputWrite;
	bool 		 paused;
#endif
#ifdef PLATFORM_POSIX
	Buffer<char> cmd_buf;
	Vector<char *> args;
	pid_t        pid;
	int          rpipe[2], wpipe[2], epipe[2];
	String       exit_string;
	bool         doublefork;
#endif
	int          exit_code;
	String       wreso, wrese; // Output fetched during Write

	typedef LocalProcess2 CLASSNAME;

	bool DoStart(const char *cmdline, const Vector<String> *arg, bool spliterr, const char *envptr = NULL, const char *dir = NULL);

public:
	bool Start(const char *cmdline, const char *envptr = NULL, const char *dir = NULL)  { return DoStart(cmdline, NULL, false, envptr, dir); }
	bool Start2(const char *cmdline, const char *envptr = NULL, const char *dir = NULL) { return DoStart(cmdline, NULL, true, envptr, dir); }

	bool Start(const char *cmd, const Vector<String>& arg, const char *envptr = NULL, const char *dir = NULL)  { return DoStart(cmd, &arg, false, envptr, dir); }
	bool Start2(const char *cmd, const Vector<String>& arg, const char *envptr = NULL, const char *dir = NULL) { return DoStart(cmd, &arg, true, envptr, dir); }
	
#ifdef PLATFORM_POSIX
	LocalProcess2& DoubleFork(bool b = true)                   	{ doublefork = b; return *this; }

	int  GetPid()  const                                        { return pid; }
#endif

#ifdef PLATFORM_WIN32
	HANDLE  GetProcessHandle()  const                           { return hProcess; }
	DWORD  GetPid()  const                                    	{ return dwProcessId; }
#endif

	int  Finish(String& out);
		
	LocalProcess2& ConvertCharset(bool b = true)                { convertcharset = b; return *this; }
	LocalProcess2& NoConvertCharset()                           { return ConvertCharset(false); }

	LocalProcess2()                                                                          { Init(); }
	LocalProcess2(const char *cmdline, const char *envptr = NULL, const char *dir = NULL)    { Init(); Start(cmdline, envptr, dir); }
	LocalProcess2(const char *cmdline, const Vector<String>& arg, const char *envptr = NULL, const char *dir = NULL) { Init(); Start(cmdline, arg, envptr, dir); }
	virtual ~LocalProcess2()                                                                 { Kill(); }
};

}

#endif