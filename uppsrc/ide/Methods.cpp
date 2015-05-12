#include "ide.h"

class TextOption : public Option {
public:
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;
};

void  TextOption::SetData(const Value& data)
{
	String s = data;
	Set(!(IsNull(s) || s == "0"));
}

Value TextOption::GetData() const
{
	return Get() ? "1" : "0";
}

class TextSwitch : public Switch {
public:
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;
};

void  TextSwitch::SetData(const Value& data)
{
	String s = data;
	Switch::SetData(atoi((String)data));
}

Value TextSwitch::GetData() const
{
	return AsString(Switch::GetData());
}

class DirTable : public ArrayCtrl {
public:
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;

protected:
	void Modify()  { Update(); }

	EditString      edit;
	SelectDirButton edit_dir;

	void Init(const char *name = NULL);

public:
	DirTable();
	DirTable(const char *name);
};

void   DirTable::SetData(const Value& data)
{
	Vector<String> l = Split((String)data, ';');
	Clear();
	for(int i = 0; i < l.GetCount(); i++)
		Add(l[i]);
}

Value  DirTable::GetData() const
{
	String s;
	for(int i = 0; i < GetCount(); i++) {
		if(i) s << ';';
		s << (String)Get(i, 0);
	}
	return s;
}

void DirTable::Init(const char *name)
{
	AutoHideSb();
	AddColumn(name).Edit(edit);
	Appending().Removing().Moving();
	edit_dir.Attach(edit);
	edit_dir.AllFilesType();
	WhenArrayAction = edit <<= callback(this, &DirTable::Modify);
}

DirTable::DirTable()
{
	Init();
	NoHeader();
}

DirTable::DirTable(const char *name)
{
	Init(name);
}

class DirMap : public ArrayCtrl {
public:
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;

protected:
	void Modify()  { Update(); }

	EditString      localpath, remotepath;
	SelectDirButton edit_dir;

public:
	DirMap();
};

void DirMap::SetData(const Value& data)
{
	Vector<String> l = Split((String)data, ';');
	Clear();
	for(int i = 0; i < l.GetCount(); i++) {
		String li = l[i];
		int f = li.Find('>');
		if(f >= 0)
			Add(li.Left(f), li.Mid(f + 1));
	}
}

Value DirMap::GetData() const
{
	String s;
	for(int i = 0; i < GetCount(); i++) {
		if(i) s << ';';
		s << (String)Get(i, 0) << '>' << (String)Get(i, 1);
	}
	return s;
}

DirMap::DirMap()
{
	AutoHideSb();
	AddColumn("Map local path").Edit(localpath);
	AddColumn("To remote path").Edit(remotepath);
	Appending().Removing().Moving();
	edit_dir.Attach(localpath);
	WhenArrayAction = localpath <<= remotepath <<= callback(this, &DirMap::Modify);
}

struct BuildMethods : public WithBuildMethodsLayout<TopWindow>
{
	TextOption debug_blitz;
	TextSwitch debug_linkmode;
	TextOption release_blitz;
	TextSwitch release_linkmode;
	TextOption linkmode_lock;
	TextOption allow_pch;
	DirTable   path;
	DirTable   include;
	DirTable   lib;
	DirMap     remote_path_map;
	OpenFileButton open_script;

	EditStringNotNull name;
	Index<String>     origfile;
	String            default_method;

	void Load();
	bool Save();

	void NewBuilder();
	void ShowDefault();
	void SetDefault();
	void ChangeMethod();
	void Import();

	void MethodMenu(Bar& bar);

	typedef BuildMethods CLASSNAME;

	BuildMethods();
};

int CharFilterFileName(int c)
{
	return IsAlNum(c) || c == '_' ? c : 0;
}

BuildMethods::BuildMethods()
{
	CtrlLayoutOKCancel(*this, "Build methods");
	method.AddColumn("Method").Edit(name);
	name.SetFilter(CharFilterFileName);
	method.AddCtrl("BUILDER", builder);
	method.AddCtrl("COMPILER", compiler);
	method.AddCtrl("COMMON_OPTIONS", common_options);
	method.AddCtrl("COMMON_CPP_OPTIONS", common_cpp_options);
	method.AddCtrl("COMMON_C_OPTIONS", common_c_options);
	method.AddCtrl("COMMON_FLAGS", common_flags);
	method.AddCtrl("DEBUG_INFO", debug_info);
	method.AddCtrl("DEBUG_BLITZ", debug_blitz);
	method.AddCtrl("DEBUG_LINKMODE", debug_linkmode);
	method.AddCtrl("DEBUG_OPTIONS", debug_options);
	method.AddCtrl("DEBUG_FLAGS", debug_flags);
	method.AddCtrl("DEBUG_LINK", debug_link);
	method.AddCtrl("RELEASE_BLITZ", release_blitz);
	method.AddCtrl("RELEASE_LINKMODE", release_linkmode);
	method.AddCtrl("RELEASE_OPTIONS", speed_options);
	method.AddCtrl("RELEASE_SIZE_OPTIONS", size_options);
	method.AddCtrl("RELEASE_FLAGS", release_flags);
	method.AddCtrl("RELEASE_LINK", release_link);
	method.AddCtrl("DEBUGGER", debugger);
	method.AddCtrl("PATH", path);
	method.AddCtrl("INCLUDE", include);
	method.AddCtrl("LIB", lib);
#if 0 // REMOTE REMOVED
	method.AddCtrl("REMOTE_HOST", remote_host);
	method.AddCtrl("REMOTE_OS", remote_os);
	remote_os.Add("WIN32");
	remote_os.Add("LINUX");
	remote_os.Add("WINCE");
	remote_os.Add("UNIX");
	remote_os.Add("SOLARIS");
	remote_os.Add("BSD");
	method.AddCtrl("REMOTE_TRANSFER", remote_file_access);
	remote_file_access.Add("0", "direct (SAMBA)");
	remote_file_access.Add("1", "indirect (transfer)");
	method.AddCtrl("REMOTE_MAP", remote_path_map);
#endif
	method.AddCtrl("SCRIPT", scriptfile);
	method.AddCtrl("LINKMODE_LOCK", linkmode_lock);
	
	allow_pch.SetLabel("Allow precompiled headers");
	method.AddCtrl("ALLOW_PRECOMPILED_HEADERS", allow_pch);
	
	open_script.Attach(scriptfile);
	open_script.Type("Build scripts (*.bsc)", "*.bsc")
		.AllFilesType();
	open_script.DefaultExt("bsc");
	method.Appending().Removing().Duplicating();
	method.WhenCursor = THISBACK(ChangeMethod);
	method.WhenBar = THISBACK(MethodMenu);

	paths.Add(path.SizePos(), "PATH - executable directories");
	paths.Add(include.SizePos(), "INCLUDE directories");
	paths.Add(lib.SizePos(), "LIB directories");

	debug_info.Add("0", "None");
	debug_info.Add("1", "Minimal");
	debug_info.Add("2", "Full");

	for(int i = 0; i < BuilderMap().GetCount(); i++)
		builder.Add(BuilderMap().GetKey(i));

	builder <<= THISBACK(NewBuilder);
	setdefault <<= THISBACK(SetDefault);

	linkmode_lock.SetLabel("Lock link mode");
}

void BuildMethods::MethodMenu(Bar& bar)
{
	method.StdBar(bar);
	bar.Separator();
	bar.Add("Import", THISBACK(Import));
}

void BuildMethods::Import()
{
	if(!Save())
		return;
	FileSel fsel;
	fsel.Type("Build methods (*.bm)", "*.bm")
		.AllFilesType()
		.Multi()
		.DefaultExt("bm");
	if(!fsel.ExecuteOpen())
		return;
	for(int i = 0; i < fsel.GetCount(); i++) {
		String f = LoadFile(fsel[i]);
		if(f.IsVoid()) {
			if(!PromptOKCancel(NFormat("Failed to load [* \1%s\1]. Continue?", fsel[i])))
				break;
			continue;
		}
		String nf = ConfigFile(GetFileNamePos(fsel[i]));
		if(FileExists(nf) && !PromptOKCancel(NFormat("File already exists: [* \1%s\1]. Overwrite?", nf)))
			continue;
		if(!SaveFile(nf, f))
			if(!PromptOKCancel(NFormat("Failed to save [* \1%s\1]. Continue?", nf)))
				break;
	}
	Load();
}

static int sCompare(const Value& v1, const Value& v2)
{
	return (String)v1 < (String)v2;
}

void BuildMethods::NewBuilder()
{
	String b = ~builder;
	bool gcc = b == "GCC" || b == "GCC32" || b == "GCC_ARM";
	if(IsNull(speed_options)) {
		if(gcc)
			speed_options <<= "-O3 -ffunction-sections -fdata-sections";
		else
			speed_options <<= "-O2";
	}
	if(IsNull(size_options)) {
		if(gcc)
			size_options <<= "-Os -finline-limit=20 -ffunction-sections -fdata-sections";
		else
			size_options <<= "-O1";
	}
	if(IsNull(debug_options)) {
		if(gcc)
			debug_options <<= "-O0";
		else
			debug_options <<= "-Od";
	}
	if(IsNull(debugger)) {
		if(gcc)
			debugger <<= "gdb";
		else
			debugger <<= "msdev";
	}
	if(IsNull(release_link) && gcc)
		release_link <<= "-Wl,--gc-sections";
	ChangeMethod();
}

void BuildMethods::ChangeMethod()
{
	String b;
	if(method.IsCursor())
		b = method.Get("BUILDER");
	scriptfile.Enable(b == "SCRIPT");
}

void BuildMethods::Load()
{
	FindFile ff(ConfigFile("*.bm"));
	while(ff) {
		VectorMap<String, String> map;
		String fn = ConfigFile(ff.GetName());
		if(LoadVarFile(fn, map)) {
			origfile.Add(fn);
			method.Add(GetFileTitle(fn));
			for(int j = 1; j < method.GetIndexCount(); j++)
				method.Set(method.GetCount() - 1, j, map.Get(method.GetId(j).ToString(), Null));
		}
		ff.Next();
	}
	method.Sort(0, sCompare);
	method.GoBegin();
}

bool BuildMethods::Save()
{
	int i;
	Index<String> name;
	for(i = 0; i < method.GetCount(); i++) {
		String n = method.Get(i, 0);
		if(name.Find(n) >= 0) {
			Exclamation("Duplicate method [* " + DeQtf(n) + "] !");
			return false;
		}
		name.Add(n);
	}
	Index<String> saved;
	for(i = 0; i < method.GetCount(); i++) {
		VectorMap<String, String> map;
		for(int j = 1; j < method.GetIndexCount(); j++)
			map.Add(method.GetId(j).ToString(), method.Get(i, j));
		if(map.Get("BUILDER", "") != "SCRIPT")
			map.RemoveKey("SCRIPT");
		String fn = ConfigFile(String(method.Get(i, 0)) + ".bm");
		if(!SaveVarFile(fn, map)) {
			Exclamation("Error saving [* " + fn + "] !");
			return false;
		}
		saved.Add(fn);
	}
	for(i = 0; i < origfile.GetCount(); i++)
		if(saved.Find(origfile[i]) < 0)
			DeleteFile(origfile[i]);
	return true;
}

struct BoldDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
					   Color ink, Color paper, dword style) const {
		w.DrawRect(r, paper);
		DrawSmartText(w, r.left, r.top, r.Width(), (String)q, StdFont().Bold(), ink);
	}
};


void BuildMethods::ShowDefault()
{
	String m = GetDefaultMethod();
	for(int i = 0; i < method.GetCount(); i++)
		if((String)method.Get(i, 0) == m)
			method.SetDisplay(i, 0, Single<BoldDisplay>());
		else
			method.SetDisplay(i, 0, StdDisplay());
}

void BuildMethods::SetDefault()
{
	if(method.IsCursor()) {
		SaveFile(ConfigFile("default_method"), method.GetKey());
		ShowDefault();
	}
}

void Ide::SetupBuildMethods()
{
	BuildMethods m;
	m.Load();
	m.ShowDefault();
	m.use_target = use_target;
	m.method.FindSetCursor(method);
	for(;;) {
		int c = m.Run();
		if(c == IDCANCEL)
			break;
		if(c == IDOK && m.Save()) {
			use_target = m.use_target;
			break;
		}
	}
	SyncCodeBase();
	SyncBuildMode();
	SetBar();
}

String Ide::GetIncludePath()
{
	SetupDefaultMethod();
	VectorMap<String, String> bm = GetMethodVars(method);
	String include = GetVar("UPP") + ';' + bm.Get("INCLUDE", "");
#ifdef PLATFORM_POSIX
	static String sys_includes;
	ONCELOCK {
		Index<String> r;
		for(int pass = 0; pass < 2; pass++) {
			String h = Sys(pass ? "clang -v -x c++ -E /dev/null" : "gcc -v -x c++ -E /dev/null");
			Vector<String> ln = Split(h, '\n');
			for(int i = 0; i < ln.GetCount(); i++) {
				String dir = TrimBoth(ln[i]);
				if(DirectoryExists(dir))
					r.FindAdd(NormalizePath(dir));
			}
		}
		r.FindAdd("/usr/include");
		r.FindAdd("/usr/local/include");
		sys_includes = Join(r.GetKeys(), ";");
	}
	MergeWith(include, ";", sys_includes);
#endif
	;

	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pkg = wspc.GetPackage(i);
		for(int j = 0; j < pkg.include.GetCount(); j++)
			MergeWith(include, ";", SourcePath(wspc[i], pkg.include[j].text));
	}
	
	return include;
}

String Ide::IdeGetIncludePath()
{
	return GetIncludePath();
}
