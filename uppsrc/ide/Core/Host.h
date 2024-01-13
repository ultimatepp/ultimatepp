enum { REMOTE_TIMEOUT = 2000 };

extern String HostConsole;

void AddHostFlags(Index<String>& cfg);

class Host {
public:
	struct FileInfo : Time, Moveable<FileInfo> {
		int length;
	};
	
public:
	static const String CMDLINE_PREFIX;
	
	Vector<String> exedirs;
	String         environment;

	String        *cmdout;
	
	bool           canlog = true; // it does PutVerbose for commands
	
public:
	void    DoDir(const String& s);

	void                  Log(const String& s) { if(canlog) PutVerbose(s); }
	
	String                GetEnvironment();
	void                  AddEnvironment(const char *id, const char *value);
	Vector<FileInfo>      GetFileInfo(const Vector<String>& path);

	void                  ChDir(const String& path);
	bool                  RealizeDir(const String& path);

	int                   Execute(const char *cmdline);
	int                   ExecuteWithInput(const char *cmdline, bool noconvert);
	int                   Execute(const char *cmdline, Stream& out, bool noconvert = false);
	int                   AllocSlot();
	bool                  Run(const char *cmdline, int slot, String key, int blitz_count);
	bool                  Run(const char *cmdline, Stream& out, int slot, String key, int blitz_count);
	bool                  Wait();
	bool                  Wait(int slot);
	void                  OnFinish(Event<> cb);
	bool                  StartProcess(LocalProcess& p, const char *cmdline);
	void                  Launch(const char *cmdline, bool console = false);
	void                  AddFlags(Index<String>& cfg);
	
	const Vector<String>& GetExecutablesDirs() const;
	void AddExecutable(const String& dir, const String& exe);
	
private:
	bool HasPlatformFlag(const Index<String>& cfg);
};

#ifdef PLATFORM_POSIX
void   RemoveConsoleScripts();
String ResolveHostConsole();
#endif