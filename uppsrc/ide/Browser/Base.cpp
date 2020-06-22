#include "Browser.h"

#include <plugin/lz4/lz4.h>

#define LTIMING(x)    // DTIMING(x)
#define LLOG(x)       // DLOG(x)
#define LTIMESTOP(x)  // RTIMESTOP(x)

#define LDUMP(x)      // DDUMP(x)

// #define HAS_CLOG

#ifdef HAS_CLOG
#define CLOG(x)       // RLOG(x)
#else
#define CLOG(x)
#endif

#define MLOG(x)

#define CPP_CODEBASE_VERSION 3141592

INITIALIZER(CodeBase)
{
	void InitializeTopicModule();
	InitializeTopicModule();
}

CppBase& CodeBase()
{
	static CppBase b;
	return b;
}

ArrayMap<String, SourceFileInfo> source_file;

void SourceFileInfo::Serialize(Stream& s)
{
	s % time % dependencies_md5sum % md5sum;
	if(s.IsLoading()) {
		depends.Clear();
		depends_time = Null;
	}
}

String CodeBaseCacheDir()
{
#ifdef _DEBUG
	return ConfigFile("cfg/debug_codebase");
#else
	return ConfigFile("cfg/codebase");
#endif
}

struct RCB_FileInfo {
	String path;
	Time   time;
	int64  length;
	
	bool operator<(const RCB_FileInfo& a) const { return time > a.time; }
};

void ReduceCacheFolder(const char *path, int max_total)
{
	Array<RCB_FileInfo> file;
	FindFile ff(AppendFileName(path, "*.*"));
	int64 total = 0;
	while(ff) {
		if(ff.IsFile()) {
			RCB_FileInfo& m = file.Add();
			m.path = ff.GetPath();
			m.time = ff.GetLastAccessTime();
			m.length = ff.GetLength();
			total += m.length;
		}
		ff.Next();
	}
	Sort(file);
	while(total > max_total && file.GetCount()) {
		DeleteFile(file.Top().path);
		total -= file.Top().length;
		file.Drop();
	}
}

void ReduceCodeBaseCache()
{
	ReduceCacheFolder(CodeBaseCacheDir(), 256000000);
}

String CodeBaseCacheFile()
{
	return AppendFileName(CodeBaseCacheDir(), GetVarsName() + '.' + IdeGetCurrentMainPackage() + '.' + IdeGetCurrentBuildMethod() + ".codebase");
}

static bool   s_console;

void BrowserScanError(int line, const String& text, int file)
{
	if(s_console)
		IdePutErrorLine(String().Cat() << source_file.GetKey(file) << " (" << line << "): " << text);
}

void SerializeCodeBase(Stream& s)
{
	CppBaseLock __;
	MLOG(s.IsLoading());
	source_file.Serialize(s);
	MLOG("source_file " << MemoryUsedKb());
	SerializePPFiles(s);
	MLOG("PP files " << MemoryUsedKb());
	CodeBase().Serialize(s);
	MLOG("codebase " << MemoryUsedKb());
}

void SaveCodeBase()
{
	CppBaseLock __;
	LTIMING("SaveCodeBase");
	LLOG("Save code base " << CodeBase().GetCount());
	RealizeDirectory(CodeBaseCacheDir());
	StringStream ss;
	Store(callback(SerializeCodeBase), ss, CPP_CODEBASE_VERSION);
	String data = ss.GetResult();
	String path = CodeBaseCacheFile();
	SaveFile(path, LZ4Compress(data));
}

bool TryLoadCodeBase(const char *pattern)
{
	CppBaseLock __;
	LLOG("+++ Trying to load " << pattern);
	FindFile ff(pattern);
	String path;
	int64  len = -1;
	while(ff) { // Load biggest file, as it has the most chances to have the data we need
		if(ff.IsFile() && ff.GetLength() > len) {
			path = ff.GetPath();
			len = ff.GetLength();
		}
		ff.Next();
	}
	if(path.GetCount()) {
		LTIMING("Load code base");
		StringStream ss(LZ4Decompress(LoadFile(path)));
		MLOG("Decompressed " << MemoryUsedKb());
		if(Load(callback(SerializeCodeBase), ss, CPP_CODEBASE_VERSION)) {
			CLOG("*** Loaded " << ff.GetPath() << ' ' << GetSysTime() << ", file count: " << source_file.GetCount() << ", codebase: " << CodeBase().GetCount());
			MLOG("TryLoadCodeBase loaded: " << MemoryUsedKb());
			return true;
		}
	}
	return false;
}

void LoadCodeBase()
{
	CppBaseLock __;
	MLOG("LoadCodeBase start: " << MemoryUsedKb());
	TryLoadCodeBase(CodeBaseCacheFile()) ||
	TryLoadCodeBase(AppendFileName(CodeBaseCacheDir(), GetVarsName() + ".*." + IdeGetCurrentBuildMethod() + ".codebase")) ||
	TryLoadCodeBase(AppendFileName(CodeBaseCacheDir(), GetVarsName() + ".*.codebase")) ||
	TryLoadCodeBase(AppendFileName(CodeBaseCacheDir(), "*.codebase"));
	
	LLOG("LoadCodeBase: " << CodeBase().GetCount());
}

void FinishCodeBase()
{
	LTIMING("FinishBase");

	CppBaseLock __;
	Qualify(CodeBase());
}

void LoadDefs()
{
	LTIMING("LoadDefs");
	CppBaseLock __;
	Vector<String> defs;
	defs.Add(ConfigFile("global.defs"));
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pk = wspc.GetPackage(i);
		String n = wspc[i];
		for(int i = 0; i < pk.file.GetCount(); i++) {
			String path = SourcePath(n, pk.file[i]);
			if(GetFileExt(path) == ".defs")
				defs.Add(path);
		}
	}
	
	String fp;
	for(int i = 0; i < defs.GetCount(); i++)
		fp << defs[i] << "\n" << GetFileTimeCached(defs[i]) << "\n";

	static String defs_fp;
	if(fp != defs_fp) {
		defs_fp = fp;
		String h;
		for(int i = 0; i < defs.GetCount(); i++)
			h << LoadFile(defs[i]) << "\n";
		SetPPDefs(h);
	}
}

void BaseInfoSync(Progress& pi)
{ // clears temporary caches (file times etc..)
	CppBaseLock __;
	PPSync(TheIde()->IdeGetIncludePath());

	LTIMESTOP("Gathering files");
	ClearSources();
	LoadDefs();
	const Workspace& wspc = GetIdeWorkspace();
	LTIMING("Gathering files");
	pi.SetText("Gathering files");
	pi.SetTotal(wspc.GetCount());
	
	{
	RTIMESTOP("GatherSources");
	for(int pass = 0; pass < 2; pass++)
		for(int i = 0; i < wspc.GetCount(); i++) {
			pi.Step();
			const Package& pk = wspc.GetPackage(i);
			String n = wspc[i];
			for(int i = 0; i < pk.file.GetCount(); i++) {
				String path = SourcePath(n, pk.file[i]);
				if(pass ? IsHFile(path)
				   : IsCPPFile(path) || findarg(ToLower(GetFileExt(path)), ".lay", ".sch", ".iml") >= 0)
					GatherSources(path, path);
			}
		}
	}

	SweepPPFiles(GetAllSources());
}

int GetSourceFileIndex(const String& path)
{
	return source_file.FindPut(NormalizeSourcePath(path));
}

String GetSourceFilePath(int file)
{
	if(file < 0 || file >= source_file.GetCount())
		return Null;
	return source_file.GetKey(file);
}

Index<String> sTimePath;

Time GetDependsTime(const Vector<int>& file)
{
	LTIMING("CreateTimePrint");
	Time tm = Time::Low();
	INTERLOCKED {
		static Index<String> path;
		for(int i = 0; i < file.GetCount(); i++)
			if(file[i] < sTimePath.GetCount())
				tm = max(tm, GetFileTimeCached(sTimePath[file[i]]));
	}
	return tm;
}

bool CheckFile(SourceFileInfo& f, const String& path)
{
	LTIMING("CheckFile");
	CppBaseLock __;
	Time ftm = GetFileTimeCached(path);
	bool tmok = f.time == ftm;
	f.time = ftm;
	if(findarg(ToLower(GetFileExt(path)), ".lay", ".iml", ".sch") >= 0)
		return tmok;
	if(!IsNull(f.depends_time) && tmok && f.depends_time == GetDependsTime(f.depends) && f.dependencies_md5sum.GetCount())
		return true;
	Index<String> visited;
	String md5 = GetDependeciesMD5(path, visited);
	bool r = f.dependencies_md5sum == md5 && tmok;
#ifdef HAS_CLOG
	if(!r) CLOG(path << " " << f.dependencies_md5sum << " " << md5);
#endif
	f.depends.Clear();
	f.dependencies_md5sum = md5;
	for(int i = 0; i < visited.GetCount(); i++)
		f.depends.Add(sTimePath.FindAdd(visited[i]));
	f.depends_time = GetDependsTime(f.depends);
	return r;
}

void UpdateCodeBase2(Progress& pi)
{
	CLOG("============= UpdateCodeBase2 " << GetSysTime());
	CppBaseLock __;
	pi.SetText("Checking source files");
	pi.SetPos(0);
	Index<int>  keep_file;
	Index<int>  parse_file;
	CLOG("Gathered files: " << GetAllSourceMasters());
	const Index<String>& src = GetAllSources();
	pi.SetTotal(src.GetCount());
	for(int i = 0; i < src.GetCount(); i++) {
		pi.Step();
		String path = src[i];
		int q = GetSourceFileIndex(path);
		SourceFileInfo& f = source_file[q];
		LLOG("== CHECK == " << q << ": " << path);
		if(CheckFile(f, path))
			keep_file.Add(q);
		else {
			LLOG("PARSE: " << path);
			parse_file.Add(q);
		}
	}
	
	CppBase& base = CodeBase();

	base.Sweep(keep_file);

	for(int i = 0; i < source_file.GetCount(); i++) {
		if(keep_file.Find(i) < 0 && parse_file.Find(i) < 0 && !source_file.IsUnlinked(i)) {
			source_file.Unlink(i);
		}
	}

#ifdef HAS_CLOG
	for(int i = 0; i < source_file.GetCount(); i++)
		if(!source_file.IsUnlinked(i))
			CLOG(i << " " << source_file.GetKey(i) << " " << source_file[i].dependencies_md5sum << " " << source_file[i].time);
#endif

	// This is the only place where parsing runs in parallel
	pi.SetTotal(parse_file.GetCount());
	pi.SetPos(0);
	pi.AlignText(ALIGN_LEFT);
	LLOG("=========================");
	RTIMESTOP("Parsing files");
	CoFor(parse_file.GetCount(), [&](int i) {
		String path = GetSourceFilePath(parse_file[i]);
		pi.SetText(GetFileName(GetFileFolder(path)) + "/" + GetFileName(path));
		pi.Step();
		FileIn fi(path);
		LDUMP(path);
		LDUMP(parse_file[i]);
		ParseSrc(fi, parse_file[i], callback1(BrowserScanError, i));
	});
}

void UpdateCodeBase(Progress& pi)
{
	CppBaseLock __;
	BaseInfoSync(pi);

	UpdateCodeBase2(pi);
}

void ParseSrc(Stream& in, int file, Event<int, const String&> error)
{
	String path = GetSourceFilePath(file);
	CLOG("====== Parse " << file << ": " << path);
	CppBase base;
	Vector<String> pp;
	String ext = ToLower(GetFileExt(path));
	if(ext == ".lay")
		pp.Add(PreprocessLayFile(path));
	else
	if(ext == ".iml")
		pp.Add(PreprocessImlFile(path));
	else
	if(ext == ".sch")
		pp.Append(PreprocessSchFile(path));
	else
		PreprocessParse(base, in, file, path, error);

	for(int i = 0; i < pp.GetCount(); i++)
		Parse(base, pp[i], file, FILE_OTHER, path, error, Vector<String>(), Index<String>());
	
	INTERLOCKED
		CodeBase().Append(pick(base));
}

void CodeBaseScanFile0(Stream& in, const String& fn)
{
	LLOG("===== CodeBaseScanFile " << fn);
	CppBaseLock __;

	InvalidateFileTimeCache(NormalizeSourcePath(fn));
	PPSync(TheIde()->IdeGetIncludePath());

	LTIMING("CodeBaseScan");
	
	int file = GetSourceFileIndex(fn);
	CppBase& base = CodeBase();
	base.RemoveFile(file);
	ParseSrc(in, file, CNULL);
}

void CodeBaseScanFile(Stream& in, const String& fn)
{
	CppBaseLock __;
	CodeBaseScanFile0(in, fn);
	FinishCodeBase();
}

void CodeBaseScanFile(const String& fn, bool auto_check)
{
	LLOG("CodeBaseScanFile " << fn);
	CppBaseLock __;
	String md5sum = GetPPMD5(fn);
	FileIn in(fn);
	CodeBaseScanFile(in, fn);
	int file = GetSourceFileIndex(fn);
	SourceFileInfo& f = source_file[file];
	CLOG("CodeBaseScanFile " << fn << ", " << md5sum << " " << f.md5sum);
	if(md5sum != f.md5sum) {
		if(auto_check)
			SyncCodeBase();
		f.md5sum = md5sum;
	}
	else
		FinishCodeBase();
}

void ClearCodeBase()
{
	// TODO: Create combined defs
	CppBaseLock __;
	CleanPP();
	CodeBase().Clear();
	source_file.Clear();
}

void SyncCodeBase()
{
	LTIMING("SyncCodeBase");
	LTIMESTOP("SyncCodeBase");
	CLOG("============= Sync code base");
	CppBaseLock __;
	if(IsNull(IdeGetCurrentMainPackage())) {
		ClearCodeBase();
		return;
	}
	Progress pi;
	pi.Title("Parsing source files");
	UpdateCodeBase(pi);
	FinishCodeBase();
}

void NewCodeBase()
{
	CppBaseLock __;
	ReduceCodeBaseCache();
	if(IsNull(IdeGetCurrentMainPackage())) {
		ClearCodeBase();
		return;
	}
	static int start;
	if(start) return;
	start++;
	LoadCodeBase();
	LLOG("NewCodeBase loaded " << CodeBase().GetCount());
	SyncCodeBase();
	LLOG("NewCodeBase synced " << CodeBase().GetCount());
	SaveCodeBase();
	LLOG("NewCodeBase saved " << CodeBase().GetCount());
	start--;
}

void RescanCodeBase()
{
	CppBaseLock __;
	ClearCodeBase();
	s_console = true;
	Progress pi;
	pi.Title("Parsing source files");
	UpdateCodeBase(pi);
	FinishCodeBase();
	s_console = false;
}

bool ExistsBrowserItem(const String& item)
{
	CppBaseLock __;
	return GetCodeRefItem(item);
}
