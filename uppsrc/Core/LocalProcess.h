struct AProcess : NoCopy {
public:
	virtual void Kill() = 0;
	virtual bool IsRunning() = 0;
	virtual void Write(String s) = 0;
	virtual bool Read(String& s) = 0;
	virtual bool Read2(String& os, String& es) { NEVER(); return false; }
	virtual int  GetExitCode() = 0;
	virtual String GetExitMessage() { return String(); };
	virtual void CloseRead()        {}
	virtual void CloseWrite()       {}
	virtual void Detach()           {};
	
	String  Get()                  { String x; if(Read(x)) return x; return String::GetVoid(); }

	AProcess() {}
	virtual ~AProcess() {}
};

class LocalProcess : public AProcess {
public:
	virtual void Kill();
	virtual bool IsRunning();
	virtual void Write(String s);
	virtual bool Read(String& s);
	virtual bool Read2(String& os, String &es);
	virtual String GetExitMessage();
	virtual int  GetExitCode();
	virtual void CloseRead();
	virtual void CloseWrite();
	virtual void Detach();

private:
	void         Init();
	void         Free();
#ifdef PLATFORM_POSIX
	bool         DecodeExitCode(int code);
#endif

private:
	bool         convertcharset;

#ifdef PLATFORM_WIN32
	HANDLE       hProcess;
	HANDLE       hOutputRead;
	HANDLE       hErrorRead;
	HANDLE       hInputWrite;
	DWORD        dwProcessId;
#endif
#ifdef PLATFORM_POSIX
	pid_t        pid;
	int          rpipe[2], wpipe[2], epipe[2];
	String       exit_string;
	bool         doublefork;
#endif
	int          exit_code;
	String       wreso, wrese; // Output fetched during Write

	typedef LocalProcess CLASSNAME;

	bool DoStart(const char *cmdline, const Vector<String> *arg, bool spliterr, const char *envptr, const char *cd);

public:
	bool Start(const char *cmdline, const char *envptr = NULL, const char *cd = NULL)                         { return DoStart(cmdline, NULL, false, envptr, cd); }
	bool Start2(const char *cmdline, const char *envptr = NULL, const char *cd = NULL)                        { return DoStart(cmdline, NULL, true, envptr, cd); }

	bool Start(const char *cmd, const Vector<String>& arg, const char *envptr = NULL, const char *cd = NULL)  { return DoStart(cmd, &arg, false, envptr, cd); }
	bool Start2(const char *cmd, const Vector<String>& arg, const char *envptr = NULL, const char *cd = NULL) { return DoStart(cmd, &arg, true, envptr, cd); }
	
#ifdef PLATFORM_POSIX
	LocalProcess& DoubleFork(bool b = true)                           { doublefork = b; return *this; }

	int  GetPid()  const                                              { return pid; }
#endif

#ifdef PLATFORM_WIN32
	HANDLE  GetProcessHandle()  const                                 { return hProcess; }
#endif

	int  Finish(String& out);
		
	LocalProcess& ConvertCharset(bool b = true)                       { convertcharset = b; return *this; }
	LocalProcess& NoConvertCharset()                                  { return ConvertCharset(false); }

	LocalProcess()                                                                          { Init(); }
	LocalProcess(const char *cmdline, const char *envptr = NULL)                            { Init(); Start(cmdline, envptr); }
	LocalProcess(const char *cmdline, const Vector<String>& arg, const char *envptr = NULL) { Init(); Start(cmdline, arg, envptr); }
	virtual ~LocalProcess()                                                                 { Kill(); }
};

int    Sys(const char *cmdline, String& out, bool convertcharset = true);
String Sys(const char *cmdline, bool convertcharset = true);

int    Sys(const char *cmd, const Vector<String>& arg, String& out, bool convertcharset = true);
String Sys(const char *cmd, const Vector<String>& arg, bool convertcharset = true);
