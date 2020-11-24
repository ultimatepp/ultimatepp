#include "Core.h"

String GetLocalDir()
{
	return ConfigFile("UppLocal");
}

String LocalPath(const String& filename)
{
	return AppendFileName(GetLocalDir(), filename);
}

Vector<String> IgnoreList()
{
	Vector<String> ignore;
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pk = wspc.GetPackage(i);
		for(int j = 0; j < pk.GetCount(); j++)
			if(!pk[j].separator && pk[j] == "ignorelist") {
				FileIn in(SourcePath(wspc[i], pk[j]));
				while(in && !in.IsEof()) {
					String s = in.GetLine();
					if(!s.IsEmpty())
						ignore.Add(s);
				}
			}
	}
	return ignore;
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
	return Split(s, ';');
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
		                                GetFileName(uppfile)) + ".upp") : "";
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

String DefaultUpvFilePath()
{
	return ConfigFile("uppiverse.path");
}

String GetUpvDir()
{
	String d = GetVar("UPPIVERSE");
	if(d.GetCount() && DirectoryExists(d)) return d;
	d = LoadFile(DefaultUpvFilePath());
	if(d.GetCount() && DirectoryExists(d)) return d;
	d = ConfigFile("uppiverse");
	RealizeDirectory(d);
	return d;
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

bool upv_loaded;

Vector<String> GetUppDirs() {
	Vector<String> s = SplitDirs(GetVar("UPP"));
	static Vector<String> upv_dirs;
	if(!upv_loaded) {
		upv_dirs.Clear();
		FindFile ff(GetUpvDir() + "/*.*");
		while(ff) {
			if(ff.IsFolder())
				upv_dirs.Add(ff.GetPath());
			ff.Next();
		}
		upv_loaded = true;
	}
	s.Append(upv_dirs);
	return s;
}

void Nest::InvalidatePackageCache()
{
	package_cache.Clear();
	upv_loaded = false;
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
