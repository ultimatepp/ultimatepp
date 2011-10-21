#ifndef COMMON_H
#define COMMON_H

#include <Esc/Esc.h>
#include <Web/Web.h>

using namespace Upp;

int CharFilterCid(int c);

int    ReadLNG(CParser& p);
String MakeLNG(int lang);

bool   OldLang();

String        PrintTime(int msecs);
inline String GetPrintTime(dword time0) { return PrintTime(msecs(time0)); }

bool   SaveChangedFile(const char *path, String data, bool delete_empty = false);

bool IsDoc(String s);
void CopyFolder(const char *_dst, const char *_src, Index<String>& used, bool all);

class Workspace;

struct Ide;

namespace Upp {
class  Ctrl;
class  Image;
};

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
	virtual void      IdeGotoCodeRef(String link) = 0;
	virtual void      IdeOpenTopicFile(const String& file) = 0;
	virtual void      IdeFlushFile() = 0;
	virtual String    IdeGetFileName() = 0;
	virtual String    IdeGetNestFolder() = 0;

	virtual String GetDefaultMethod();
	virtual VectorMap<String, String> GetMethodVars(const String& method);

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
void             IdeGotoCodeRef(String s);

String GetDefaultMethod();
VectorMap<String, String> GetMethodVars(const String& method);

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

#include "Host.h"

struct IdeMacro {
	IdeMacro();

	int hotkey;
	String menu;
	String submenu;
	EscValue code;
};

ArrayMap<String, EscValue>& UscGlobal();
Array<IdeMacro>&            UscMacros();

void UscSetCleanModules(void (*CleanModules)());
void SetIdeModuleUsc(bool (*IdeModuleUsc)(CParser& p));
void UscSetReadMacro(void (*ReadMacro)(CParser& p));

void CleanUsc();
void ParseUscFile(const char *filename) throw(CParser::Error);

Point ReadNums(CParser& p);
Point ReadPoint(CParser& p);

struct SemiTextTest : public TextTest {
	virtual const char *Accept(const char *s) const;
};

Vector<String> SplitDirs(const char *s);

class Nest {
	VectorMap<String, String> var;
	VectorMap<String, String> package_cache;

	String PackagePath0(const String& name);

public:
	bool   Save(const char *path);
	bool   Load(const char *path);
	String Get(const String& id);
	void   Set(const String& id, const String& val);

	void   InvalidatePackageCache();
	String PackagePath(const String& name);
};

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
void   SetVar(const String& var, const String& val, bool save = true);

String GetAnyFileName(const char *path);
String GetAnyFileTitle(const char *path);
String CatAnyPath(String path, const char *more);

void   InvalidatePackageCache();
String PackagePath(const String& name);
String SourcePath(const String& package, const String& name);
inline
String PackageDirectory(const String& name) { return GetFileDirectory(PackagePath(name)); }
bool   IsNestReadOnly(const String& path);

String GetLocalDir();
String LocalPath(const String& filename);

Vector<String> IgnoreList();

bool   IsFullDirectory(const String& d);
bool   IsFolder(const String& path);

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
	
	String ToString() const { return when + ": " + text ; }
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
	void Reset();

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
	Array<OptItem>           include;
	Array<File>              file;
	Array<Config>            config;
	Array<CustomStep>        custom;
	Time                     time;
	bool                     bold, italic;
	Color                    ink;

	int   GetCount() const                { return file.GetCount(); }
	File& operator[](int i)               { return file[i]; }
	const File& operator[](int i) const   { return file[i]; }

	void  Load(const char *path);
	bool  Save(const char *file) const;

	static void SetPackageResolver(bool (*Resolve)(const String& error, const String& path, int line));

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
	String           debug_link;
	String           release_link;
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

void                  HdependSetDirs(pick_ Vector<String>& id);
void                  HdependTimeDirty();
void                  HdependClearDependencies();
void                  HdependAddDependency(const String& file, const String& depends);
Time                  HdependFileTime(const String& path);
Vector<String>        HdependGetDependencies(const String& path);
String                FindIncludeFile(const char *s, const String& filedir);
bool                  HdependBlitzApproved(const String& path);
const Vector<String>& HdependGetDefines(const String& path);
const Vector<String>& HdependGetAllFiles();

#endif
