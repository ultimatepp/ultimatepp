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
	String GitTagCommit(const String &repo, const String &tag);
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
	String MsvcCrtVersion();
	String WindowsSdkVersion();
	String ClangVersion();
	String FindUpwards(const String& dir, const String& filename, int maxdepth);

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

// Resolve a tag to its commit SHA. Tries the peeled form first
// (annotated tags -> the tagged commit), then the plain ref
// (lightweight tags point directly at the commit). Returns empty
// when the tag doesn't exist or the network is unavailable —
// callers must treat empty as "unknown", never as "not shipped".
String SBOMContext::GitTagCommit(const String &repo, const String &tag)
{
	Vector<String> forms;
	forms << tag + "^{}" << tag;                    // annotated, then lightweight
	for(const String& f : forms) {
		String out = Sys(String(git) << " ls-remote " << repo
		                        << " refs/tags/" << f);
		for(const String& line : Split(out, '\n')) {
			Vector<String> p = Split(line, '\t');
			if(p.GetCount() == 2 && Regex("^[0-9a-fA-F]{40,64}$").Match(p[0])
			   && p[1] == "refs/tags/" + f)
				return p[0];
		}
	}
	return String();
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

// Walks up from <dir> looking for <filename>. Used to locate SDKManifest.xml,
// which sits above the INCLUDE entries (<...>\Include\<sdkver>\ucrt) rather
// than inside any of them.
String SBOMContext::FindUpwards(const String& dir, const String& filename, int maxdepth)
{
	String d = TrimBoth(dir);
	while(d.GetCount() && (d.EndsWith("\\") || d.EndsWith("/")))
		d.Trim(d.GetCount() - 1);
	for(int i = 0; i <= maxdepth; i++) {
		String f = AppendFileName(d, filename);
		if(FileExists(f))
			return f;
		String p = GetFileFolder(d);
		if(p.IsEmpty() || p == d)
			break;
		d = p;
	}
	return String();
}

// Reads _VC_CRT_{MAJOR,MINOR,BUILD}_VERSION from crtversion.h found in the
// build method's INCLUDE path; composes "14.51.36244". The header is
// authoritative over the toolset folder name — the CRT is serviced
// independently (folder 15.61.36231 vs header 14.51.36244 in the wild).
String SBOMContext::MsvcCrtVersion()
{
	for(String p : Split(bm.Get("INCLUDE", ""), ';')) {
		p = TrimBoth(p);
		if(!p.GetCount())
			continue;
		String s = LoadFile(AppendFileName(p, "crtversion.h"));
		if(s.IsEmpty())
			continue;
		auto M = [&](const char *n) -> String {
			String key = String("#define ") + n;
			int q = s.Find(key);
			if(q < 0)
				return String();
			q += key.GetCount();
			while(q < s.GetCount() && (s[q] == ' ' || s[q] == '\t'))
				q++;
			int e = q;
			while(e < s.GetCount() && IsDigit(s[e]))
				e++;
			return s.Mid(q, e - q);
		};
		String maj = M("_VC_CRT_MAJOR_VERSION");
		String min = M("_VC_CRT_MINOR_VERSION");
		String bld = M("_VC_CRT_BUILD_VERSION");
		if(maj.GetCount() && min.GetCount() && bld.GetCount())
			return maj + '.' + min + '.' + bld;
	}
	return String();
}

// SDKManifest.xml carries PlatformIdentity = "UAP, Version=10.0.26100.0".
// Located by walking up from each INCLUDE entry; depth 6 comfortably covers
// <kits>\<ver>\Include\<sdkver>\{ucrt,um,winrt,cppwinrt}.
String SBOMContext::WindowsSdkVersion()
{
	for(String p : Split(bm.Get("INCLUDE", ""), ';')) {
		p = TrimBoth(p);
		if(!p.GetCount())
			continue;
		String f = FindUpwards(p, "SDKManifest.xml", 6);
		if(!f.GetCount())
			continue;
		String s = LoadFile(f);
		int q = s.Find("PlatformIdentity");
		if(q < 0)
			continue;
		q = s.Find("Version=", q);
		if(q < 0)
			continue;
		q += 8;
		int e = q;
		while(e < s.GetCount() && (IsDigit(s[e]) || s[e] == '.'))
			e++;
		String v = s.Mid(q, e - q);
		if(v.GetCount())
			return v;
	}
	return String();
}

// "clang version 23.1.3 (https://github.com/llvm/llvm-project.git 0d261d1...)"
// "Ubuntu clang version 18.1.3 (1ubuntu1)"  -> bare "18.1.3"
// Probed through the builder so the build method's PATH applies.
// "clang version 23.1.3 (https://github.com/llvm/llvm-project.git 0d261d1...)"
// "Ubuntu clang version 18.1.3 (1ubuntu1)"  -> bare "18.1.3"
// cc has no path, so the build method's PATH is walked — the same
// mechanism as MingwCrtVersion(). Same ';' split caveat as there.
String SBOMContext::ClangVersion()
{
	Vector<String> path = Split(bm.Get("PATH", ""), ';');
	for(String p : path) {
		p = TrimBoth(p);
		if(!p.GetCount())
			continue;
		String exe = AppendFileName(p, cc);
		if(!FileExists(exe)) {
#ifdef PLATFORM_WIN32
			exe << ".exe";
			if(!FileExists(exe))
#endif
				continue;
		}
		RDUMP(exe);
		String call = "\"" + exe + "\" --version";
		RDUMP(call);
		
		String out = Sys("\"" + exe + "\" --version");
		int q = out.Find("clang version ");
		if(q < 0)
			continue;
		q += 14;
		int e = q;
		while(e < out.GetCount() && (IsDigit(out[e]) || out[e] == '.'))
			e++;
		String v = out.Mid(q, e - q);
		if(v.GetCount())
			return v;
	}
	// cc not under the build method PATH (system-wide compiler): bare name
	String out = Sys(cc + " --version");
	int q = out.Find("clang version ");
	if(q < 0)
		return String();
	q += 14;
	int e = q;
	while(e < out.GetCount() && (IsDigit(out[e]) || out[e] == '.'))
		e++;
	return out.Mid(q, e - q);
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

	// ------------------------------------------------------------------------
	// Embedded sbom.json reader — full CycloneDX document variant.
	//
	// Recognized by the presence of a root "components" array:
	//   { "bomFormat": "CycloneDX", "components": [ ... ], "dependencies": [...] }
	// ReadComponent above remains the reader for the SPDX-ish fragment
	// shape (bare component object / array of them) and is untouched.
	// ------------------------------------------------------------------------

	// CycloneDX carries custom hints as properties[{name, value}]
	auto CDXProp = [](Value c, const char *key) -> String {
		for(Value p : c["properties"])
			if(p["name"] == key)
				return p["value"];
		return String();
	};

	// licenses[] entries: {expression:...} or {license:{id:...}|{name:...}}
	auto CDXLicenses = [](Value c, SBOMComponent& m) {
		for(Value l : c["licenses"]) {
			String s = l["expression"];
			if(!s.GetCount()) s = l["license"]["id"];
			if(!s.GetCount()) s = l["license"]["name"];
			if(s.GetCount())
				m.licenses << s;
		}
	};

	// externalReferences[] -> homepage / originUrl / sourceDistributions.
	// Hash format is normalized to the (url, alg, checksum) tuple shape
	// used everywhere else in this file; alg passes through as written
	// ("SHA-1", "SHA-256", ...). First hash entry of a ref wins, matching
	// the vcpkg SPDX reader's attitude: one digest is enough.
	auto CDXExtRefs = [](Value c, SBOMComponent& m) {
		for(Value r : c["externalReferences"]) {
			String ty  = r["type"];
			String url = r["url"];
			if(!url.GetCount())
				continue;
			if(ty == "website") {
				if(!m.homepage.GetCount())
					m.homepage = url;
			}
			else if(ty == "distribution" || ty == "vcs") {
				if(!m.originUrl.GetCount())
					m.originUrl = url;
			}
			else if(ty == "source-distribution") {
				String alg, sum;
				for(Value h : r["hashes"]) {
					String a = h["alg"], v = h["content"];
					if(a.GetCount() && v.GetCount()) { alg = a; sum = v; break; }
				}
				m.sourceDistributions << MakeTuple<String, String, String>(url, alg, sum);
			}
		}
	};

	// bom-ref -> cs index, for edges declared in the document's dependencies[]
	VectorMap<String, int> cdx_ref_idx;

	auto ReadCDXComponent = [&](Value p, int pi) {
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
			m.name    = nm;
			m.version = p["version"];
			String ty = p["type"];
			if(ty.GetCount())
				m.type = ty;
			CDXLicenses(p, m);
			CDXExtRefs(p, m);
			m.purl = p["purl"];
			int t = TierByName(CDXProp(p, "upp:deployment-mode"));
			m.external = t >= 0 ? t : T_DYNAMIC_BUNDLED;
			cdx_ref_idx.Add(m.bom_ref, cs.GetCount() - 1);
		}
		int ci = ci_ext.Get(nm, -1);
		if(ci >= 0 && own_ci[pi] >= 0)
			AddDependency(cs[own_ci[pi]], cs[ci].bom_ref);
	};

	// Walk components[] (plus any nested components[], legal in CycloneDX),
	// then apply dependencies[] edges by bom-ref. Edges run last so refs
	// resolve regardless of declaration order; unknown refs are ignored —
	// the graph stays closed.
	auto ReadCDXDocument = [&](Value doc, int pi) {
		for(Value c : doc["components"]) {
			ReadCDXComponent(c, pi);
			for(Value c2 : c["components"])       // nested sub-components
				ReadCDXComponent(c2, pi);
		}
		for(Value d : doc["dependencies"]) {
			int from = cdx_ref_idx.Get(String(d["ref"]), -1);
			if(from < 0)
				continue;
			for(Value t : d["dependsOn"]) {
				int to = cdx_ref_idx.Get(String(t), -1);
				if(to >= 0 && to != from)
					AddDependency(cs[from], cs[to].bom_ref);
			}
		}
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

		// Effective WHEN keys: host/manager keys + this package's resolved
		// config (carries main-config selections like GUI X11).
		Index<String> keyset;
		for(const String& k : dep_keys)
			keyset.FindAdd(k);

		Index<String> cfg;
		if(!win_target)
			cfg = mk.PackageConfig(wspc, i, bm, mk.mainconfigparam, host, builder);
		for(int ci = 0; ci < cfg.GetCount(); ci++)
			keyset.FindAdd(cfg[ci]);
	
		for(String fn : pk.file) {
			String file = SourcePath(n, fn);
			String name = GetFileName(file);
			if(!ToLower(name).EndsWith("sbom.json"))
				continue;
			Vector<String> parts = Split(name, '.');
			if(parts.GetCount()){
				String prefix = parts[0];
				if(ToLower(prefix) == "sbom" || keyset.Find(prefix) >= 0){
					Value sbom = ParseJSON(LoadFile(file));
					if(sbom["components"].Is<ValueArray>())
						ReadCDXDocument(sbom, i);        // full CycloneDX document
					else if(sbom.Is<ValueArray>())
						for(Value p : sbom)
							ReadComponent(p, i);         // legacy SPDX fragment (array)
					else
						ReadComponent(sbom, i);          // legacy SPDX fragment (single)
				}
			}
		}
		
		if(!win_target) {
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
	
		Vector<String> eff_keys = keyset.PickKeys();
		for(auto s : RequiredExternalDependenciesInfo(pk, eff_keys)) {
	
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

	// MSVC toolchain: /MT (default here) links the VC toolset CRT *and* the
	// static UCRT (libucrt.lib, owned by the Windows SDK) into the binary.
	// /MD leaves both as imports: the UCRT is OS-shipped since Windows 10,
	// the vcruntime side comes from the VC++ redistributable.
	// crt_dyn is the builder-resolved truth: one bit, no flag re-parsing.
	if(win_target && msvc) {
		bool crt_dyn = (link_mode == LINK_DYNAMIC) || md;
		String crt_version = MsvcCrtVersion();
		String sdk_ver     = WindowsSdkVersion();

		static const char *VCRUNTIME_REF = "com.microsoft.vcruntime";
		static const char *UCRT_REF      = "com.microsoft.ucrt";

		bool fresh;
		CompIdxExt(VCRUNTIME_REF, fresh);
		if(fresh) {
			SBOMComponent& r = cs.Add();
			r.type     = "library";
			r.bom_ref  = VCRUNTIME_REF;
			r.name     = crt_dyn ? "Microsoft Visual C++ Redistributable (dynamic)"
			                    : "Microsoft Visual C++ Runtime (statically linked)";
			r.version  = crt_version;
			r.external = crt_dyn ? T_DYNAMIC_PLATFORM : T_STATIC_THIRD_PARTY;
		}
		if(own_ci[0] >= 0)
			AddDependency(cs[own_ci[0]], VCRUNTIME_REF);

		// Static UCRT slice — only when /MT actually embeds it.
		// Version is the SDK/UCRT snapshot it was built from; that is
		// what identifies the code inside the binary, not the toolset.
		if(!crt_dyn) {
			CompIdxExt(UCRT_REF, fresh);
			if(fresh) {
				SBOMComponent& u = cs.Add();
				u.type     = "library";
				u.bom_ref  = UCRT_REF;
				u.name     = "Universal CRT (statically linked)";
				u.version  = sdk_ver;
				u.external = T_STATIC_THIRD_PARTY;
			}
			if(own_ci[0] >= 0)
				AddDependency(cs[own_ci[0]], UCRT_REF);
		}
		// Dynamic case: UCRT is inside com.microsoft.windows (ucrtbase.dll),
		// nothing to add. Windows SDK never appears as a component or edge —
		// it is build provenance (property above), never shipped payload.
		
		// CRT slices resolve against OS-provided libraries: in /MD,
		// vcruntime140.dll itself imports the UCRT api sets (forwards into
		// ucrtbase.dll); under /MT the statically linked CRT and UCRT slices
		// still call kernel32/ntdll directly. Truthful in both modes, and it
		// gives downstream tooling a closed-form reason the OS node is in
		// the closure — no phantom com.microsoft.ucrt component, no
		// version claim on an OS-serviced DLL.
		static const char *OS_REF_M = "com.microsoft.windows";
		int rq = ci_ext.Get(VCRUNTIME_REF, -1);
		if(rq >= 0)
			AddDependency(cs[rq], OS_REF_M);
		if(!crt_dyn) {
			int uq = ci_ext.Get(UCRT_REF, -1);
			if(uq >= 0)
				AddDependency(cs[uq], OS_REF_M);
		}
	}
	// llvm-mingw is two independently versioned, independently CVE-tracked
	// halves: LLVM/clang (compiler, compiler-rt, libc++, libunwind) and the
	// mingw-w64 runtime (CRT). Collapsing them loses real signal, so both are
	// emitted. No object/archive-member enumeration — nothing below component
	// level is separately monitorable or updatable.
	else if(win_target && clang) {
		static const char *LLVM_REF  = "org.llvm.runtimes";
		static const char *MINGW_REF = "org.mingw.w64-crt";

		bool fresh;
		CompIdxExt(LLVM_REF, fresh);
		if(fresh) {
			SBOMComponent& m = cs.Add();
			m.type     = "library";
			m.bom_ref  = LLVM_REF;
			m.name     = "LLVM runtimes (statically linked)";
			m.version  = ClangVersion();
			if(m.version.GetCount())
				m.purl = "pkg:generic/llvm-runtimes@" + m.version;
			m.licenses << "Apache-2.0 WITH LLVM-exception";
			m.external = T_STATIC_THIRD_PARTY;
			m.homepage = "https://github.com/mstorsjo/llvm-mingw";
			m.originUrl = "git+https://github.com/mstorsjo/llvm-mingw.git";
			String srcUrl = "git+https://github.com/llvm/llvm-project.git";
			String vstring = String("llvmorg-") + m.version;
			m.sourceDistributions << Tuple<String, String, String>(srcUrl + "@" + vstring, "SHA-1", GitTagCommit(srcUrl.Mid(4), vstring));
		}
		if(own_ci[0] >= 0)
			AddDependency(cs[own_ci[0]], LLVM_REF);

		CompIdxExt(MINGW_REF, fresh);
		if(fresh) {
			SBOMComponent& m = cs.Add();
			m.type     = "library";
			m.bom_ref  = MINGW_REF;
			m.name     = "mingw-w64 C runtime (statically linked)";
			m.version  = MingwCrtVersion();
			if(m.version.GetCount())
				m.purl = "pkg:generic/mingw-w64@" + m.version;
			m.external = T_STATIC_THIRD_PARTY;
			m.homepage = "https://github.com/mstorsjo/llvm-mingw";
			m.originUrl = "git+https://github.com/mstorsjo/llvm-mingw.git";
			String srcUrl = "git+https://github.com/mingw-w64/mingw-w64.git";
			String vstring = String("v") + m.version;
			m.sourceDistributions << Tuple<String, String, String>(srcUrl + "@" + vstring, "SHA-1", GitTagCommit(srcUrl.Mid(4), vstring));
		}
		if(own_ci[0] >= 0)
			AddDependency(cs[own_ci[0]], MINGW_REF);
		
		// mingw-w64 CRT links against ucrtbase.dll; the LLVM aggregate's
		// libc++ calls the UCRT and libunwind unwinds through ntdll
		// (RtlUnwind). Both resolve inside the OS node, so the known
		// llvm-mingw UCRT dependency is expressed as an edge rather than
		// as a component we cannot version honestly.
		static const char *OS_REF_L = "com.microsoft.windows";
		int lq = ci_ext.Get(LLVM_REF, -1);
		if(lq >= 0)
			AddDependency(cs[lq], OS_REF_L);
		int mq = ci_ext.Get(MINGW_REF, -1);
		if(mq >= 0)
			AddDependency(cs[mq], OS_REF_L);
	}
	
	// Platform component — emitted last, after the CRT/toolchain block.
	// The OS node closes the graph: everything above it is either shipped
	// payload or a component that resolves into it. Edges targeting
	// com.microsoft.windows were already recorded by the CRT blocks above.
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