#ifndef COMMON_H
#define COMMON_H

#include <CtrlLib/CtrlLib.h>
#include <Esc/Esc.h>
#include <Web/Web.h>

using namespace Upp;

#define IMAGECLASS IdeCommonImg
#define IMAGEFILE  <ide/Common/common.iml>
#include <Draw/iml_header.h>

int CharFilterCid(int c);

int    ReadLNG(CParser& p);
String MakeLNG(int lang);

bool   OldLang();

String        PrintTime(int msecs);
inline String GetPrintTime(dword time0) { return PrintTime(msecs(time0)); }

bool   SaveChangedFile(const char *path, String data, bool delete_empty = false);

class Workspace;

struct Ide;

class IdeContext
{
public:
	virtual void      PutConsole(const char *s) = 0;
	virtual void      PutVerbose(const char *s) = 0;

	virtual const Workspace& IdeWorkspace() const = 0;
	virtual bool             IdeIsBuilding() const = 0;
	virtual String           IdeGetOneFile() const = 0;
	virtual int              IdeConsoleExecute(const char *cmdline, Stream *out = NULL, const char *envptr = NULL, bool quiet = false) = 0;
	virtual int              IdeConsoleExecuteWithInput(const char *cmdline, Stream *out, const char *envptr, bool quiet) = 0;
	virtual int              IdeConsoleExecute(One<SlaveProcess> process, const char *cmdline, Stream *out = NULL, bool quiet = false) = 0;
	virtual int              IdeConsoleAllocSlot() = 0;
	virtual bool             IdeConsoleRun(const char *cmdline, Stream *out = NULL, const char *envptr = NULL, bool quiet = false, int slot = 0, String key = Null, int blitz_count = 1) = 0;
	virtual bool             IdeConsoleRun(One<SlaveProcess> process, const char *cmdline, Stream *out = NULL, bool quiet = false, int slot = 0, String key = Null, int blitz_count = 1) = 0;
	virtual void             IdeConsoleFlush() = 0;
	virtual void             IdeConsoleBeginGroup(String group) = 0;
	virtual void             IdeConsoleEndGroup() = 0;
	virtual bool             IdeConsoleWait() = 0;

	virtual bool      IdeIsDebug() const = 0;
	virtual void      IdeEndDebug() = 0;
	virtual void      IdeSetBottom(Ctrl& ctrl) = 0;
	virtual void      IdeActivateBottom() = 0;
	virtual void      IdeRemoveBottom(Ctrl& ctrl) = 0;
	virtual void      IdeSetRight(Ctrl& ctrl) = 0;
	virtual void      IdeRemoveRight(Ctrl& ctrl) = 0;

	virtual String    IdeGetFileName() const = 0;
	virtual int       IdeGetFileLine() = 0;
	virtual String    IdeGetLine(int i) const = 0;

	virtual void      IdeSetDebugPos(const String& fn, int line, const Image& img, int i) = 0;
	virtual void      IdeHidePtr() = 0;
	virtual bool      IdeDebugLock() = 0;
	virtual bool      IdeDebugUnLock() = 0;
	virtual bool      IdeIsDebugLock() const = 0;
	virtual void      IdeSetBar() = 0;

	virtual ~IdeContext() {}
};

IdeContext *TheIde();
void        TheIde(IdeContext *context);

void      PutConsole(const char *s);
void      PutVerbose(const char *s);

const Workspace& GetIdeWorkspace();
bool             IdeIsBuilding();
String           IdeGetOneFile();
int              IdeConsoleExecute(const char *cmdline, Stream *out = NULL, const char *envptr = NULL, bool quiet = false);
int              IdeConsoleExecuteWithInput(const char *cmdline, Stream *out, const char *envptr, bool quiet);
int              IdeConsoleExecute(One<SlaveProcess> process, const char *cmdline, Stream *out = NULL, bool quiet = false);
int              IdeConsoleAllocSlot();
bool             IdeConsoleRun(const char *cmdline, Stream *out = NULL, const char *envptr = NULL, bool quiet = false, int slot = 0, String key = Null, int blitz_count = 1);
bool             IdeConsoleRun(One<SlaveProcess> process, const char *cmdline, Stream *out = NULL, bool quiet = false, int slot = 0, String key = Null, int blitz_count = 1);
void             IdeConsoleFlush();
void             IdeConsoleBeginGroup(String group);
void             IdeConsoleEndGroup();
bool             IdeConsoleWait();

bool      IdeIsDebug();
void      IdeEndDebug();
void      IdeSetBottom(Ctrl& ctrl);
void      IdeActivateBottom();
void      IdeRemoveBottom(Ctrl& ctrl);
void      IdeSetRight(Ctrl& ctrl);
void      IdeRemoveRight(Ctrl& ctrl);

String    IdeGetFileName();
int       IdeGetFileLine();
String    IdeGetLine(int i);

void      IdeSetDebugPos(const String& fn, int line, const Image& img, int i);
void      IdeHidePtr();
bool      IdeDebugLock();
bool      IdeDebugUnLock();
bool      IdeIsDebugLock();

void      IdeSetBar();

struct IdeDesigner  {
	virtual String GetFileName() const = 0;
	virtual void   Save() = 0;
	virtual void   SyncUsc()                                        {}
	virtual void   SaveEditPos()                                    {}
	virtual void   EditMenu(Bar& menu) = 0;
	virtual int    GetCharset() const                               { return -1; }
	virtual Ctrl&  DesignerCtrl() = 0;
	virtual void   SetFocus()                                       { DesignerCtrl().SetFocus(); }

	virtual ~IdeDesigner() {}
};

struct IdeModule {
	virtual void         CleanUsc() {}
	virtual bool         ParseUsc(CParser&)                                       { return false; }
	virtual Image        FileIcon(const char *filename)                           { return Null; }
	virtual IdeDesigner *CreateDesigner(Ide *ide, const char *path, byte charset) { return CreateDesigner(path, charset); }
	virtual IdeDesigner *CreateDesigner(const char *path, byte charset)           { return NULL; }
	virtual void         Serialize(Stream& s) {}

	virtual ~IdeModule() {}
};

void       RegisterIdeModule(IdeModule& module);
int        GetIdeModuleCount();
IdeModule& GetIdeModule(int q);

struct IdeMacro {
	IdeMacro();

	int hotkey;
	String menu;
	String submenu;
	EscValue code;
};

ArrayMap<String, EscValue>& UscGlobal();
Array<IdeMacro>&            UscMacros();

void CleanUsc();
void ParseUscFile(const char *filename) throw(CParser::Error);

Point ReadNums(CParser& p);
Point ReadPoint(CParser& p);

struct SemiTextTest : public TextTest {
	virtual const char *Accept(const char *s) const;
};

Vector<String> SplitDirs(const char *s);

String VarFilePath();
String VarFilePath(String name);

bool   SaveVarFile(const char *filename, const VectorMap<String, String>& var);
bool   LoadVarFile(const char *name, VectorMap<String, String>& var);
bool   SaveVars(const char *name);
bool   LoadVars(const char *name);
String GetVar(const String& var);
String GetVarsName();
String VarFilePath();
Vector<String> GetUppDirs();
String GetUppDir();
void   SetVar(const String& var, const String& value);

String GetAnyFileName(const char *path);
String GetAnyFileTitle(const char *path);
String CatAnyPath(String path, const char *more);
String PackagePath(const String& name);
String SourcePath(const String& package, const String& name);
inline
String PackageDirectory(const String& name) { return GetFileDirectory(PackagePath(name)); }

String GetLocalDir();
String LocalPath(const String& filename);

Vector<String> IgnoreList();

bool   IsFullDirectory(const String& d);

bool   IsCSourceFile(const char *path);
bool   IsCHeaderFile(const char *path);

String FollowCygwinSymlink(const String& filename);

void   SplitPathMap(const char *path_map, Vector<String>& local, Vector<String>& remote);
String JoinPathMap(const Vector<String>& local, const Vector<String>& remote);
void   SplitHostName(const char *hostname, String& host, int& port);

Vector<String> SplitFlags0(const char *flags);
Vector<String> SplitFlags(const char *flags, bool main = false);
Vector<String> SplitFlags(const char *flags, bool main, const Vector<String>& accepts);

bool   MatchWhen(const String& when, const Vector<String>& flag);
String ReadWhen(CParser& p);
String AsStringWhen(const String& when);

struct OptItem {
	String   when;
	String   text;
};

struct CustomStep {
	String when;
	String ext;
	String command;
	String output;

	void   Load(CParser& p) throw(CParser::Error);
	String AsString() const;

	String GetExt() const;
	bool   MatchExt(const char *fn) const;
};

Vector<String> Combine(const Vector<String>& conf, const char *flags);
String Gather(const Array<OptItem>& set, const Vector<String>& conf, bool nospace = false);

bool   HasFlag(const Vector<String>& conf, const char *flag);

enum {
	FLAG_MISMATCH = -2,
	FLAG_UNDEFINED = -1,
};

int    GetType(const Vector<String>& conf, const char *flags);
int    GetType(const Vector<String>& conf, const char *flags, int def);
bool   GetFlag(const Vector<String>& conf, const char *flag);
String RemoveType(Vector<String>& conf, const char *flags);

class Package {
public:
	struct File : public String {
		Array<OptItem> option;
		Array<OptItem> depends;
		bool           readonly;
		bool           separator;
		int            tabsize;
		byte           charset;
		int            font;
		String         highlight;
		int            optimize_speed;

		void operator=(const String& s)   { String::operator=(s); readonly = separator = false; }
		void Init()  { readonly = separator = false; tabsize = Null; charset = 0; font = 0;
		               optimize_speed = false; }

		File()                            { Init(); }
		File(const String& s) : String(s) { Init(); }
	};
	struct Config {
		String name;
		String param;
	};
	byte                     charset;
	bool                     optimize_speed;
	bool                     noblitz;
	String                   description;
	Vector<String>           accepts;
	Array<OptItem>           flag;
	Array<OptItem>           uses;
	Array<OptItem>           target;
	Array<OptItem>           library;
	Array<OptItem>           link;
	Array<OptItem>           option;
	Array<File>              file;
	Array<Config>            config;
	Array<CustomStep>        custom;
	Time                     time;

	int   GetCount() const                { return file.GetCount(); }
	File& operator[](int i)               { return file[i]; }
	const File& operator[](int i) const   { return file[i]; }

	void  Load(const char *file);
	bool  Save(const char *file) const;

	Package();
};

class Workspace {
	void     AddUses(Package& p, bool match, const Vector<String>& flag);
	void     AddLoad(const String& name, bool match, const Vector<String>& flag);

public:
	ArrayMap<String, Package> package;

	void           Clear()                     { package.Clear(); }
	String         operator[](int i) const     { return package.GetKey(i); }
	Package&       GetPackage(int i)           { return package[i]; }
	const Package& GetPackage(int i) const     { return package[i]; }
	int            GetCount() const            { return package.GetCount(); }

	void           Scan(const char *prjname);
	void           Scan(const char *prjname, const Vector<String>& flag);

	Vector<String> GetAllAccepts(int pk) const;

	void     Dump();
};

struct Ide;

bool FinishSave(String tmpfile, String outfile);
bool FinishSave(String outfile);
bool SaveFileFinish(const String& filename, const String& data);
bool SaveChangedFileFinish(const String& filename, const String& data);

enum {
	DEBUG_NONE, DEBUG_MINIMAL, DEBUG_FULL
};

struct Host {
	struct FileInfo : Time, Moveable<FileInfo> {
		int length;
	};
	virtual String             GetEnvironment() = 0;
	virtual String             GetHostPath(const String& path) = 0;
	virtual String             GetLocalPath(const String& path) = 0;
	virtual String             NormalizePath(const String& path) = 0;
	virtual Vector<FileInfo>   GetFileInfo(const Vector<String>& path) = 0;
	virtual void               DeleteFile(const Vector<String>& path) = 0;
	virtual void               DeleteFolderDeep(const String& dir) = 0;
	virtual void               ChDir(const String& path) = 0;
	virtual void               RealizeDir(const String& path) = 0;
	virtual void               SaveFile(const String& path, const String& data) = 0;
	virtual String             LoadFile(const String& path) = 0;
	virtual int                Execute(const char *cmdline) = 0;
	virtual int                ExecuteWithInput(const char *cmdline) = 0;
	virtual int                Execute(const char *cmdline, Stream& out) = 0;
	virtual int                AllocSlot() = 0;
	virtual bool               Run(const char *cmdline, int slot, String key, int blitz_count) = 0;
	virtual bool               Run(const char *cmdline, Stream& out, int slot, String key, int blitz_count) = 0;
	virtual bool               Wait() = 0;
	virtual One<SlaveProcess>  StartProcess(const char *cmdline) = 0;
	virtual void               Launch(const char *cmdline, bool console = false) = 0;
	virtual void               AddFlags(Index<String>& cfg) = 0;

	virtual ~Host() {}
};

String FindInDirs(const Vector<String>& dir, const String& file);
String FindCommand(const Vector<String>& exedir, const String& cmdline);

struct MakeFile {
	String outdir;
	String outfile;
	String output;
	String config;
	String install;
	String rules;
	String linkdep;
	String linkfiles;
	String linkfileend;
};

String GetMakePath(String fn, bool win32);
String AdjustMakePath(const char *fn);

enum {
	R_OPTIMAL,
	R_SPEED,
	R_SIZE
};

struct Builder {
	Host            *host;
	Index<String>    config;
	String           compiler;
	String           method;
	String           outdir;
	Vector<String>   include;
	Vector<String>   libpath;
	String           target;
	String           debug_options;
	String           release_options;
	String           release_size_options;
	String           version;
	String           script;
	bool             doall;

	virtual bool BuildPackage(const String& package, Vector<String>& linkfile, String& linkoptions,
		const Vector<String>& all_uses, const Vector<String>& all_libraries, int optimize)
	{ return false; }
	virtual bool Link(const Vector<String>& linkfile, const String& linkoptions, bool createmap)
	{ return false; }
	virtual bool Preprocess(const String& package, const String& file, const String& result, bool asmout)
	{ return false; }
	virtual void   AddFlags(Index<String>& cfg) {}
	virtual void   AddMakeFile(MakeFile& mfinfo, String package,
		const Vector<String>& all_uses, const Vector<String>& all_libraries,
		const Index<String>& common_config, bool exporting) {}
	virtual String GetTargetExt() const = 0;

	Builder()          { doall = false; }
	virtual ~Builder() {}
};

VectorMap<String, Builder *(*)()>& BuilderMap();
void RegisterBuilder(const char *name, Builder *(*create)());

struct Debugger {
	virtual void DebugBar(Bar& bar) = 0;
	virtual bool SetBreakpoint(const String& filename, int line, const String& bp) = 0;
	virtual bool RunTo() = 0;
	virtual void Run() = 0;
	virtual void Stop() = 0;
	virtual bool IsFinished() = 0;

	virtual ~Debugger() {}
};

void                  HdependSetDirs(pick_ Vector<String>& id);
void                  HdependTimeDirty();
void                  HdependClearDependencies();
void                  HdependAddDependency(const String& file, const String& depends);
Time                  HdependFileTime(const String& path);
Vector<String>        HdependGetDependencies(const String& path);
bool                  HdependBlitzApproved(const String& path);
const Vector<String>& HdependGetDefines(const String& path);
const Vector<String>& HdependGetAllFiles();

void      SourceFs(FileSel& fsel);
FileSel&  AnySourceFs();
FileSel&  AnyPackageFs();
FileSel&  BasedSourceFs();
FileSel&  OutputFs();

Image     IdeFileImage(const String& filename, bool fast = false);

void             RegisterWorkspaceConfig(const char *name);
void             RegisterWorkspaceConfig(const char *name, Callback WhenFlush);
String&          WorkspaceConfigData(const char *name);

template <class T>
bool LoadFromWorkspace(T& x, const char *name)
{
	StringStream ss(WorkspaceConfigData(name));
	return ss.IsEof() || Load(x, ss);
}

template <class T>
void StoreToWorkspace(T& x, const char *name)
{
	StringStream ss;
	Store(x, ss);
	WorkspaceConfigData(name) = ss;
}

void SerializeWorkspaceConfigs(Stream& s);

#endif
