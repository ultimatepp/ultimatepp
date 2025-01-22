#include "Core.h"

String GetLocalDir()
{
	return ConfigFile("UppLocal");
}

String LocalPath(const String& filename)
{
	return AppendFileName(GetLocalDir(), filename);
}

String FollowCygwinSymlink(const String& file) {
	for(String fn = file;;) {
		if(fn.IsEmpty())
			return fn;
		FileIn fi(fn);
		if(!fi.IsOpen())
			return fn;
		char buffer[10];
		if(!fi.GetAll(buffer, 10) || memcmp(buffer, "!<symlink>", 10))
			return fn;
		fn = NormalizePath(LoadStream(fi), GetFileDirectory(fn));
	}
}

Vector<String> SplitDirs(const char *s) {
#ifdef PLATFORM_POSIX
	return Split(s, [](int c) { return findarg(c, ';', ':') >= 0 ? c : 0; });
#else
	return Split(s, ';');
#endif
}

static String varsname = "default";

String GetVarsName()
{
	return varsname;
}

String VarFilePath(String name) {
	return ConfigFile(name + ".var");
}

String VarFilePath() {
	String p = varsname;
	if(IsFullPath(varsname))
		return varsname;
	return VarFilePath(GetVarsName());
}

bool SaveVarFile(const char *filename, const VectorMap<String, String>& var)
{
	FileOut out(filename);
	for(int i = 0; i < var.GetCount(); i++)
		out << var.GetKey(i) << " = " << AsCString(var[i]) << ";\n";
	out.Close();
	return !out.IsError();
}

bool LoadVarFile(const char *name, VectorMap<String, String>& _var)
{
	try {
		VectorMap<String, String> var;
		String env = LoadFile(name);
		try {
			CParser p(env);
			while(!p.IsEof()) {
				String v = p.ReadId();
				p.Char('=');
				if(p.IsString())
					var.GetAdd(v) = p.ReadString();
				else {
					String ln;
					while(p.PeekChar() != '\r' && p.PeekChar() != '\n' && p.PeekChar() != ';')
						ln.Cat(p.GetChar());
					var.GetAdd(v) = ln;
					p.Spaces();
				}
				p.Char(';');
			}
		}
		catch(CParser::Error) {}
		_var = pick(var);
		return true;
	}
	catch(...) {
		return false;
	}
}

bool Nest::Save(const char *path)
{
	return SaveVarFile(path, var);
}

bool Nest::Load(const char *path)
{
	InvalidatePackageCache();
	return LoadVarFile(path, var);
}

String Nest::Get(const String& id)
{
	return var.Get(id, String());
}

void Nest::Set(const String& id, const String& val)
{
	var.GetAdd(id) = val;
	InvalidatePackageCache();
}

String Nest::PackagePath0(const String& name)
{
	String uppfile = NativePath(name);
	if(IsFullPath(uppfile)) return NormalizePath(uppfile);
	Vector<String> d = GetUppDirs();
	String p;
	for(int i = 0; i < d.GetCount(); i++) {
		p = NormalizePath(AppendFileName(AppendFileName(d[i], uppfile),
		                  GetFileName(uppfile)) + ".upp");
		if(FileExists(p)) return p;
	}
	return d.GetCount() ? NormalizePath(AppendFileName(AppendFileName(d[0], uppfile),
		                                GetFileName(uppfile)) + ".upp") : String();
}

String Nest::PackagePath(const String& name)
{
	int q = package_cache.Find(name);
	if(q < 0) {
		String h = PackagePath0(name);
		package_cache.Add(name, h);
		return h;
	}
	return package_cache[q];
}

Nest& MainNest()
{
	return Single<Nest>();
}

bool SaveVars(const char *name)
{
	if(!MainNest().Save(VarFilePath(name)))
		return false;
	varsname = name;
	return true;
}

#ifdef PLATFORM_POSIX

String GetDefaultUppOut()
{
	String out = GetFileFolder(GetUserConfigDir()) + "/.cache/upp.out";
	RealizeDirectory(out);
	return out;
}

#else

String GetDefaultUppOut()
{
	return ConfigFile("out");
}

#endif

String GetUppOut()
{
	return Nvl(GetVar("OUTPUT"), GetDefaultUppOut());
}

String DefaultHubFilePath()
{
	return ConfigFile("UppHub.path");
}

bool hubdir_resolved;

void SetHubDir(const String& path)
{
	SaveChangedFile(DefaultHubFilePath(), path);
	hubdir_resolved = false;
}

static String override_hub_dir;

void OverrideHubDir(const String& path)
{
	override_hub_dir = path;
}

String GetHubDir()
{
	if(override_hub_dir.GetCount())
		return override_hub_dir;
	static String d;
	if(hubdir_resolved)
		return d;
	hubdir_resolved = true;
	d = GetVar("UPPHUB");
	if(d.GetCount() == 0 || !DirectoryExists(d)) {
		d = LoadFile(DefaultHubFilePath());
		if(d.GetCount() == 0 || !DirectoryExists(d)) {
			d = ConfigFile("UppHub");
			RealizeDirectory(d);
		}
	}
	d = NormalizePath(d);
	return d;
}

bool InUppHub(const String& p)
{
	return p.StartsWith(GetHubDir());
}

bool   LoadVars(const char *name) {
	varsname = (name && *name ? name : "default");
	return MainNest().Load(VarFilePath());
}

String GetVar(const String& var) {
	return MainNest().Get(var);
}

void SetVar(const String& var, const String& val, bool save) {
	MainNest().Set(var, val);
	if(save)
		SaveVars(GetVarsName());
}

bool hub_loaded;

String main_nest;

void SetMainNest(const String& n)
{
	main_nest = n;
}

String GetAssemblyId()
{
	String id = GetVarsName();
	Vector<String> s = SplitDirs(GetVar("UPP"));
	if(s.GetCount() && main_nest.GetCount() && s[0] != main_nest)
		id << "_" << GetFileName(main_nest);
	return id;
}

Vector<String> GetUppDirsRaw()
{
	Vector<String> s = SplitDirs(GetVar("UPP"));
	static Vector<String> hub_dirs;
	if(!hub_loaded) {
		hub_dirs.Clear();
		for(const FindFile& ff : FindFile(GetHubDir() + "/*.*"))
			if(ff.IsFolder())
				hub_dirs.Add(ff.GetPath());
		hub_loaded = true;
	}
	s.Append(hub_dirs);
	return s;
}

Vector<String> GetUppDirs()
{
	Vector<String> s = GetUppDirsRaw();
	if(main_nest.GetCount() && (s.GetCount() == 0 || main_nest != s[0]))
		s.Insert(0, main_nest);
	return s;
}

bool IsHubDir(const String& path)
{
	return NormalizePath(path).StartsWith(GetHubDir());
}

void Nest::InvalidatePackageCache()
{
	package_cache.Clear();
	hub_loaded = false;
}

String GetUppDir() {
	Vector<String> s = GetUppDirs();
#ifdef PLATFORM_WIN32
	return s.GetCount() == 0 ? GetFileFolder(GetExeFilePath()) : s[0];
#endif
#ifdef PLATFORM_POSIX
	return s.GetCount() == 0 ? GetHomeDirectory() : s[0];
#endif
}
