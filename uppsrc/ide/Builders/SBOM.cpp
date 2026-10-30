#include "Builders.h"

String Format8601Z(Time t)
{
	return Format("%04.4d-%02.2d-%02.2d`T%02.2d`:%02.2d`:%02.2d`Z",
				t.year, t.month, t.day, t.hour, t.minute, t.second);
}

static bool SBOMIsSpdxLike(const String& s)
{
	if(s.IsEmpty() || s.GetCount() > 64)
		return false;
	bool word = false;
	for(int i = 0; i < s.GetLength(); i++) {
		int c = s[i];
		if(IsAlNum(c) || c == '.' || c == '-' || c == '+')
			word = true;
		else
		if(c != ' ')
			return false;
	}
	return word;
}

// ============================================================================
// SBOMContext Class — All utility functions and caches as members.
// Sections per family: neutral/ELF, dpkg, RPM, git, vcpkg, windows toolchain.
// BREW later: one new section + dispatch hooks at the IsDPKG()/IsRPM()
// branch points (// BREW: later drop-in).
// ============================================================================

class SBOMContext {
public:
	SBOMContext(MakeBuild& mk_, const String& triplet_, const Workspace& wspc_,
	            Host& host_, Builder& builder_)
		: mk(mk_), triplet(triplet_), wspc(wspc_), host(host_), builder(builder_) {
		bm = GetMethodVars(mk.method);
		cc = builder.CompilerName();

		// Tier tree — once, against package 0's config (one PackageConfig
		// call feeding both link_mode and win_target, as before)
		Index<String> main_cfg = mk.PackageConfig(wspc, 0, bm, mk.mainconfigparam, host, builder);
		link_mode = LinkMode(main_cfg);
#ifdef PLATFORM_WIN32
		win_target = true;
#else
		// Cross builds on Linux for Windows are controlled by win_target
		win_target = main_cfg.Find("WIN32") >= 0;
#endif
		dep_keys = TargetDependencyKeys(win_target);

		msvc = bm.Get("BUILDER", "GCC").StartsWith("MSC");
		clang = bm.Get("BUILDER", "GCC") == "CLANG";
		md   = mk.mainconfigparam.Find("MD") >= 0;

#ifdef PLATFORM_WIN32
		git = GetExeDirFile("bin/mingit/cmd/git.exe");
		if(!FileExists(git))
			git = "git";  // Fallback if mingit not present
#else
		git = "git";
#endif
	}
	
	// ------------------------------------------------------------------------
	// Public orchestration: CreateSBOMComponents
	// ------------------------------------------------------------------------
	Array<SBOMComponent> CreateSBOMComponents(Gate<int, int>& progress);
	
protected:
	// ------------------------------------------------------------------------
	// Enums & value types
	// ------------------------------------------------------------------------
	
	enum {
		LINK_STATIC,
		LINK_DYNAMIC
	};
	
	enum {
		DECL_EXTERNAL_DEPENDENCY,
		DECL_STATIC_LIBRARY,
		DECL_DYNAMIC_LIBRARY,
		DECL_LIBRARY,
	};
	
	struct Decl : Moveable<Decl> {
		int decl = 0;
		String name;
		int pk = -1;
		Decl(int d, const String& n, int p) : decl(d), name(n), pk(p) {}
	};
	
	struct PkgMeta : Moveable<PkgMeta> {
		String version;
		String homepage;
		String arch;
		String source;
	};
	
	// ------------------------------------------------------------------------
	// NEUTRAL / ELF HELPER FUNCTIONS (pm-agnostic)
	// ------------------------------------------------------------------------
	
	// ldconfig cache: one Sys() call, parsed once in C++
	Vector<Tuple<String, String>>& LdconfigSoDB();
	String FindSoPath(const String& base);
	String FindSonameFile(const String& soname);
	
	// Batch readelf -d: extract NEEDED from multiple .so files
	Vector<Vector<String>> BatchReadelfNeeded(const Vector<String>& files);
	
	// Single-command ownership probes (fallback when DB miss)
	String PkgNameOnly(String s);
	String RealFile(const String& path);
	String CanonDir(const String& s);
	
	VectorMap<String, String>& FileOwnerDB();
	VectorMap<String, String>& SonameOwnerDB();
	String OwnerOfFileFast(const String& file);
	String OwnerOfFile(const String& file);
	String DynOwnerFile(const String& base);
	String StaticOwner(const String& base);
	
	// ------------------------------------------------------------------------
	// DPKG FAMILY FUNCTIONS
	// ------------------------------------------------------------------------
	
	void DpkgBuildFileOwnerDB();
	void DpkgBuildShlibsOwnerDB();
	
	// Memoized per-package dpkg-query -W (monotonic growth, no eviction flush)
	PkgMeta DpkgQueryPackageMeta(const String& pkg);
	
	String OsReleaseField(const char *key);
	String DebNamespace();
	String DebDistro();
	
	void DpkgFillPkgMeta(SBOMComponent& m);
	
	// ------------------------------------------------------------------------
	// RPM FAMILY FUNCTIONS
	// ------------------------------------------------------------------------
	
	void RpmBuildFileOwnerDB();
	void RpmBuildSonameOwnerDB();
	String RpmNamespace();
	String RpmDistro();
	void RpmFillPkgMeta(SBOMComponent& m);
	
	// ------------------------------------------------------------------------
	// GIT FAMILY FUNCTIONS
	// ------------------------------------------------------------------------
	
	Tuple<String, String, String> GitResolveCommit(const String& repo_root);
	String GitRepoTopLevel(const String& dir);
	String GitOriginOf(const String& dir);
	String GitRepoPrefix(String origin);
	String PkgName(const String& s);
	
	// ------------------------------------------------------------------------
	// VCPKG FAMILY FUNCTIONS
	// ------------------------------------------------------------------------
	
	bool IsVcpkgPort(const String& a);
	void VcpkgFillFromInstalledList(SBOMComponent& m, const String& name);
	
	// ------------------------------------------------------------------------
	// WINDOWS TOOLCHAIN FAMILY
	// ------------------------------------------------------------------------
	
	String MingwCrtVersion();
	
	// ------------------------------------------------------------------------
	// NEUTRAL TIERING / DISPATCH HELPERS
	// ------------------------------------------------------------------------
	
	int LinkMode(const Index<String>& cfg);
	int DynTierOf(const String& dyn_owner);
	int VcpkgTier(int lm);
	int DeclTier(int decl, const String& name, int lm, int dyn_tier);
	Vector<String> TargetDependencyKeys(bool wt);
	void FillPkgMeta(SBOMComponent& m);
	
	// ------------------------------------------------------------------------
	// Environment (fixed per invocation)
	// ------------------------------------------------------------------------
	
	MakeBuild&       mk;              // owns method vars and PackageConfig
	const String&    triplet;
	const Workspace& wspc;
	Host&            host;
	Builder&         builder;
	VectorMap<String, String> bm;     // GetMethodVars(mk.method)
	String           cc;
	bool             win_target = false;
	int              link_mode = 0;
	Vector<String>   dep_keys;
	bool             msvc = false;
	bool             clang = false;
	bool             md = false;
	String           git;
	
	// ------------------------------------------------------------------------
	// CACHE MEMBERS (instance-scoped: fresh per invocation, isolated across
	// concurrent CreateSBOMComponents calls). No locking — builds are
	// serialized on one driving thread.
	// ------------------------------------------------------------------------
	
	// Neutral/ELF caches
	Vector<Tuple<String, String>> so_db;
	bool so_db_loaded = false;
	
	// Ownership databases
	VectorMap<String, String> file_owner_db;
	bool file_owner_db_loaded = false;
	VectorMap<String, String> soname_owner_db;
	bool soname_owner_db_loaded = false;
	
	// dpkg-query memo
	Index<String>   dpkg_query_batch_cache;
	Vector<PkgMeta> dpkg_query_batch_results;
	
	// os-release (deb side, lowercased once)
	String os_release_cache;
	bool os_release_loaded = false;
	
	// RPM ns/distro, computed once
	String rpm_namespace;
	String rpm_distro;
	bool rpm_ns_loaded = false;
	
	// Git caches (repo_origin: repo top-levels only, positive entries only —
	// negatives deliberately never cached, a subdir may itself be a repo)
	VectorMap<String, Tuple<String, String, String>> git_commit_cache;
	VectorMap<String, String> repo_origin;
	VectorMap<String, String> repo_prefix;
	
	// ------------------------------------------------------------------------
	// Per-invocation bookkeeping for CreateSBOMComponents
	// ------------------------------------------------------------------------
	
	Index<String>             deps_done;
	Index<String>             seen_ext;
	VectorMap<String, int>    ci_ext;
	Vector<Decl>              decls;
	Vector<int>               own_ci;
	VectorMap<String, String> override_licenses;
	Index<String>             all_host;
	
	Index<String>               candidate_files;
	Vector<String>              resolved_file_to_base;
	VectorMap<String, String>   file_owner_memo;
	
	int progress_done = 0;
	int progress_total = 0;
	Gate<int, int>* progress_gate = nullptr;
};

// ============================================================================
// NEUTRAL / ELF HELPER IMPLEMENTATION
// ============================================================================

Vector<Tuple<String, String>>& SBOMContext::LdconfigSoDB()
{
	if(!so_db_loaded) {
		so_db_loaded = true;
		for(String l : Split(Sys("ldconfig -p"), '\n')) {
			int q = l.Find("=>");
			if(q < 0)
				continue;
			String soname = TrimBoth(l.Left(q));
			int sp = soname.Find(' ');              // strip "(libc6,...)" tag
			if(sp >= 0)
				soname.Trim(sp);
			if(soname.GetCount())
				so_db.Add(MakeTuple(soname, TrimBoth(l.Mid(q + 2))));
		}
	}
	return so_db;
}

String SBOMContext::FindSoPath(const String& base)
{
	String pref = "lib" + base + ".so";
	String best;
	for(const auto& e : LdconfigSoDB()) {
		if(!e.a.StartsWith(pref))
			continue;
		if(e.a.GetCount() == pref.GetCount())       // exact libX.so — strongest
			return e.b;
		bool tail_digits = true;                    // else libX.so.N
		for(const char *c = ~e.a + pref.GetCount() + 1; *c; c++)
			if(*c != '.' && !IsDigit(*c)) { tail_digits = false; break; }
		if(tail_digits && best.IsEmpty())
			best = e.b;
	}
	return best;
}

String SBOMContext::FindSonameFile(const String& soname)
{
	for(const auto& e : LdconfigSoDB())
		if(e.a == soname)
			return e.b;
	String pref = soname + ".";                    // tolerate minor drift
	for(const auto& e : LdconfigSoDB())
		if(e.a.StartsWith(pref))
			return e.b;
	return String();
}

Vector<Vector<String>> SBOMContext::BatchReadelfNeeded(const Vector<String>& files)
{
	Vector<Vector<String>> results;
	results.SetCount(files.GetCount());

	Vector<int> qidx;                       // results-index of each queried file
	String args;
	for(int i = 0; i < files.GetCount(); i++) {
		if(files[i].IsEmpty()) continue;
		qidx.Add(i);
		args << (args.GetCount() ? " " : "") << files[i];
	}
	if(qidx.IsEmpty())
		return results;

	bool one = qidx.GetCount() == 1;        // single file: readelf omits the header
	int cur = one ? 0 : -1;                 // slot into qidx
	for(String l : Split(Sys("readelf -d " + args), '\n')) {
		if(!one && l.StartsWith("File:")) { // "File: /usr/lib/.../libz.so.1"
			String f = TrimBoth(l.Mid(5));
			cur = -1;
			for(int k = 0; k < qidx.GetCount(); k++)
				if(files[qidx[k]] == f) { cur = k; break; }
			continue;
		}
		if(cur < 0) continue;
		int q1 = l.Find("(NEEDED)");
		if(q1 < 0) continue;
		q1 = l.Find('[', q1);
		int q2 = q1 >= 0 ? l.Find(']', q1) : -1;
		if(q1 >= 0 && q2 > q1)
			results[qidx[cur]].Add(l.Mid(q1 + 1, q2 - q1 - 1));
	}
	return results;
}

String SBOMContext::PkgNameOnly(String s)
{
	int nl = s.Find('\n');
	if(nl >= 0)
		s = s.Left(nl);
	s = TrimBoth(s);
	int q = s.Find(':');                            // "libz1:amd64: /path"
	return q >= 0 ? TrimBoth(s.Mid(0, q)) : String();
}

String SBOMContext::RealFile(const String& path)
{
	if(!path.GetCount())
		return String();
	return TrimBoth(HostSys("readlink -f " + path)); // no shell for simple paths
}

String SBOMContext::CanonDir(const String& s)
{
#ifdef PLATFORM_WIN32
	return ToLower(s);   // NTFS paths are case-insensitive; Map find is not
#else
	return s;
#endif
}

VectorMap<String, String>& SBOMContext::FileOwnerDB()
{
	if(!file_owner_db_loaded) {
		file_owner_db_loaded = true;
#ifdef PLATFORM_POSIX
		if(IsDPKG())
			DpkgBuildFileOwnerDB();
		else if(IsRPM())
			RpmBuildFileOwnerDB();
#endif
	}
	return file_owner_db;
}

VectorMap<String, String>& SBOMContext::SonameOwnerDB()
{
	if(!soname_owner_db_loaded) {
		soname_owner_db_loaded = true;
#ifdef PLATFORM_POSIX
		if(IsRPM())
			RpmBuildSonameOwnerDB();
		else if(IsDPKG())
			DpkgBuildShlibsOwnerDB();
#endif
	}
	return soname_owner_db;
}

String SBOMContext::OwnerOfFileFast(const String& file)
{
	if(!file.GetCount())
		return String();
	return FileOwnerDB().Get(file, String());
}

String SBOMContext::OwnerOfFile(const String& file)
{
	if(!file.GetCount())
		return String();
	if(IsRPM()) {
		String r = TrimBoth(Sys("rpm -qf --qf %{NAME} " + file));
		return r.Find("not owned") >= 0 || r.StartsWith("file ") ? String() : r;
	}
	if(IsDPKG())
		return PkgNameOnly(Sys("dpkg -S " + file));
	return String();                                // BREW: later drop-in
}

#ifdef PLATFORM_POSIX
String SBOMContext::DynOwnerFile(const String& base)
{
	return RealFile(FindSoPath(base));
}
#endif

String SBOMContext::StaticOwner(const String& base)
{
#ifdef PLATFORM_POSIX
	String want = "lib" + base + ".a";
	String p = TrimBoth(HostSys(cc + " -print-file-name=" + want));
	return p.GetCount() && p != want && GetFileName(p) == want ? OwnerOfFileFast(RealFile(p)) : String();
#else
	return base; // never reached on win_target, keeps it callable
#endif
}

// ============================================================================
// DPKG FAMILY IMPLEMENTATION
// ============================================================================

// Pure file I/O from /var/lib/dpkg/info/<pkg>[:arch].list files. Registers
// both spellings (/usr/x and /x) at load — merged-usr alias.
void SBOMContext::DpkgBuildFileOwnerDB()
{
	FindFile ff("/var/lib/dpkg/info/*.list");
	while(ff) {
		String pkg = GetFileTitle(ff.GetName());
		int c = pkg.Find(':');
		if(c >= 0) pkg.Trim(c);              // strip ":amd64"
		for(String l : Split(LoadFile(ff.GetPath()), '\n')) {
			l = TrimBoth(l);
			if(l.GetCount()) {
				file_owner_db.GetAdd(l) = pkg;                        // first owner wins
				if(l.StartsWith("/usr/"))
					file_owner_db.GetAdd("/" + l.Mid(5), pkg);      // merged-usr alias
			}
		}
		ff.Next();
	}
}

// Debian-native soname index: /var/lib/dpkg/info/*.shlibs
// Format (Policy 8.6.4.2): [type:] libname sover deps...
//   "libgcc_s 1 libgcc-s1 (>= 3.3)" -> libgcc_s.so.1 -> libgcc-s1
void SBOMContext::DpkgBuildShlibsOwnerDB()
{
	const char *INFO = "/var/lib/dpkg/info/";
	FindFile ff(String(INFO) + "*.shlibs");
	while(ff) {
		String body = LoadFile(String(INFO) + ff.GetName());
		for(String l : Split(body, '\n')) {
			l = TrimBoth(l);
			if(!l.GetCount() || l[0] == '#')
				continue;
			Vector<String> w = Split(l, ' ');
			int f = w.GetCount() && w[0].EndsWith(":") ? 1 : 0;
			if(w.GetCount() < f + 3)
				continue;
			String pkg = w[f + 2];
			int q = pkg.Find('(');
			if(q >= 0)
				pkg = pkg.Left(q);
			if(pkg.GetCount())
				soname_owner_db.GetAdd(w[f] + ".so." + w[f + 1], pkg);
		}
		ff.Next();
	}
}

SBOMContext::PkgMeta SBOMContext::DpkgQueryPackageMeta(const String& pkg)
{
	int cached = dpkg_query_batch_cache.Find(pkg);
	if(cached >= 0)
		return dpkg_query_batch_results[cached];
	
	String out = Sys("dpkg-query -W -f='${Version}\n${Homepage}\n${Architecture}\n${Source}\n' " + pkg);
	Vector<String> rec = Split(out, '\n', false);
	PkgMeta pm;
	pm.version = rec.GetCount() > 0 ? TrimBoth(rec[0]) : String();
	pm.homepage = rec.GetCount() > 1 ? TrimBoth(rec[1]) : String();
	pm.arch = rec.GetCount() > 2 ? TrimBoth(rec[2]) : String();
	pm.source = rec.GetCount() > 3 ? TrimBoth(rec[3]) : String();
	
	int pq = pm.source.Find(" (");
	if(pq >= 0) pm.source.Trim(pq);
	if(!pm.source.GetCount()) pm.source = pkg;
	
	dpkg_query_batch_cache.Add(pkg);
	dpkg_query_batch_results.Add(pm);
	return dpkg_query_batch_results[dpkg_query_batch_results.GetCount()-1];
}

String SBOMContext::OsReleaseField(const char *key)
{
	if(!os_release_loaded) {
		os_release_loaded = true;
		os_release_cache = ToLower(LoadFile("/etc/os-release"));
	}
	String prefix = ToLower(String(key)) + "=";
	String v;
	for(String l : Split(os_release_cache, '\n')) {
		l = TrimBoth(l);
		if(l.StartsWith(prefix)) {
			v = TrimBoth(l.Mid(prefix.GetCount()));
			break;
		}
	}
	v.TrimStart("\"");
	v.TrimEnd("\"");
	return v;
}

String SBOMContext::DebNamespace()
{
	String id = OsReleaseField("ID");
	String id_like = OsReleaseField("ID_LIKE");
	String ubuntu_codename = OsReleaseField("UBUNTU_CODENAME");

	if(id == "ubuntu" || id_like.Find("ubuntu") >= 0)
		return "ubuntu";
	if(id == "debian" || id_like.Find("debian") >= 0)
		return "debian";
	if(ubuntu_codename.GetCount())
		return "ubuntu";
	return "debian";
}

String SBOMContext::DebDistro()
{
	String ns = DebNamespace();
	String v = (ns == "ubuntu") ? OsReleaseField("UBUNTU_CODENAME")
								: OsReleaseField("VERSION_CODENAME");

	bool ok = v.GetCount() > 0;
	for(int i = 0; i < v.GetCount(); i++) {
		char c = v[i];
		if(c < 'a' || c > 'z')
			ok = false;
	}
	return ok ? v : String();
}

void SBOMContext::DpkgFillPkgMeta(SBOMComponent& m)
{
	const PkgMeta& pm = DpkgQueryPackageMeta(m.name);
	if(!pm.version.GetCount()) {
		m.homepage = String();
		return;
	}
	m.version = pm.version;
	m.homepage = pm.homepage;
	
	m.purl << "pkg:deb/" << DebNamespace() << '/' << m.name << '@' << pm.version;
	String quals;
	if(pm.arch.GetCount())
		quals << "?arch=" << pm.arch;
	
	String distro = DebDistro();
	if(distro.GetCount())
		quals << (quals.GetCount() ? '&' : '?') << "distro=" << distro;
		
	quals << (quals.GetCount() ? '&' : '?') << "upstream=" << pm.source;
	m.purl << quals;

	String cr = LoadFile("/usr/share/doc/" + m.name + "/copyright");
	for(String l : Split(cr, '\n')) {
		l = TrimBoth(l);
		if(!l.StartsWith("License:"))
			continue;
		String v = TrimBoth(l.Mid(8));
		if(SBOMIsSpdxLike(v)) {
			m.licenses.Clear();
			m.licenses << v;
			break;
		}
	}
}

// ============================================================================
// RPM FAMILY IMPLEMENTATION
// ============================================================================

// One line per package: NAME \t file \t file ... \t
void SBOMContext::RpmBuildFileOwnerDB()
{
	String out = Sys("rpm -qa --qf '%{NAME}\t[%{FILENAMES}\t]\n'");
	for(String l : Split(out, '\n')) {
		Vector<String> v = Split(l, '\t');          // empty tokens dropped by default
		if(v.GetCount() < 2)
			continue;
		String pkg = TrimBoth(v[0]);
		if(!pkg.GetCount() || pkg == "(none)")
			continue;
		for(int i = 1; i < v.GetCount(); i++)
			if(v[i].GetCount())
				file_owner_db.GetAdd(TrimBoth(v[i]), pkg);
	}
}

// One line per package: NAME \t provide \t provide ...
// Shared-library provides look like "libgcc_s.so.1()(64bit)".
void SBOMContext::RpmBuildSonameOwnerDB()
{
	String s = Sys("rpm -qa --qf '%{NAME}\t[%{PROVIDENAME}\t]\n'");
	for(String l : Split(s, '\n')) {
		Vector<String> v = Split(l, '\t');
		if(v.GetCount() < 2)
			continue;
		String pkg = TrimBoth(v[0]);
		if(!pkg.GetCount() || pkg == "(none)")
			continue;
		for(int i = 1; i < v.GetCount(); i++) {
			String t = TrimBoth(v[i]);
			int q = t.Find('(');
			if(q >= 0)
				t = t.Left(q);
			t = TrimBoth(t);
			if(t.Find(".so") < 0 || t.Find(' ') >= 0)
				continue;
			soname_owner_db.GetAdd(t, pkg);
		}
	}
}

String SBOMContext::RpmNamespace()
{
	if(!rpm_ns_loaded) {
		rpm_ns_loaded = true;
		String id, vid;
		for(String l : Split(LoadFile("/etc/os-release"), '\n')) {
			l = TrimBoth(l);
			if(l.StartsWith("ID="))
				id = TrimBoth(l.Mid(3));
			else if(l.StartsWith("VERSION_ID="))
				vid = TrimBoth(l.Mid(11));
		}
		id.TrimStart("\"");  id.TrimEnd("\"");
		vid.TrimStart("\""); vid.TrimEnd("\"");
		Vector<String> v = Split(vid, '.');
		if(id == "sles" || id == "sle" || id == "sled") {
			rpm_namespace = "suse";
			rpm_distro = v.GetCount() > 1 ? "sle" + v[0] + "-sp" + v[1]
				: v.GetCount()    ? "sle" + v[0]
					: "sles";
		}
		else
			rpm_namespace = id;
	}
	return rpm_namespace;
}

String SBOMContext::RpmDistro()
{
	RpmNamespace();
	return rpm_distro;
}

void SBOMContext::RpmFillPkgMeta(SBOMComponent& m)
{
	String url, arch, lic, srcrpm, epoch, out = Sys(
		"rpm -q --qf '%{EPOCH}\n%{VERSION}-%{RELEASE}\n%{URL}\n%{ARCH}\n%{LICENSE}\n%{SOURCERPM}\n' "
		+ m.name);
	String vrel;
	SplitTo(out, '\n', false, epoch, vrel, url, arch, lic, srcrpm);
	epoch = TrimBoth(epoch);
	vrel  = TrimBoth(vrel);
	if(!vrel.GetCount()) {                 // rpm -q found nothing → no metadata
		m.homepage = String();
		return;
	}
	m.version  = vrel;
	m.homepage = url.GetCount() ? TrimBoth(url) : String();
	lic = TrimBoth(lic);
	if(lic.GetCount() && lic != "(none)") {
		m.licenses.Clear();
		m.licenses << lic;
	}

	m.purl << "pkg:rpm/" << RpmNamespace() << '/' << m.name << '@' << m.version;

	String quals;
	auto AddQual = [&](const char *key, String val) {
		val = TrimBoth(val);
		if(!val.GetCount() || val == "(none)")
			return;
		quals << (quals.GetCount() ? '&' : '?') << key << '=' << val;
	};
	AddQual("arch", arch);
	AddQual("distro", RpmDistro());
	if(epoch.GetCount() && epoch != "0")
		AddQual("epoch", epoch);
	AddQual("upstream", srcrpm);
	m.purl << quals;
}

// ============================================================================
// GIT FAMILY IMPLEMENTATION
// ============================================================================

// Cache keyed by canonical root; quoted git -C "<root>" (quoting fixed a
// space-in-path bug — preserve). Failure → all-empty tuple.
Tuple<String, String, String> SBOMContext::GitResolveCommit(const String& repo_root)
{
	if(!repo_root.GetCount())
		return MakeTuple(String(), String(), String());
	int q = git_commit_cache.Find(repo_root);
	if(q >= 0)
		return git_commit_cache[q];

	String hash, sha1, ts;
	String log_out = TrimBoth(HostSys(git + " -C \"" + repo_root
									  + "\" log -1 --date=unix --format=\"%h %H %cd\""));
	if(!SplitTo(log_out, ' ', hash, sha1, ts)) {
		hash = ""; sha1 = ""; ts = "";
	}
	Tuple<String, String, String> entry = MakeTuple(hash, sha1, ts);
	git_commit_cache.Add(repo_root, entry);
	return entry;
}

// Walk up directories over known repo tops (positive-only caching in
// repo_origin; negatives deliberately never cached — a subdir may itself be
// a repo), then one rev-parse --show-toplevel + one remote.origin.url per repo.
String SBOMContext::GitRepoTopLevel(const String& dir)
{
	String d = CanonDir(dir);
	for(;;) {
		if(repo_origin.Find(d) >= 0)
			return d;                      // d is a known repo top
		String parent = GetFileFolder(d);
		if(parent.IsEmpty() || parent == d)
			break;
		d = parent;
	}
	String root = TrimBoth(Sys(git + " -C \"" + dir + "\" rev-parse --show-toplevel"));
	if(!root.GetCount())
		return String();                  // outside any repo; not cached
	root = CanonDir(root);
	if(repo_origin.Find(root) < 0) {       // first sighting of this repo
		String origin = TrimBoth(Sys(git + " -C \"" + root + "\" config --get remote.origin.url"));
		if(origin.StartsWith("git@github.com:"))
			origin.Replace("git@github.com:", "https://github.com/");
		repo_origin.Add(root, origin);
	}
	return root;
}

String SBOMContext::GitOriginOf(const String& dir)
{
	String root = GitRepoTopLevel(dir);
	return root.GetCount() ? repo_origin.Get(root) : String();
}

String SBOMContext::GitRepoPrefix(String origin)
{
	if(!origin.GetCount())
		return String("./");
	while(origin.EndsWith(".git")) origin.TrimEnd(".git");
	while(origin.EndsWith("/"))   origin.TrimEnd("/");
	int q = max(origin.ReverseFind('/'), origin.ReverseFind(':'));
	return origin.Mid(q + 1) + "/";
}

// Folds case on Windows for the memo key but returns original-case names.
String SBOMContext::PkgName(const String& s)
{
	String n = Filter(s, [](int c) { return c == '\\' ? '/' : c; });
	String key = n;
#ifdef PLATFORM_WIN32
	key = ToLower(key);          // fold for repo_prefix / wspc comparison ONLY
#endif
	int q = repo_prefix.Find(key);
	if(q < 0) {
		bool in_ws = false;
		for(int wi = 0; wi < wspc.GetCount(); wi++)
			if(Filter(wspc[wi], [](int c){ return c=='\\'?'/':c; }) == key
#ifdef PLATFORM_WIN32
			|| ToLower(Filter(wspc[wi], [](int c){ return c=='\\'?'/':c; })) == key
#endif
			) { in_ws = true; break; }
		String prefix = in_ws ? GitRepoPrefix(GitOriginOf(PackageDirectory(n))) : String("./");
		q = repo_prefix.GetCount();
		repo_prefix.Add(key, prefix);
	}
	return repo_prefix[q] + n;   // <- original-case n, not the folded key
}

// ============================================================================
// VCPKG FAMILY IMPLEMENTATION
// ============================================================================

bool SBOMContext::IsVcpkgPort(const String& a)
{
	for(const VcpkgInstalled& it : VcpkgList())
		if(it.name == a && it.triplets.Find(triplet) >= 0)
			return true;
	return false;
}

void SBOMContext::VcpkgFillFromInstalledList(SBOMComponent& m, const String& name)
{
	for(const VcpkgInstalled& it : VcpkgList())
		if(it.name == name && it.triplets.Find(triplet) >= 0) {
			m.version = it.version;
			return;
		}
}

// ============================================================================
// WINDOWS TOOLCHAIN IMPLEMENTATION
// ============================================================================

// Splits PATH on ';' only — works for the U++ build-method PATH
// representation; a colon-PATH host would silently lose the CRT version.
// Accepted-fragile: retain as-is.
String SBOMContext::MingwCrtVersion()
{
	String maj, min, fix;
	Vector<String> path = Split(bm.Get("PATH", ""), ';');
	for(String p : path){
		String s = LoadFile(GetFileFolder(p) + "/include/_mingw_mac.h");
		if(!s.IsEmpty()){
			auto M = [&](const char *n) {
				int m = s.FindAfter(n);
				if(m >= 0){
					String ss = s.Mid(m, 50);
					ss.Replace("\t"," ");
					Vector<String> v = Split(ss, '\n', true);
					if(v.GetCount()) return TrimBoth(v[0]);
				}
				return String();
			};
			if((maj = M("__MINGW64_VERSION_MAJOR")).GetCount()) {
				if((min = M("__MINGW64_VERSION_MINOR")).GetCount())
					if((fix = M("__MINGW64_VERSION_BUGFIX")).GetCount())
						min << '.' << fix;
				return maj + '.' + min;
			}
		}
	}
	return String();
}

// ============================================================================
// NEUTRAL TIERING / DISPATCH HELPERS
// ============================================================================

int SBOMContext::LinkMode(const Index<String>& cfg)
{
	return cfg.Find("PORTABLE_HYBRID") >= 0
		|| (cfg.Find("SHARED") < 0 && cfg.Find("SO") < 0) ? LINK_STATIC : LINK_DYNAMIC;
}

int SBOMContext::DynTierOf(const String& dyn_owner)
{
	return dyn_owner.GetCount() ? T_DYNAMIC_PLATFORM : T_DYNAMIC_BUNDLED;
}

int SBOMContext::VcpkgTier(int lm)
{
	return lm == LINK_STATIC ? T_STATIC_THIRD_PARTY : T_DYNAMIC_BUNDLED;
}

int SBOMContext::DeclTier(int decl, const String& name, int lm, int dyn_tier)
{
	String ext = ToLower(GetFileExt(name));
	if(ext == ".a" || ext == ".lib")
		return T_STATIC_THIRD_PARTY;	// explicit static file
	if(ext == ".so" || ext == ".dll")
		return dyn_tier;				// explicit dynamic file
	switch(decl) {
	case DECL_STATIC_LIBRARY:
	case DECL_EXTERNAL_DEPENDENCY:
		return T_STATIC_THIRD_PARTY;	// always
	case DECL_DYNAMIC_LIBRARY:
		return dyn_tier;				// forced dynamic: provider rule even in
										// static builds, never T_STATIC_THIRD_PARTY
	default:
		return lm == LINK_STATIC ? T_STATIC_THIRD_PARTY : dyn_tier;
	}
}

// Target-aware key set for .upp external_dependency selection.
// GetExternalDependenciesKeys() describes the HOST (manager id + platform
// #defines), so a Linux->Windows cross build selects the Debian-gated
// entries. A WIN32 target must select exactly what a native Windows build
// selects: manager "VCPKG" plus the "WIN32" platform tag.
Vector<String> SBOMContext::TargetDependencyKeys(bool wt)
{
	if(wt) {
		Vector<String> keys;
		keys << "VCPKG" << "WIN32";
		return keys;
	}
	return GetExternalDependenciesKeys();   // native POSIX/Windows: unchanged
}

// Dispatcher: dpkg / rpm arms keep their original guards verbatim.
void SBOMContext::FillPkgMeta(SBOMComponent& m)
{
	if(m.name.IsEmpty())
		return;

	if(!m.purl.IsEmpty() || !m.version.IsEmpty())
		return;

	if(IsDPKG())
		DpkgFillPkgMeta(m);
	else if(IsRPM())
		RpmFillPkgMeta(m);
}

// ============================================================================
// MAIN ORCHESTRATION: CreateSBOMComponents
// ============================================================================

Array<SBOMComponent> SBOMContext::CreateSBOMComponents(Gate<int, int>& progress)
{
	Array<SBOMComponent> cs;

	progress_gate = &progress;
	own_ci.SetCount(wspc.GetCount(), -1);
	all_host = AllHostPackagesList();
	
	// External-component bookkeeping — the duplicate-component and
	// duplicate-edge guard for the whole graph. Every component insertion
	// path MUST go through CompIdxExt before emitting.
	auto AddDependency = [&](SBOMComponent& m, const String& name) {
		for(String h : { m.bom_ref + "\v" + name, name + "\v" + m.bom_ref }) {
			if(deps_done.Find(h) >= 0)
				return;
			deps_done.Add(h);
		}
		m.depends << name;
	};

	auto CompIdxExt = [&](const String& nm, bool& fresh) -> int {
		if(seen_ext.Find(nm) >= 0) {
			fresh = false;
			return ci_ext.Get(nm, -1);
		}
		seen_ext.Add(nm);
		ci_ext.Add(nm, cs.GetCount());
		fresh = true;
		return cs.GetCount();
	};

	auto TierByName = [&](const String& s) {
		for(int i = 0; i < (int)T_COUNT; i++)
			if(s == sbom_tier[i])
				return i;
		return -1;
	};

	auto Tick = [&]() -> bool {
		if(progress_done >= progress_total)
			progress_total = progress_done + 1;
		bool cancel = (*progress_gate)(progress_done, progress_total);
		progress_done++;
		return cancel;
	};

	// Embedded sbom.json fragment reader
	auto ReadComponent = [&](Value p, int pi) {
		String nm = p["name"];
		if(nm.IsEmpty())
			return;
		bool fresh;
		CompIdxExt(nm, fresh);
		if(fresh) {
			SBOMComponent& m = cs.Add();
			m.bom_ref = p["bom-ref"];
			if(!m.bom_ref.GetCount())
				m.bom_ref = nm;
			m.name      = nm;
			m.version   = p["versionInfo"];
			m.licenses << ~p["licenseConcluded"];
			m.homepage  = p["homepage"];
			m.originUrl = p["downloadLocation"];
			String ty = p["type"];
			if(ty.GetCount())
				m.type = ty;
			int t = TierByName(p["upp:deployment-mode"]);
			m.external = t >= 0 ? t : T_DYNAMIC_BUNDLED;
			for(Value r : p["externalRefs"])
				if(r["referenceType"] == "purl")
					m.purl = r["referenceLocator"];
		}
		int ci = ci_ext.Get(nm, -1);
		if(ci >= 0 && own_ci[pi] >= 0)
			AddDependency(cs[own_ci[pi]], cs[ci].bom_ref);
	};

	// Step 2 — per-package gather (git log batched via GitResolveCommit)
	progress_done = 0;
	progress_total = wspc.GetCount();
	for(int i = 0; i < wspc.GetCount(); i++) {
		if(Tick()) {
			cs.Clear();
			return cs;
		}
		const Package& pk = wspc.GetPackage(i);
		String n = wspc[i];
		{
			SBOMComponent& m = cs.Add();
			own_ci[i] = cs.GetCount() - 1;

			String root = GitRepoTopLevel(PackageDirectory(n));
			String origin = root.GetCount() ? repo_origin.Get(root) : String();
			m.bom_ref = m.name = PkgName(n);               // PkgName also canonicalizes

			if(i == 0)
				m.type = "application";

			if(origin.GetCount()) {
				m.originUrl = "git+" + origin;
				origin.TrimEnd(".git");
				m.homepage = origin;
			}
			Tuple<String, String, String> commit = GitResolveCommit(root);
			
			String hash = commit.a;
			String sha1 = commit.b;
			String ts = commit.c;
			if(hash.GetCount()) {
				m.version = Format8601Z(Atoi64(ts) + Time(1970, 1, 1)) + "#" + hash;
				if(m.originUrl.GetCount())
					m.sourceDistributions << MakeTuple<String, String, String>(m.originUrl + "@" + hash, "SHA-1", sha1);
			}
			m.licenses << Nvl(pk.license_id, "BSD-2-Clause");
			
			for(const OptItem& u : pk.uses)
				AddDependency(m, PkgName(u.text));
		}

		for(String fn : pk.file) {
			String file = SourcePath(n, fn);
			if(ToLower(GetFileName(file)) == "sbom.json") {
				Value sbom = ParseJSON(LoadFile(file));
				if(sbom.Is<ValueArray>())
					for(Value p : sbom)
						ReadComponent(p, i);
				else
					ReadComponent(sbom, i);
			}
		}

		if(!win_target) {
			Index<String> cfg = mk.PackageConfig(wspc, i, bm, mk.mainconfigparam, host, builder);

			auto GatherNames = [&](const Array<OptItem>& set, int decl) {
				for(String s : Split(Gather(set, cfg.GetKeys()), ' '))
					if(s.GetCount())
						decls.Add(Decl(decl, s, i));
			};
			GatherNames(pk.static_library, DECL_STATIC_LIBRARY);
			GatherNames(pk.dynamic_library, DECL_DYNAMIC_LIBRARY);
			GatherNames(pk.library, DECL_LIBRARY);

			for(String mod : Split(Gather(pk.pkg_config, cfg.GetKeys()), ' '))
				for(String s : Split(builder.EvalCmdX("pkg-config --libs " + mod), CharFilterWhitespace))
					if(s.TrimStart("-l") && s.GetCount())
						decls.Add(Decl(DECL_LIBRARY, s, i));
		}
		
		for(auto s : RequiredExternalDependenciesInfo(pk, dep_keys)) {
			String nm = s.name;
			int q = nm.Find('|');
			if(q >= 0) {
				bool win_sel = win_target && VcpkgList().GetCount() > 0;
				nm.Clear();
				for(String a : Split(s.name, '|')) {
					a = TrimBoth(a);
					if(!a.GetCount())
						continue;
					if(win_sel) {
						// Windows target: only an installed vcpkg port is valid.
						// The other branches are POSIX-only (gtk/x11/fontconfig...)
						// and must be dropped, not kept as a fallback — keeping them
						// is what leaked libbz2-dev / libgtk-3-dev into the cross SBOM.
						if(IsVcpkgPort(a)) {
							nm = a;
							break;
						}
					}
					else
						if(!nm.GetCount() || all_host.Find(a) >= 0)
							nm = a;              // unchanged host-side semantics
				}
				if(win_sel && !nm.GetCount())
					continue;
			}
			if(!nm.GetCount())
				continue;
			AddDependency(cs[own_ci[i]], nm);
			decls.Add(Decl(DECL_EXTERNAL_DEPENDENCY, nm, i));
			if(s.license.GetCount())
				override_licenses.GetAdd(nm) = s.license;
		}
	}

	progress_total = progress_done + decls.GetCount();

#ifdef PLATFORM_POSIX
	if(!win_target && IsRPM()) {
		(void)FileOwnerDB();  // Force-populate RPM ownership map before step 3 uses it
	}
#endif

	// Memoized resolve — step 3 and 4 share results.
	// Host ownership describes the *host*, not the target. For a foreign
	// target, ldconfig/readlink/dpkg-S answer about the wrong machine.
	auto GetResolvedOwner = [&](const String& base) -> String {
		if(win_target)
			return String();
	
		int q = file_owner_memo.Find(base);
		if(q >= 0)
			return file_owner_memo[q];

		String file = FindSoPath(base);
		String realpath;
		int path_idx = candidate_files.Find(file);
		if(path_idx < 0) {
			realpath = RealFile(file);
			candidate_files.Add(file);
			resolved_file_to_base.Add(realpath);
		} else {
			realpath = resolved_file_to_base[path_idx];
		}

		String owner;
#ifdef PLATFORM_POSIX
		if(IsRPM() || IsDPKG())
			owner = OwnerOfFileFast(realpath);
#endif
		if(!owner.GetCount())
			owner = OwnerOfFile(realpath);   // host-only fallback; empty on Win32

		file_owner_memo.Add(base, owner);
		return owner;
	};

	// Step 3 — resolution + tier assignment
	for(const Decl& d : decls) {
		if(Tick()) {
			cs.Clear();
			return cs;
		}
		int tier;
		String nm = d.name;
		
		if(d.decl == DECL_EXTERNAL_DEPENDENCY)
			tier = win_target ? VcpkgTier(link_mode) : T_STATIC_THIRD_PARTY;
		else {
			String ext = ToLower(GetFileExt(d.name));
			bool dynq = ext == ".so" || ext == ".dll"
					|| d.decl == DECL_DYNAMIC_LIBRARY
					|| d.decl == DECL_LIBRARY && link_mode == LINK_DYNAMIC;
			String owner;
			if(dynq)
				owner = GetResolvedOwner(d.name);
			else {
				if(win_target) {
					// Cross-build: don't resolve static libs against Linux filesystem
					tier = T_STATIC_THIRD_PARTY;
					nm = d.name;
				} else {
					String sta = StaticOwner(d.name);
					tier = DeclTier(d.decl, d.name, link_mode, DynTierOf(String()));
					nm = sta.GetCount() ? sta : d.name;
				}
				bool fresh;
				CompIdxExt(nm, fresh);
				if(fresh) {
					SBOMComponent& m = cs.Add();
					m.bom_ref = m.name = nm;
					m.external = tier;
					
					// Add vcpkg metadata filling for win_target here too
					if(win_target) {
						VcpkgFillFromInstalledList(m, nm);
					}
				}
				if(own_ci[d.pk] >= 0)
					AddDependency(cs[own_ci[d.pk]], nm);
				continue;
			}

			tier = DeclTier(d.decl, d.name, link_mode, DynTierOf(owner));
			if(tier == T_DYNAMIC_PLATFORM && owner.GetCount())
				nm = owner;               // Use owning package name, not bare soname
			else if(tier == T_STATIC_THIRD_PARTY)
				nm = d.name;
		}
		
		bool fresh;
		CompIdxExt(nm, fresh);
		if(fresh) {
			SBOMComponent& m = cs.Add();
			m.bom_ref = m.name = nm;
			m.external = tier;
			
#ifdef PLATFORM_POSIX
			if(!win_target && (IsRPM() || IsDPKG())
			&& (tier == T_STATIC_THIRD_PARTY || tier == T_DYNAMIC_PLATFORM))
				FillPkgMeta(m);
#endif
			if(win_target && (tier == T_DYNAMIC_BUNDLED || tier == T_STATIC_THIRD_PARTY))
				VcpkgFillFromInstalledList(m, nm);
		}
		if(own_ci[d.pk] >= 0)
			AddDependency(cs[own_ci[d.pk]], nm);
	}

	// Platform component
	if(win_target) {
		static const char *OS_REF = "com.microsoft.windows";
		bool fresh;
		CompIdxExt(OS_REF, fresh);
		if(fresh) {
			SBOMComponent& m = cs.Add();
			m.type     = "operating-system";
			m.bom_ref  = OS_REF;
			m.name     = "Microsoft Windows";
			m.external = T_DYNAMIC_PLATFORM;
		}
		if(own_ci[0] >= 0)
			AddDependency(cs[own_ci[0]], OS_REF);
	}

	// Windows vcpkg transitive expansion
	if(win_target) {
		Index<String> vnames;
		for(const Decl& d : decls)
			if(d.decl == DECL_EXTERNAL_DEPENDENCY)
				vnames.FindAdd(d.name);
		progress_total = progress_done + vnames.GetCount();
		for(int i = 0; i < vnames.GetCount(); i++) {
			if(Tick()) {
				cs.Clear();
				return cs;
			}
			String name = vnames[i];
			
			Value spdx = ParseJSON(LoadFile(
				GetFileFolder(VcpkgExe()) + "/installed/" + triplet + "/share/" + name + "/vcpkg.spdx.json"));
			for(Value p : spdx["packages"])
				if(p["SPDXID"] == "SPDXRef-port") {
					int q = ci_ext.Get(name, -1);
					if(q < 0)
						break;
					SBOMComponent& m = cs[q];
					m.version = p["versionInfo"];
					String lic = p["licenseConcluded"];
					if(!IsEmpty(lic)) {
						m.licenses.Clear();
						m.licenses << ~lic;
					}
					m.homepage = p["homepage"];
					m.originUrl = p["downloadLocation"];
					for(Value r : p["externalRefs"])
						if(r["referenceType"] == "purl")
							m.purl = r["referenceLocator"];
					for(String dep : Split(Split(Split(Sys(VcpkgExe() + " depend-info " + name),
													CharFilterCrLf).Top(), ':').Top(), ',')) {
						dep = TrimBoth(dep);
						if(dep.GetCount() && !dep.StartsWith("vcpkg-")) {
							bool fresh;
							CompIdxExt(dep, fresh);
							if(fresh) {
								SBOMComponent& mc = cs.Add();
								mc.bom_ref = mc.name = dep;
								mc.external = m.external;
								VcpkgFillFromInstalledList(mc, dep);
							}
							int fq = ci_ext.Get(name, -1);
							if(fq >= 0)
								AddDependency(cs[fq], dep);
							if(vnames.Find(dep) < 0) {
								vnames.Add(dep);
								progress_total++;
							}
						}
					}
					for(Value p2 : spdx["packages"]) {
						String id = p2["SPDXID"];
						if(id.StartsWith("SPDXRef-resource-")) {
							String url = p2["downloadLocation"];
							if(!IsEmpty(url) && url != "NONE")
								for(Value ch : p2["checksums"])
									cs[q].sourceDistributions
									<< MakeTuple<String, String, String>(~url, ~ch["algorithm"], ~ch["checksumValue"]);
						}
					}
					break;
				}
		}
	}

	// MSVC / LLVM-Mingw runtime components
	if(win_target && msvc) {
		static const char *CRT_REF = "com.microsoft.vcruntime";
		bool crt_dyn = md || link_mode == LINK_DYNAMIC;

		int ci = ci_ext.Find(CRT_REF);
		if(ci < 0) {
			SBOMComponent& r = cs.Add();
			r.bom_ref  = CRT_REF;
			r.name     = crt_dyn ? "Microsoft Visual C++ Runtime (dynamic)"
								: "Microsoft Visual C++ Runtime (statically linked)";
			r.external = crt_dyn ? T_DYNAMIC_PLATFORM
								: T_STATIC_THIRD_PARTY;
			ci_ext.Add(CRT_REF, cs.GetCount() - 1);
		}
		if(own_ci[0] >= 0)
			AddDependency(cs[own_ci[0]], CRT_REF);
	}
	else if(win_target && clang) {
		struct Rt { const char *ref, *name, *license; };
		static const Rt rt[] = {
			{ "org.mingw.w64-crt",    "mingw-w64 C runtime (statically linked)",          "" },
			{ "org.llvm.compiler-rt", "LLVM compiler-rt builtins (statically linked)",     "Apache-2.0 WITH LLVM-exception" },
			{ "org.llvm.libunwind",   "LLVM libunwind (statically linked)",                "Apache-2.0 WITH LLVM-exception" },
			{ "org.llvm.libcxx",      "LLVM libc++ (statically linked)",                   "Apache-2.0 WITH LLVM-exception" },
		};
		for(const Rt& r : rt) {
			bool fresh;
			CompIdxExt(r.ref, fresh);
			if(fresh) {
				SBOMComponent& m = cs.Add();
				m.type     = "library";
				m.bom_ref  = r.ref;
				m.name     = r.name;
				m.external = T_STATIC_THIRD_PARTY;
				if(*r.license)
					m.licenses << r.license;
				if(r.ref == String("org.mingw.w64-crt"))
					m.version = MingwCrtVersion();
			}
			if(own_ci[0] >= 0)
				AddDependency(cs[own_ci[0]], r.ref);
		}
	}

#ifdef PLATFORM_POSIX
	if(!win_target) {
		// Step 4 — T4: NEEDED closure (BFS, batched per round)
		Vector<String> queue;               // loader file paths
		Vector<String> qfrom;               // owning package of each file
		Index<String>   discovered;         // enqueued-or-walked: each file enters exactly once

		for(const Decl& d : decls) {
			if(d.decl == DECL_EXTERNAL_DEPENDENCY) continue;
			String ext = ToLower(GetFileExt(d.name));
			bool dynq = ext == ".so" || ext == ".dll"
					|| d.decl == DECL_DYNAMIC_LIBRARY
					|| d.decl == DECL_LIBRARY && link_mode == LINK_DYNAMIC;
			if(!dynq) continue;
			String owner = GetResolvedOwner(d.name);
			if(!owner.GetCount()) continue;
			String file = FindSoPath(d.name);
			if(file.IsEmpty()) continue;
			if(discovered.Find(file) < 0) {
				discovered.Add(file);
				queue.Add(file);
				qfrom.Add(owner);
			}
		}

		int head = 0;
		while(head < queue.GetCount()) {
			if(Tick()) { cs.Clear(); return cs; }
			int round_end = queue.GetCount();

			Vector<String> round;            // the new frontier
			for(int i = head; i < round_end; i++)
				round.Add(queue[i]);

			Vector<Vector<String>> needed = BatchReadelfNeeded(round);

			// Resolve ALL loader paths of this round's NEEDED sonames
			// with one readlink call. Trim-then-split-then-count-guard:
			// without the pre-trim the count is always args+1, the guard
			// always fails, and every transitive loses its owner.
			Index<String> rawfiles;
			for(const Vector<String>& ns : needed)
				for(const String& sn : ns)
					rawfiles.FindAdd(FindSonameFile(sn));   // may be empty

			VectorMap<String, String> real_of;
			{
				Vector<String> args;
				for(const String& p : rawfiles)
					if(p.GetCount())
						args.Add(p);
				if(args.GetCount()) {
					String out = TrimBoth(Sys("readlink -f " + Join(args, " ")));
					Vector<String> lines = Split(out, '\n', false);
					if(lines.GetCount() == args.GetCount())
						for(int i = 0; i < args.GetCount(); i++)
							real_of.Add(args[i], TrimBoth(lines[i]));
				}
			}

			// Attribute the round's edges and enqueue the newly discovered files.
			for(int i = 0; i < round.GetCount(); i++) {
				String from = qfrom[head + i];
				for(const String& sn : needed[i]) {
					String rawfile = FindSonameFile(sn);
					String nfile = real_of.Get(rawfile, String());
					if(!nfile.GetCount()) nfile = rawfile;

					String owner = nfile.GetCount() ? OwnerOfFileFast(nfile) : String();
					if(owner.IsEmpty())
						owner = SonameOwnerDB().Get(sn, String());   // RPM provides + DPKG shlibs

					String nm = owner.GetCount() ? owner : sn;

					bool fresh;
					CompIdxExt(nm, fresh);
					if(fresh) {
						SBOMComponent& m = cs.Add();
						m.bom_ref = m.name = nm;
						m.external = T_DYNAMIC_PLATFORM_TRANSITIVE;
						if(IsRPM() || IsDPKG())
							FillPkgMeta(m);
					}
					int fq = ci_ext.Get(from, -1);
					if(fq >= 0)
						AddDependency(cs[fq], nm);
					if(nfile.GetCount() && discovered.Find(nfile) < 0) {
						discovered.Add(nfile);
						queue.Add(nfile);           // <- the closure edge that was lost
						qfrom.Add(nm);
					}
				}
			}
			head = round_end;
		}
		
		// Step 5 — toolchain baseline (POSIX only)
		if(!msvc) {
			if(mk.mainconfigparam.Find("PORTABLE_HYBRID") >= 0) {
				progress_total = progress_done + 1;
				if(Tick()) {
					cs.Clear();
					return cs;
				}
				String nm = IsRPM() ? "glibc" : "libc6";
				bool fresh;
				CompIdxExt(nm, fresh);
				if(fresh) {
					SBOMComponent& m = cs.Add();
					m.bom_ref = m.name = nm;
					m.external = T_DYNAMIC_PLATFORM;
					if(IsRPM() || IsDPKG())
						FillPkgMeta(m);
				}
				if(own_ci[0] >= 0)
					AddDependency(cs[own_ci[0]], nm);
			}
			else if(link_mode == LINK_DYNAMIC) {
				progress_total = progress_done + 4;
				for(const char *rt : {"c", "m", "stdc++", "gcc_s"}) {
					if(Tick()) {
						cs.Clear();
						return cs;
					}
					String owner = OwnerOfFileFast(DynOwnerFile(rt));
					if(owner.IsEmpty() && IsRPM())
						owner = SonameOwnerDB().Get("lib" + String(rt) + ".so.1", String());
					String nm = owner.GetCount() ? owner : String("lib") + rt;
					bool fresh;
					int c = CompIdxExt(nm, fresh);
					if(fresh) {
						SBOMComponent& m = cs.Add();
						m.bom_ref = m.name = nm;
						m.external = T_DYNAMIC_PLATFORM;
						if(IsRPM() || IsDPKG())
							FillPkgMeta(m);

					}
					else if(cs[c].external > T_DYNAMIC_PLATFORM)
						cs[c].external = T_DYNAMIC_PLATFORM;    // direct req outranks transitive discovery
						
					if(own_ci[0] >= 0)
						AddDependency(cs[own_ci[0]], nm);
				}
			}
		}
	}
#endif
	
	// Final pass: apply license overrides
	for(SBOMComponent& c : cs) {
		String ol = override_licenses.Get(c.name, Null);
		if(ol.GetCount()) {
			c.licenses.Clear();
			c.licenses << ol;
		}
	}

	return cs;
}

// ============================================================================
// Entry Points — delegate to SBOMContext
// ============================================================================

Array<SBOMComponent> MakeBuild::CreateSBOMComponents(const String& triplet, Gate<int, int> progress)
{
	Host host;
	CreateHost(host);
	One<Builder> b = CreateBuilder(&host);
	if(!b)
		return Array<SBOMComponent>();

	const Workspace& wspc = GetIdeWorkspace();
	SBOMContext ctx(*this, triplet, wspc, host, *b);
	return ctx.CreateSBOMComponents(progress);
}

String MakeBuild::CreateSBOM(const String& triplet, int mode)
{
	return CreateSBOM(CreateSBOMComponents(triplet), mode);
}

// ============================================================================
// Emission (unchanged, already efficient)
// ============================================================================

String MakeBuild::CreateSBOM(const Array<SBOMComponent>& cs, int mode)
{
	auto EmitLicense = [&](JsonArray& arr, const String& s) {
		if(s.IsEmpty() || s == "NOASSERTION")
			return;

		static Index<String> ops;
		static Index<String> lcspdx;
		static Vector<String> canon;
		if(canon.IsEmpty()) {
			for(int i = 0; i < SPDXLicenses().GetCount(); i++) {
				canon << SPDXLicenses()[i];
				lcspdx.Add(ToLower(SPDXLicenses()[i]));
			}
			ops.Add("("); ops.Add(")"); ops.Add("or"); ops.Add("and"); ops.Add("with");
		}

		String low = ToLower(s);
		low.TrimStart("(");
		low.TrimEnd(")");
		Vector<String> vec = Split(low, ' ');
		if(vec.GetCount() >= 3) {
			for(int i = 0; i < vec.GetCount(); i++) {
				if(lcspdx.Find(vec[i]) < 0 && ops.Find(vec[i]) < 0)
					break;
				if(i == vec.GetCount() - 1) {
					String lic;
					for(int j = 0; j < vec.GetCount(); j++) {
						int o = ops.Find(vec[j]);
						if(o >= 0)
							lic << ToUpper(ops[o]) << " ";
						else {
							int l = lcspdx.Find(vec[j]);
							if(l >= 0)
								lic << canon[l] << " ";
						}
					}
					arr << Json("expression", TrimRight(lic));
					return;
				}
			}
		}
		int f = lcspdx.Find(low);
		arr << Json("license", f >= 0 ? Json("id", canon[f]) : Json("name", s));
	};

	int tier_clamp = mode == SBOM_CONTAINER ? T_DYNAMIC_BUNDLED : T_COUNT;
	int depth = mode == SBOM_BASE ? T_DYNAMIC_BUNDLED : T_DYNAMIC_PLATFORM_TRANSITIVE;
	
	Index<String> alive;
	Vector<int>   emitted_i;

	JsonArray components, dependencies;
	Json      main_component;

	for(int i = 0; i < cs.GetCount(); i++) {
		const SBOMComponent& c = cs[i];
		if(c.external > depth)
			continue;
		if(c.name.IsEmpty())
			continue;

		Json component;
		component("type", c.type)
				("name", c.name)
				("bom-ref", c.bom_ref);
		if(c.version.GetCount())
			component("version", c.version);

		if(mode != SBOM_CONTAINER && c.external >= T_DYNAMIC_PLATFORM)
			component("scope", "required")
					("isExternal", true);

		JsonArray licenses;
		for(const String& s : c.licenses)
			if(!IsEmpty(s))
				EmitLicense(licenses, s);
		if(licenses)
			component("licenses", licenses);

		JsonArray extRefs;
		if(!IsEmpty(c.homepage))
			extRefs << Json("type", "website")("url", c.homepage);
		if(!IsEmpty(c.originUrl))
			extRefs << Json("type", "distribution")("url", c.originUrl);

		auto SourceRef = [](const auto& s) {
			Json r;
			r("type", "source-distribution")("url", s.a);
			String alg = ToUpper(s.b);
			alg = alg.GetCount() > 3 && alg[3] != '-' && alg.StartsWith("SHA") ? "SHA-" + alg.Mid(3) : alg;
			if(alg.GetCount() && s.c.GetCount())
				r("hashes", JsonArray() << Json("alg", alg)("content", s.c));
			return r;
		};
		for(const auto& s : c.sourceDistributions)
			if(!IsEmpty(s.a))
				extRefs << SourceRef(s);

		if(extRefs)
			component("externalReferences", extRefs);
		if(!IsEmpty(c.purl))
			component("purl", c.purl);

		int tier_value = c.type == "operating-system"
			? c.external
			: clamp(c.external, (int)T_STATIC_IN_REPO, tier_clamp);
		component("properties", JsonArray()
			<< Json("name", "upp:deployment-mode")("value", sbom_tier[tier_value]));

		alive.Add(c.bom_ref);
		emitted_i << i;
		if(!main_component)
			main_component = component;
		else
			components << component;
	}

	for(int i : emitted_i) {
		const SBOMComponent& c = cs[i];
		JsonArray deps;
		for(String dep : c.depends)
			if(alive.Find(dep) >= 0)
				deps << dep;
		if(deps)
			dependencies << Json("ref", c.bom_ref)("dependsOn", deps);
	}

	Json metadata = Json("timestamp", Format8601Z(GetUtcTime()))
						("tools", JsonArray() << Json("vendor", "U++")
													("name", UMK ? "umk" : "TheIDE"));
	metadata("lifecycles", JsonArray() << Json("phase", "build"));
	metadata("component", main_component);

	Json sbom;

	sbom("bomFormat", "CycloneDX")
		("specVersion", "1.7")
		("version", 1)
		("serialNumber", "urn:uuid:" + Uuid::CreateV4().ToStringWithDashes())
		("metadata", metadata);

	if(components)
		sbom("components", components);
	if(dependencies)
		sbom("dependencies", dependencies);

	return sbom;
}