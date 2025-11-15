#include "Core.h"

void InvalidatePackageCache()
{
	MainNest().InvalidatePackageCache();
}

String PackageDirectory(const String& name)
{
	return MainNest().PackageDirectory(name);
}

String GetPathNest(const String& path)
{
	if(IsExternalMode())
		return Null;
	String h = UnixPath(NormalizePath(path));
	for(auto dir : GetUppDirs())
		if(h.StartsWith(UnixPath(NormalizePath(dir)) + '/'))
			return dir;
	return Null;
}

String SourcePath(const String& package, const String& file) {
	if(IsFullPath(file)) return NativePath(file);
	return NormalizePath(AppendFileName(PackageDirectory(package), file));
}

bool IsDirectoryExternalPackage(const String& dir)
{
	String source_masks = GetVar("SOURCE_MASKS");
	for(FindFile ff(dir + "/*.*"); ff; ff.Next())
		if(ff.IsFile() && PatternMatchMulti(source_masks, ff.GetName()))
			return true;
	return false;
}

bool IsDirectoryPackage(const String& path)
{
	if(IsExternalMode())
		return DirectoryExists(path);
	return FileExists(AppendFileName(path, GetFileTitle(path) + ".upp"));
}

String EncodePathAsFileName(const String& path)
{
	String p = UnixPath(path);
	p.Replace("/", "[sLash]");
	p.Replace(":", "[cOlon]");
	return p;
}

String DecodePathFromFileName(const String& n)
{
	String p = n;
	p.Replace("[sLash]", "/");
	p.Replace("[cOlon]", ":");
	return p;
}

String PackageFile(const String& package)
{ // in external mode we are storing packages into .config
	if(IsExternalMode()) {
		String p = ConfigFile("external") + "/" + EncodePathAsFileName(package);
		RealizePath(p);
		return p;
	}
	return AppendFileName(PackageDirectory(package), GetFileTitle(package) + ".upp");
}

bool IsNestReadOnly(const String& path)
{
	Vector<String> d = GetUppDirs();
	for(int i = 0; i < d.GetCount(); i++)
		if(path.StartsWith(d[i]) && FileExists(AppendFileName(d[i], "readonly")))
			return true;
	return false;
}

void SplitHostName(const char *hostname, String& host, int& port) {
	enum { DEFAULT_REMOTE_PORT = 2346 };
	const char *p = hostname;
	while(p && *p != ':')
		p++;
	host = String(hostname, p);
	if(*p++ == ':' && IsDigit(*p))
		port = stou(p);
	else
		port = DEFAULT_REMOTE_PORT;
}

Vector<String> SplitFlags0(const char *flags) {
	return Split(flags, CharFilterWhitespace);
}

Vector<String> SplitFlags(const char *flags, bool main, const Vector<String>& accepts)
{
	Vector<String> v = SplitFlags0(flags);
	Vector<String> h;
	for(int i = 0; i < v.GetCount(); i++)
		if(v[i][0] == '.') {
			String f = v[i].Mid(1);
			if(main || FindIndex(accepts, f) >= 0)
				h.Add(v[i].Mid(1));
		}
		else
			h.Add(v[i]);
	if(main)
		h.Add("MAIN");
	return h;
}

Vector<String> SplitFlags(const char *flags, bool main)
{
	Vector<String> dummy;
	return SplitFlags(flags, main, dummy);
}

bool HasFlag(const Vector<String>& conf, const char *flag) {
	return FindIndex(conf, flag) >= 0;
}

Vector<String> Combine(const Vector<String>& conf, const char *flags) {
	Vector<String> cfg(conf, 1);
	try {
		CParser p(flags);
		while(!p.IsEof()) {
			bool isnot = p.Char('!');
			if(!p.IsId()) break;
			String flg = p.ReadId();
			int i = FindIndex(cfg, flg);
			if(isnot) {
				if(i >= 0) cfg.Remove(i);
			}
			else
				if(i < 0) cfg.Add(flg);
		}
	}
	catch(CParser::Error) {}
	return cfg;
}

String Gather(const Array<OptItem>& set, const Vector<String>& conf, bool nospace) {
	String s;
	for(int i = 0; i < set.GetCount(); i++)
		if(MatchWhen(set[i].when, conf)) {
			if(!nospace && !s.IsEmpty()) s.Cat(' ');
			s.Cat(set[i].text);
		}
	return s;
}

Vector<String> GatherV(const Array<OptItem>& set, const Vector<String>& conf) {
	Vector<String> s;
	for(int i = 0; i < set.GetCount(); i++)
		if(MatchWhen(set[i].when, conf))
			s << set[i].text;
	return s;
}

int    GetType(const Vector<String>& conf, const char *flags) {
	Vector<String> f = SplitFlags(flags);
	int q = FLAG_UNDEFINED;
	for(int i = 0; i < f.GetCount(); i++)
		if(HasFlag(conf, f[i])) {
			if(q == FLAG_UNDEFINED)
				q = i;
			else
				q = FLAG_MISMATCH;
		}
	return q;
}

String RemoveType(Vector<String>& conf, const char *flags)
{
	String old;
	Index<String> f(SplitFlags(flags));
	for(int i = conf.GetCount(); --i >= 0;)
		if(f.Find(conf[i]) >= 0)
		{
			old = conf[i];
			conf.Remove(i);
		}
	return old;
}

int    GetType(const Vector<String>& conf, const char *flags, int def) {
	int q = GetType(conf, flags);
	return q == FLAG_UNDEFINED ? def : q;
}

bool   GetFlag(const Vector<String>& conf, const char *flag) {
	return FindIndex(conf, flag) >= 0;
}

void Workspace::AddLoad(const String& name)
{
	package.Add(name).Load(PackageFile(name));
}

void Workspace::AddUses(Package& p, const Vector<String> *flag)
{
	int q = package.GetCount();
	for(int i = 0; i < p.uses.GetCount(); i++) {
		String uses = UnixPath(p.uses[i].text);
		if((!flag || MatchWhen(p.uses[i].when, *flag)) && package.Find(uses) < 0)
			AddLoad(uses);
	}
	int n = package.GetCount();
	for(int i = q; i < package.GetCount(); i++)
		AddUses(package[i], flag);
	for(int i = q; i < n; i++)
		use_order.Add(i);
}

void Workspace::Scan(const char *prjname, const Vector<String> *flag) {
	package.Clear();
	use_order.Clear();
	AddLoad(prjname);
	AddUses(package[0], flag);
	use_order.Add(0);
}

void Workspace::Dump() {
	for(int i = 0; i < package.GetCount(); i++) {
		Package& prj = package[i];
		LOG("Package " << package.GetKey(i));
		LOG(" file ");
		for(int i = 0; i < prj.file.GetCount(); i++)
			LOG("\t" << prj.file[i]);
	}
}

Vector<String> Workspace::GetAllAccepts(int pk) const
{
	Index<String> accepts;
	Index<int> pkg;
	pkg.Add(pk);
	for(int i = 0; i < pkg.GetCount(); i++) {
		const Package& p = package[pkg[i]];
		FindAppend(accepts, p.accepts);
		for(int u = 0; u < p.uses.GetCount(); u++) {
			int f = package.Find(UnixPath(p.uses[u].text));
			if(f >= 0)
				pkg.FindAdd(f);
		}
	}
	return accepts.PickKeys();
}