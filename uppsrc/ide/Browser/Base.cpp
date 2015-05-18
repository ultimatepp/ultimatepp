#include "Browser.h"

#include <plugin/lz4/lz4.h>

#define LTIMING(x)    // RTIMING(x)
#define LLOG(x)       // DLOG(x)
#define LTIMESTOP(x)  // RTIMESTOP(x)

#define LDUMP(x)      // DDUMP(x)

ArrayMap<String, SourceFileInfo> source_file;

void SourceFileInfo::Serialize(Stream& s)
{
	s % time % check_info % ids % included_id_macros % namespace_info % using_info
	  % defined_macros % defined_namespace_info % includes;
	if(s.IsLoading()) {
		depends.Clear();
		depends_time = Null;
	}
}

String CodeBaseCacheDir()
{
	return ConfigFile("cfg/codebase");
}

struct RCB_FileInfo {
	String path;
	Time   time;
	int64  length;
	
	bool operator<(const RCB_FileInfo& a) const { return time > a.time; }
};

void ReduceCodeBaseCache()
{
	Array<RCB_FileInfo> file;
	FindFile ff(AppendFileName(CodeBaseCacheDir(), "*.*"));
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
	while(total > 120000000 && file.GetCount()) {
		DeleteFile(file.Top().path);
		total -= file.Top().length;
		file.Drop();
	}
}

String CodeBaseCacheFile()
{
	return AppendFileName(CodeBaseCacheDir(), GetVarsName() + '.' + GetCurrentMainPackage() + '.' + GetCurrentBuildMethod() + ".codebase");
}

CppBase& CodeBase()
{
	static CppBase b;
	return b;
}

static bool   s_console;

void IdePutErrorLine(const String& line);

void BrowserScanError(int line, const String& text, int file)
{
	if(s_console)
		IdePutErrorLine(String().Cat() << source_file.GetKey(file) << " (" << line << "): " << text);
}

void SerializeCodeBase(Stream& s)
{
	source_file.Serialize(s);
	SerializePPFiles(s);
	CodeBase().Serialize(s);
}

#define CPP_CODEBASE_VERSION 9

void SaveCodeBase()
{
	LTIMING("SaveCodeBase");
	LLOG("Save code base " << CodeBase().GetCount());
	RealizeDirectory(ConfigFile("cfg/codebase"));
	StringStream ss;
	Store(callback(SerializeCodeBase), ss, CPP_CODEBASE_VERSION);
	String data = ss.GetResult();
	String path = CodeBaseCacheFile();
	SaveFile(path, LZ4Compress(data));
}

bool TryLoadCodeBase(const char *pattern)
{
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
		if(Load(callback(SerializeCodeBase), ss, CPP_CODEBASE_VERSION)) {
			LLOG("Loaded " << ff.GetPath());
			return true;
		}
	}
	return false;
}

void LoadCodeBase()
{
	TryLoadCodeBase(CodeBaseCacheFile()) ||
	TryLoadCodeBase(AppendFileName(CodeBaseCacheDir(), GetVarsName() + ".*." + GetCurrentBuildMethod() + ".codebase")) ||
	TryLoadCodeBase(AppendFileName(CodeBaseCacheDir(), GetVarsName() + ".*.codebase")) ||
	TryLoadCodeBase(AppendFileName(CodeBaseCacheDir(), "*.codebase"));
	
	LLOG("LoadCodeBase: " << CodeBase().GetCount());
}

void FinishCodeBase()
{
	LTIMING("FinishBase");

	Qualify(CodeBase());
}

void BaseInfoSync(Progress& pi)
{ // clears temporary caches (file times etc..)
	PPSync(TheIde()->IdeGetIncludePath());

	LTIMESTOP("Gathering files");
	ClearSources();
	const Workspace& wspc = GetIdeWorkspace();
	LTIMING("Gathering files");
	Vector<String> defs;
	defs.Add(ConfigFile("global.defs"));
	for(int i = 0; i < wspc.GetCount(); i++) {
		pi.Step();
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

	pi.SetText("Gathering files");
	pi.SetTotal(wspc.GetCount());
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

	SweepPPFiles(GetAllSources());
}

int GetSourceFileIndex(const String& path)
{
	return source_file.FindPut(path);
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
	static Index<String> path;
	String r;
	Time tm = Time::Low();
	for(int i = 0; i < file.GetCount(); i++)
		if(file[i] < sTimePath.GetCount())
			tm = max(tm, GetFileTimeCached(sTimePath[file[i]]));
	return tm;
}

bool CheckFile(SourceFileInfo& f, const String& path)
{
	LTIMING("CheckFile");
	if(f.time != GetFileTimeCached(path))
		return false;
	if(!f.check_info)
		return true;
	if(!IsNull(f.depends_time) && f.depends_time == GetDependsTime(f.depends))
		return true;
	Cpp pp;
	FileIn in(path);
	pp.Preprocess(path, in, GetMasterFile(path), true);
	String included_id_macros = pp.GetIncludedMacroValues(f.ids.GetKeys());
	if(f.included_id_macros != included_id_macros) {
		f.depends_time = Null;
		f.depends.Clear();
		return false;
	}
	for(int i = 0; i < pp.visited.GetCount(); i++)
		f.depends.Add(sTimePath.FindAdd(pp.visited[i]));
	f.depends_time = GetDependsTime(f.depends);
	return true;
}

void ParseFiles(Progress& pi, const Index<int>& parse_file)
{
	pi.SetTotal(parse_file.GetCount());
	pi.SetPos(0);
	pi.AlignText(ALIGN_LEFT);
	for(int i = 0; i < parse_file.GetCount(); i++) {
		String path = GetSourceFilePath(parse_file[i]);
		pi.SetText(GetFileName(GetFileFolder(path)) + "/" + GetFileName(path));
		pi.Step();
		FileIn fi(path);
		LDUMP(path);
		LDUMP(parse_file[i]);
		bool dummy;
		ParseSrc(fi, parse_file[i], callback1(BrowserScanError, i), true, false, dummy, dummy);
	}
}

void UpdateCodeBase2(Progress& pi)
{
	const Workspace& wspc = GetIdeWorkspace();

	pi.SetText("Checking source files");
	pi.SetPos(0);
	Index<int>  keep_file;
	Index<int>  parse_file;
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

	for(int i = 0; i < source_file.GetCount(); i++)
		if(keep_file.Find(i) < 0 && parse_file.Find(i) < 0 && !source_file.IsUnlinked(i))
			source_file.Unlink(i);
	
	ParseFiles(pi, parse_file);
}

void UpdateCodeBase(Progress& pi)
{
	BaseInfoSync(pi);

	UpdateCodeBase2(pi);
}

Vector<String> ParseSrc(Stream& in, int file, Callback2<int, const String&> error,
                        bool do_macros, bool get_changes,
                        bool& namespace_info_changed,
                        bool& includes_changed)
{
	Vector<String> cm;
	String path = GetSourceFilePath(file);
	LLOG("====== Parse " << file << ": " << path);
	Vector<String> pp;
	String ext = ToLower(GetFileExt(path));
	int filetype = FILE_OTHER;
	SourceFileInfo& sfi = source_file[file];
	sfi.time = FileGetTime(path);
	Cpp cpp;
	bool b = false;
	if(ext == ".lay")
		pp.Add(PreprocessLayFile(path));
	else
	if(ext == ".iml")
		pp.Add(PreprocessImlFile(path));
	else
	if(ext == ".sch")
		pp.Append(PreprocessSchFile(path));
	else {
		sfi.check_info = true;
		cpp.Preprocess(path, in, GetMasterFile(GetSourceFilePath(file)));
		filetype = decode(ext, ".h", FILE_H, ".hpp", FILE_HPP,
		                       ".cpp",FILE_CPP, ".c", FILE_C, FILE_OTHER);
		if(do_macros) {
			sfi.ids = cpp.ids.PickKeys();
			sfi.included_id_macros = cpp.GetIncludedMacroValues(sfi.ids.GetKeys());
			LDUMP(sfi.ids);			
			LDUMP(sfi.time);
			VectorMap<String, String> dm = cpp.GetDefinedMacros();
			LDUMP(dm);
			if(sfi.defined_macros != dm) {
				LTIMING("Find changed macros");
				if(get_changes) {
					Buffer<bool> found(sfi.defined_macros.GetCount(), false);
					for(int i = 0; i < dm.GetCount() && cm.GetCount() < 10; i++) {
						String id = dm.GetKey(i);
						int q = sfi.defined_macros.Find(id);
						if(q < 0 || sfi.defined_macros[q] != dm[i])
							cm.Add(id);
						if(q >= 0)
							found[q] = true;
					}
					for(int i = 0; i < sfi.defined_macros.GetCount() && cm.GetCount() < 10; i++)
						if(!found[i])
							cm.Add(sfi.defined_macros.GetKey(i));
				}
				sfi.defined_macros = pick(dm);
			}
		}
		StringStream pin(cpp.output);
		Parser p;
		p.Do(pin, CodeBase(), file, filetype, GetFileName(path), error, Vector<String>(),
		     cpp.namespace_stack, cpp.namespace_using);
		if(sfi.defined_namespace_info != p.namespace_info) {
			sfi.defined_namespace_info = p.namespace_info;
			namespace_info_changed = true;
		}
		if(cpp.includes != sfi.includes) {
			sfi.includes = cpp.includes;
			includes_changed = true;
		}
		return cm;
	}

	for(int i = 0; i < pp.GetCount(); i++) {
		StringStream pin(pp[i]);
		Parser p;
		p.Do(pin, CodeBase(), file, filetype, GetFileName(path), error, Vector<String>(),
		     cpp.namespace_stack, cpp.namespace_using);
	}
	return cm;
}

bool HasIntersection(const Index<String>& ids, const Vector<String>& cm)
{
	if(cm.GetCount() >= 10)
		return true;
	for(int i = 0; i < cm.GetCount(); i++)
		if(ids.Find(cm[i]) >= 0)
			return true;
	return false;
}

void CodeBaseScanFile(Stream& in, const String& fn, bool check_macros)
{
	LLOG("===== CodeBaseScanFile " << fn);

	InvalidateFileTimeCache(fn);
	PPSync(TheIde()->IdeGetIncludePath());

	LTIMING("CodeBaseScan");
	
	int file = GetSourceFileIndex(fn);
	CppBase& base = CodeBase();
	base.RemoveFile(file);
	SourceFileInfo& f = source_file[file];
	bool namespace_info_changed = false;
	bool includes_changed = false;
	Vector<String> cm = ParseSrc(in, file, CNULL, check_macros, true,
	                             namespace_info_changed, includes_changed);
	LDUMP(f.defined_macros);
	LDUMP(check_macros);
	if(check_macros && (includes_changed || namespace_info_changed || cm.GetCount()))
		SyncCodeBase();
	else
		FinishCodeBase();
}

void CodeBaseScanFile(const String& fn, bool check_macros)
{
	FileIn in(fn);
	CodeBaseScanFile(in, fn, check_macros);
}

void ClearCodeBase()
{
	// TODO: Create combined defs
	CleanPP();
	CodeBase().Clear();
	source_file.Clear();
}

void SyncCodeBase()
{
	LTIMING("SyncCodeBase");
	LTIMESTOP("SyncCodeBase");
	LLOG("============= Sync code base");
	Progress pi;
	pi.Title("Parsing source files");
	UpdateCodeBase(pi);
	FinishCodeBase();
}

void NewCodeBase()
{
	ReduceCodeBaseCache();
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

/*
void CheckCodeBase()
{
	RTIMESTOP("CheckCodeBase");
	Progress pi;
	pi.Title("Checking source files");
	BaseInfoSync(pi);
	for(int i = 0; i < sSrcFile.GetCount(); i++)
		if(source_file.Find(sSrcFile.GetKey(i)) < 0) {
			UpdateCodeBase2(pi);
			FinishCodeBase();
			return;
		}
	for(int i = 0; i < source_file.GetCount(); i++)
		if(!source_file.IsUnlinked(i)) {
			String path = source_file.GetKey(i);
			if(sSrcFile.Find(source_file.GetKey(i)) < 0 || source_file[i].time != FileGetTime(path)) {
				UpdateCodeBase2(pi);
				FinishCodeBase();
				return;
			}
		}
}
*/

void RescanCodeBase()
{
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
	return GetCodeRefItem(item);
}
