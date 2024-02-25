#ifndef _umake_umake_h_
#define _umake_umake_h_

#include <ide/Builders/Builders.h>

extern bool SilentMode;

bool UppHubAuto(const String& main);
void UppHubUpdate(const String& main);

class Console {
public:
	virtual void Append(const String& s);

protected:
	struct Slot {
		Slot() : outfile(NULL), quiet(true), exitcode(Null) {}

		One<AProcess>     process;
		String            cmdline;
		String            output;
		String            key;
		String            group;
		Stream            *outfile;
		bool              quiet;
		int               exitcode;
		int               last_msecs;
		int               serial;
	};

	struct Group {
		Group() : count(0), start_time(::msecs()), finished(false), msecs(0), raw_msecs(0) {}

		int               count;
		int               start_time;
		bool              finished;
		int               msecs;
		int               raw_msecs;
	};

	struct Finisher {
		int               serial;
		Event<>           cb;
	};

	Array<Slot> processes;
	Array<Finisher> finisher;
	ArrayMap<String, Group> groups;
	Vector<String> error_keys;
	String current_group;
	String spooled_output;
	int console_lock;
	bool wrap_text;
	int  serial;

	void CheckEndGroup();
	void FlushConsole();

public:
	bool console;
	bool verbosebuild;

	int  Execute(const char *cmdline, Stream *out = NULL, const char *envptr = NULL, bool quiet = false, bool noconvert = false);
	int  Execute(One<AProcess> process, const char *cmdline, Stream *out = NULL, bool quiet = false);
	int  GetSlotCount() const { return processes.GetCount(); }
	int  AllocSlot();
	bool Run(const char *cmdline, Stream *out = NULL, const char *endptr = NULL, bool quiet = false, int slot = 0, String key = Null, int blitz_count = 1);
	bool Run(One<AProcess> process, const char *cmdline, Stream *out = NULL, bool quiet = false, int slot = 0, String key = Null, int blitz_count = 1);
	void BeginGroup(String group);
	void EndGroup();

	Console& operator<<(const String& s)      { Append(s); return *this; }

	bool IsRunning();
	bool IsRunning(int slot);
	int  Flush();
	void Kill(int slot);
	void Kill();
	void ClearError()                         { error_keys.Clear(); }
	Vector<String> PickErrors()               { Vector<String> e = pick(error_keys); error_keys.Clear(); return e; }
	void Wait(int slot);
	bool Wait();

	void OnFinish(Event<> cb);

	void WrapText(bool w)                     { wrap_text = w; }

	void SetSlots(int s);

	Console();
};

struct Ide : public IdeContext, public MakeBuild {
	Workspace wspc;
	Console   console;
	String    onefile;
	bool      verbose;
	String    main;
	int       build_time;

	virtual bool             IsVerbose() const;
	virtual void             PutConsole(const char *s);
	virtual void             PutVerbose(const char *s);
	virtual void             PutLinking() {}
	virtual void             PutLinkingEnd(bool ok) {}

	virtual const Workspace& IdeWorkspace() const ;
	virtual bool             IdeIsBuilding() const ;
	virtual String           IdeGetOneFile() const ;
	virtual int              IdeConsoleExecute(const char *cmdline, Stream *out = NULL, const char *envptr = NULL, bool quiet = false, bool = false);
	virtual int              IdeConsoleExecuteWithInput(const char *cmdline, Stream *out, const char *envptr, bool quiet, bool = false);
	virtual int              IdeConsoleExecute(One<AProcess> process, const char *cmdline, Stream *out = NULL, bool quiet = false);
	virtual int              IdeConsoleAllocSlot();
	virtual bool             IdeConsoleRun(const char *cmdline, Stream *out = NULL, const char *envptr = NULL, bool quiet = false, int slot = 0, String key = Null, int blitz_count = 1);
	virtual bool             IdeConsoleRun(One<AProcess> process, const char *cmdline, Stream *out = NULL, bool quiet = false, int slot = 0, String key = Null, int blitz_count = 1);
	virtual void             IdeConsoleFlush();
	virtual void             IdeConsoleBeginGroup(String group);
	virtual void             IdeConsoleEndGroup();
	virtual bool             IdeConsoleWait();
	virtual bool             IdeConsoleWait(int slot);
	virtual void             IdeConsoleOnFinish(Event<> cb);
	virtual void             IdeProcessEvents() {}

	virtual bool      IdeIsDebug() const ;
	virtual void      IdeEndDebug();
	virtual void      IdeSetBottom(Ctrl& ctrl);
	virtual void      IdeActivateBottom();
	virtual void      IdeRemoveBottom(Ctrl& ctrl);
	virtual void      IdeSetRight(Ctrl& ctrl);
	virtual void      IdeRemoveRight(Ctrl& ctrl);

	virtual String    IdeGetFileName() const ;
	virtual int       IdeGetFileLine();
	virtual String    IdeGetLine(int i) const ;

	virtual void      IdeSetDebugPos(const String& fn, int line, const Image& img, int i);
	virtual void      IdeHidePtr();
	virtual bool      IdeDebugLock();
	virtual bool      IdeDebugUnLock();
	virtual bool      IdeIsDebugLock() const ;
	virtual void      IdeSetBar();
	virtual void      IdeGotoCodeRef(String link);
	virtual void      IdeOpenTopicFile(const String& file);
	virtual void      IdeFlushFile();
	virtual String    IdeGetFileName();
	virtual String    IdeGetNestFolder();

	virtual void           ConsoleShow();
	virtual void           ConsoleSync();
	virtual void           ConsoleClear();
	virtual void           SetupDefaultMethod();
	virtual Vector<String> PickErrors();
	virtual void           BeginBuilding(bool clear_console);
	virtual void           EndBuilding(bool ok);
	virtual void           ClearErrorEditor();
	virtual void           DoProcessEvents();
	virtual void           ReQualifyCodeBase();
	virtual void           SetErrorEditor();
	virtual String         GetMain();

	virtual String                    GetDefaultMethod();
	virtual VectorMap<String, String> GetMethodVars(const String& method);
	virtual String                    GetMethodName(const String& method);

	virtual bool IsPersistentFindReplace() { return false; }

	virtual String    IdeGetIncludePath() { return Null; }

	virtual int       IdeGetHydraThreads() { return CPU_Cores(); }
	virtual String    IdeGetCurrentBuildMethod() { return Null; }
	virtual String    IdeGetCurrentMainPackage() { return Null; }
	virtual void      IdePutErrorLine(const String&) {}
	virtual void      IdeGotoFileAndId(const String& path, const String& id) {}

	void ExportMakefile(const String& ep);
	void ExportProject(const String& ep, bool all, bool deletedir = true);

	Ide();
};

#endif
