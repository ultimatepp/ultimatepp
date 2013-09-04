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
	HANDLE       hInputWrite;
#endif
#ifdef PLATFORM_POSIX
	Buffer<char> cmd_buf;
	Vector<char *> args;
	pid_t        pid;
	int          rpipe[2], wpipe[2], epipe[2];
	String       exit_string;
#endif
	int          exit_code;

	typedef LocalProcess CLASSNAME;

	bool Start(const char *cmdline, bool spliterr, const char *envptr = NULL);
public:
	bool Start(const char *cmdline, const char *envptr = NULL) { return Start(cmdline, false, envptr); }
	bool Start2(const char *cmdline, const char *envptr = NULL) { return Start(cmdline, true, envptr); }
	
	LocalProcess& ConvertCharset(bool b = true)                       { convertcharset = b; return *this; }
	LocalProcess& NoConvertCharset()                                  { return ConvertCharset(false); }

	LocalProcess()                                                    { Init(); }
	LocalProcess(const char *cmdline, const char *envptr = NULL)      { Init(); Start(cmdline, envptr); }
	virtual ~LocalProcess()                                           { Kill(); }
};

int    Sys(const char *cmd, String& out, bool convertcharset = true);
String Sys(const char *cmd, bool convertcharset = true);
