#include "Builders.h"

String Format8601Z(Time t)
{
	return Format("%04.4d-%02.2d-%02.2d`T%02.2d`:%02.2d`:%02.2d`Z",
	              t.year, t.month, t.day, t.hour, t.minute, t.second);
}

// --- Link-mode tree (Plan §3): replicate GccBuilder::Link() exactly --------

enum { LINK_STATIC, LINK_DYNAMIC };

static int LinkMode(const Index<String>& cfg)
{
	return cfg.Find("PORTABLE_HYBRID") >= 0
	    || (cfg.Find("SHARED") < 0 && cfg.Find("SO") < 0) ? LINK_STATIC : LINK_DYNAMIC;
}

// --- Declaration kinds ------------------------------------------------------

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

// Per-declaration tier (Plan §3). dyn_tier = provider-rule outcome:
// T3 if the platform offers the lib, T2 if we must ship it.
static int DeclTier(int decl, const String& name, int link_mode, int dyn_tier)
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
		return link_mode == LINK_STATIC ? T_STATIC_THIRD_PARTY : dyn_tier;
	}
}

// ============================================================================
// Shell-free probe layer.
//
// HostSys (LocalProcess) does NOT spawn a shell: pipes, redirects and quotes
// are passed literally and the command fails. Sys() DOES spawn a shell (the
// quoted dpkg-query format strings rely on this). RULES: one command per
// call, no pipes/redirects, quotes only via Sys; parsing happens in C++.
// ============================================================================

// --- ldconfig cache: one Sys() call, parsed once in C++ ---------------------
// Entry format: "\tlibz.so.1 (libc6,x86-64) => /lib/x86_64-linux-gnu/libz.so.1"

static Vector<Tuple<String, String>>& SoDB()
{
	static Vector<Tuple<String, String>> db;
	static bool loaded = false;
	if(!loaded) {
		loaded = true;
		for(String l : Split(Sys("ldconfig -p"), '\n')) {
			int q = l.Find("=>");
			if(q < 0)
				continue;
			String soname = TrimBoth(l.Left(q));
			int sp = soname.Find(' ');              // strip "(libc6,...)" tag
			if(sp >= 0)
				soname.Trim(sp);
			if(soname.GetCount())
				db.Add(MakeTuple(soname, TrimBoth(l.Mid(q + 2))));
		}
	}
	return db;
}

// Loader path for a BASENAME: entry whose soname starts with "lib<base>.so".
// Prefer an exact "lib<base>.so" then "lib<base>.so.<digits>" match.
static String FindSoPath(const String& base)
{
	String pref = "lib" + base + ".so";
	String best;
	for(const auto& e : SoDB()) {
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

// Exact SONAME -> loader path. Used for NEEDED entries, which arrive as FULL
// sonames ("libz.so.1") — never prefix them with "lib" like FindSoPath does.
static String FindSonameFile(const String& soname)
{
	for(const auto& e : SoDB())
		if(e.a == soname)
			return e.b;
	String pref = soname + ".";                    // tolerate minor drift
	for(const auto& e : SoDB())
		if(e.a.StartsWith(pref))
			return e.b;
	return String();
}

// --- Single-command ownership probes ----------------------------------------

static String PkgNameOnly(String s)         // first line "pkg: path" -> "pkg"
{
	int nl = s.Find('\n');
	if(nl >= 0)
		s = s.Left(nl);
	s = TrimBoth(s);
	int q = s.Find(':');                            // "libz1:amd64: /path"
	return q >= 0 ? TrimBoth(s.Mid(0, q)) : String();
}

static String RealFile(const String& path)         // readlink -f: single cmd, no quotes
{
	if(!path.GetCount())
		return String();
	String real = TrimBoth(Sys("readlink -f " + path));
	return real.GetCount() ? real : path;
}

static String OwnerOfFile(const String& file)
{
	if(!file.GetCount())
		return String();
	if(IsRPM()) {
		String r = TrimBoth(Sys("rpm -qf --qf %{NAME} " + file));
		return r.Find("not owned") >= 0 || r.StartsWith("file ") ? String() : r;
	}
	if(IsDPKG())
		return PkgNameOnly(Sys("dpkg -S " + file)); // multi-line -> first in C++
	return String();                                // BREW: later drop-in
}

// Real versioned file of lib<base>.so* (never the -dev symlink).
static String DynOwnerFile(const String& base)
{
	return RealFile(FindSoPath(base));
}

// Owner of lib<base>.a via the compiler's own resolution — single command.
static String StaticOwner(const String& base, const String& cc)
{
	String want = "lib" + base + ".a";
	String p = TrimBoth(HostSys(cc + " -print-file-name=" + want));
	return p.GetCount() && p != want && GetFileName(p) == want ? OwnerOfFile(p) : String();
}

// --- OSV deb namespace resolver ---------------------------------------------

#ifdef PLATFORM_POSIX
// Reads a KEY=value pair from /etc/os-release, quotes stripped.
// Empty when the key is absent.
static String OsReleaseField(const char *key)
{
	static const String os_release = ToLower(LoadFile("/etc/os-release"));
	String prefix = ToLower(String(key)) + "=";
	String v;
	for(String l : Split(os_release, '\n')) {
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

// OSV ecosystem namespace for deb-based hosts.
//
// OSV publishes no "linuxmint" ecosystem, so a deb derivative must resolve to
// its upstream: "ubuntu" or "debian".
//
// ORDER MATTERS: plain Ubuntu itself declares ID_LIKE="debian", so every
// "ubuntu" test must run before any "debian" fallback.

static String DebNamespace()
{
	static String cached;
	static bool loaded = false;
	if(loaded)
		return cached;
	loaded = true;

	String id = OsReleaseField("ID");
	String id_like = OsReleaseField("ID_LIKE");
	String ubuntu_codename = OsReleaseField("UBUNTU_CODENAME");

	if(id == "ubuntu" || id_like.Find("ubuntu") >= 0)
		return cached = "ubuntu";
	if(id == "debian" || id_like.Find("debian") >= 0)
		return cached = "debian";
	if(ubuntu_codename.GetCount())
		return cached = "ubuntu";
	return cached = "debian";
}

// distro qualifier for OSV purl queries.
//
// OSV's own records are release-scoped and carry the *codename*:
//   pkg:deb/debian/pcre2?arch=source&distro=bookworm
//   pkg:deb/ubuntu/git@...?arch=source&distro=esm-infra/focal
//
// A derivative must report the BASE codename, never its own: Mint 21.3 is
// jammy, LMDE 6 is bookworm.  UBUNTU_CODENAME carries the base for Ubuntu
// derivatives, VERSION_CODENAME for Debian derivatives.
static String DebDistro()
{
	static String cached;
	static bool loaded = false;
	if(loaded)
		return cached;
	loaded = true;

	String ns = DebNamespace();
	String v = (ns == "ubuntu") ? OsReleaseField("UBUNTU_CODENAME")
	                            : OsReleaseField("VERSION_CODENAME");

	// Forward only a plain release codename (bookworm, trixie, jammy, noble).
	// Derived/rolling identifiers such as "kali-rolling" name no release OSV
	// tracks; forwarding one could filter the query down to nothing, so drop
	// it and keep the broader match.
	bool ok = v.GetCount() > 0;
	for(int i = 0; i < v.GetCount(); i++) {
		char c = v[i];
		if(c < 'a' || c > 'z')
			ok = false;
	}
	cached = ok ? v : String();
	return cached;
}

#endif

static int DynTierOf(const String& dyn_owner) { return dyn_owner.GetCount() ? T_DYNAMIC_PLATFORM : T_DYNAMIC_BUNDLED; }

static int VcpkgTier(int link_mode) { return link_mode == LINK_STATIC ? T_STATIC_THIRD_PARTY : T_DYNAMIC_BUNDLED; }

static bool FillFromVcpkgList(SBOMComponent& m, const String& name, const String& triplet)
{
	for(const VcpkgInstalled& it : VcpkgList())
		if(it.name == name && it.triplets.Find(triplet) >= 0) {
			m.version = it.version;
			return true;
		}
	return false;
}

#ifdef PLATFORM_POSIX

// "ID=" line of /etc/os-release, quotes stripped; used for purl namespaces.

static String SBOMDistroId()
{
	for(String l : Split(LoadFile("/etc/os-release"), '\n')) {
		if(!l.StartsWith("ID="))
			continue;
		String v = TrimBoth(l.Mid(3));
		v.TrimStart("\"");
		v.TrimEnd("\"");
		return v.GetCount() ? v : String("linux");
	}
	return "linux";
}

// Heuristic SPDX-conformance filter for license strings scraped from
// /usr/share/doc/<pkg>/copyright: tokens of [A-Za-z0-9.+-] separated by
// single spaces (connectors AND/OR/WITH therefore pass). Rejects free text.
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

// Fills version / licenses / purl for a component whose m.name is a native
// package name (as resolved by OwnerOfFile()). Best effort: fields that
// cannot be resolved are left as-is, and unknown packages leave the
// component completely untouched.
static void FillPkgMeta(SBOMComponent& m){
	if(m.name.IsEmpty())
		return;

	if(!m.purl.IsEmpty() || !m.version.IsEmpty())
		return;                 // already filled — never accumulate

	if(IsDPKG()) {
		// Package names are [a-z0-9.+-]+ — no shell quoting needed.
		//
		// DEB — OSV indexes deb advisories by SOURCE package; without a
		// source= qualifier queries never match. Two dpkg quirks to absorb:
		//  - Source is omitted when binary name == source name  -> use name
		//  - older dpkg emits "src (epoch:ver)"                   -> trim "( ..."
		//
		// Multi-arch quirk (third): a Multi-Arch: same package installed for
		// several arches produces ONE RECORD PER ARCH — keep only the first,
		// otherwise fields from the second arch get glued onto the first.
		String arch, src, out = Sys(
			"dpkg-query -W -f='${Version}\n${Homepage}\n${Architecture}\n${Source}\n' " + m.name);

		Vector<String> rec = Split(out, '\n', false);   // keep empty fields
		if(rec.GetCount() > 4)                          // multi-arch duplicate records
			rec.SetCount(4);                            // -> first record only
		String version = rec.GetCount() > 0 ? TrimBoth(rec[0]) : String();
		m.homepage      = rec.GetCount() > 1 ? TrimBoth(rec[1]) : String();
		arch            = rec.GetCount() > 2 ? TrimBoth(rec[2]) : String();
		src             = rec.GetCount() > 3 ? TrimBoth(rec[3]) : String();

		if(!version.GetCount()) {      // not a PM package after all —
			m.homepage = String();      // record keeps name only, no junk purl
			return;
		}

		String source = TrimBoth(src);
		int pq = source.Find(" (");
		if(pq >= 0)
			source.Trim(pq);
		if(!source.GetCount())
			source = m.name;

		m.version = version;
		m.purl << "pkg:deb/" << DebNamespace() << '/' << m.name << '@' << version;
		String quals;
		if(arch.GetCount())                              // never emit bare "?arch="
			quals << "?arch=" << arch;
		
		String distro = DebDistro();                     // purl qualifiers stay sorted:
		if(distro.GetCount())                            // arch, distro, source
			quals << (quals.GetCount() ? '&' : '?') << "distro=" << distro;
			
		quals << (quals.GetCount() ? '&' : '?') << "upstream=" << source;
		m.purl << quals;

		// Debian ships license texts in /usr/share/doc/<pkg>/copyright.
		// Take the first machine-readable, SPDX-shaped "License:" line.
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
	else
	if(IsRPM()) {
		// One rpm -q call: epoch, version, url, arch, license, source rpm.
		// A failed lookup writes to stderr only, so an empty version means
		// "not a PM package after all" — the record keeps its name and no
		// junk purl.
		String url, arch, lic, srcrpm, epoch, out = Sys(
			"rpm -q --qf '%{EPOCH}\n%{VERSION}-%{RELEASE}\n%{URL}\n%{ARCH}\n%{LICENSE}\n%{SOURCERPM}\n' "
			+ m.name);
		String vrel;
		SplitTo(out, '\n', false, epoch, vrel, url, arch, lic, srcrpm);
		epoch = TrimBoth(epoch);
		if(!url.GetCount()) {                     // same "not a PM package" bail-out
			m.homepage = String();
			return;
		}
		m.version = vrel;                          // stays bare VR — epoch travels as a
		                                         // qualifier, per RH guidelines; keeps
		                                         // SRPM-tail matching and ecosystem
		                                         // queries untouched
		m.homepage = TrimBoth(url);
		lic = TrimBoth(lic);
		if(lic.GetCount() && lic != "(none)") {
			m.licenses.Clear();
			m.licenses << lic;
		}
	
		// purl namespace (plus SUSE-only distro=), resolved once from os-release
		static String ns, distro;
		static bool loaded = false;
		if(!loaded) {
			loaded = true;
			String id, vid;
			for(String l : Split(LoadFile("/etc/os-release"), '\n')) {
				l = TrimBoth(l);
				if(l.TrimStart("ID="))
					id = TrimBoth(l);
				else
				if(l.TrimStart("VERSION_ID="))
					vid = TrimBoth(l);
			}
			id.TrimStart("\"");  id.TrimEnd("\"");
			vid.TrimStart("\""); vid.TrimEnd("\"");
			Vector<String> v = Split(vid, '.');
			if(id == "sles" || id == "sle" || id == "sled") {
				ns = "suse";               // SUSE shares advisory streams across SPs
				distro = v.GetCount() > 1 ? "sle" + v[0] + "-sp" + v[1]
				       : v.GetCount()    ? "sle" + v[0]
				                         : "sles";
			}
			else                           // rocky, almalinux, centos, fedora, ...
				ns = id;                    // NO distro= — proven Rocky format
		}
	
		m.purl << "pkg:rpm/" << ns << '/' << m.name << '@' << m.version;
	
		// First qualifier opens with '?', the rest join with '&'.
		// Alphabetical order: arch, distro, epoch, upstream.
		String quals;
		auto AddQual = [&](const char *key, String val) {
			val = TrimBoth(val);
			if(!val.GetCount() || val == "(none)")
				return;
			quals << (quals.GetCount() ? '&' : '?') << key << '=' << val;
		};
		AddQual("arch", arch);
		AddQual("distro", distro);
		// %{EPOCH} renders "(none)" for zero-epoch packages and "0" is never
		// spelled — Red Hat's purl guidelines say emit epoch= only when nonzero,
		// and that is also OSV's RLSA convention. glibc stays epoch-less; openssl,
		// xz and libpng gain epoch=1 / epoch=1 / epoch=2.
		if(epoch.GetCount() && epoch != "0")
			AddQual("epoch", epoch);
		AddQual("upstream", srcrpm);
		m.purl << quals;
		return;
	}
}

#endif // PLATFORM_POSIX

// ============================================================================
// Full rewrite of MakeBuild::CreateSBOMComponents for ide/Builders/SBOM.cpp.
//
// Windows targets no longer scan library declarations (static_library /
// dynamic_library / library / pkg-config): every third-party library there
// is either a vcpkg external dependency or a component declared via a
// package sbom.json fragment (e.g. for libclang.dll). The OS entry is 
// unconditional and hangs off the application alone.
// ============================================================================

// ============================================================================
// Collection (Plan §7 Steps 1–5)
// ============================================================================
Array<SBOMComponent> MakeBuild::CreateSBOMComponents(const String& triplet, Gate<int, int> progress)
{
	Array<SBOMComponent> cs;

	Index<String> deps_done;

	auto AddDependency = [&](SBOMComponent& m, const String& name) {
		for(String h : { m.bom_ref + "\v" + name, name + "\v" + m.bom_ref }) {
			if(deps_done.Find(h) >= 0)
				return;
			deps_done.Add(h);
		}
		m.depends << name;
	};

	// --- External-component bookkeeping --------------------------------------
	Index<String> seen_ext;
	VectorMap<String, int> ci_ext;

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

	// Step 1 — environment identical to a real build
	Host host;
	CreateHost(host);
	One<Builder> b = CreateBuilder(&host);
	if(!b)
		return cs;
	VectorMap<String, String> bm = GetMethodVars(method);
	String cc = b->CompilerName();

	const Workspace& wspc = GetIdeWorkspace();

	// Tier tree — once, against package 0's config
	Index<String> main_cfg = PackageConfig(wspc, 0, bm, mainconfigparam, host, *b);
	int link_mode = LinkMode(main_cfg);

#ifdef PLATFORM_WIN32
	bool win_target = true;
#else
	bool win_target = main_cfg.Find("WIN32") >= 0;
#endif
	bool msvc = bm.Get("BUILDER", "GCC").StartsWith("MSC");
	bool clang = bm.Get("BUILDER", "GCC") == "CLANG";
	bool md   = mainconfigparam.Find("MD") >= 0;

	Index<String> all_host = AllHostPackagesList();
	VectorMap<String, String> override_licenses;

	Vector<Decl> decls;
	Vector<int>  own_ci(wspc.GetCount(), -1);

#ifdef PLATFORM_WIN32
	String git = GetExeDirFile("bin/mingit/cmd/git");
#else
	String git = "git";
#endif

	VectorMap<String, String> repo_prefix;
	VectorMap<String, String> repo_cache; // repository top-level dir -> origin url

	auto OriginOf = [&](const String& dir) {
		// Walk up from 'dir' to the nearest directory with a cached origin.
		// Since only repository top-levels are ever stored, every directory
		// below one resolves to the same entry.
		String d = NormalizePath(dir);
		for(;;) {
			int q = repo_cache.Find(d);
			if(q >= 0)
				return repo_cache[q];
			String parent = GetFileFolder(d);
			if(parent.IsEmpty() || parent == d)
				break; // reached filesystem root
			d = parent;
		}
		// Resolve the repository once, at its top-level directory:
		String root = TrimBoth(Sys(git + " -C " + dir + " rev-parse --show-toplevel"));
		if(root.GetCount()) {
			root = NormalizePath(root);
			String origin = TrimBoth(Sys(git + " -C " + root + " config --get remote.origin.url"));
			if(origin.StartsWith("git@github.com:"))
				origin.Replace("git@github.com:", "https://github.com/");
			repo_cache.Add(root, origin);
			return origin;
		}
		// Not inside any repository. Deliberately NOT cached: an arbitrary
		// directory can have no repo above it while one of its subdirectories
		// is an independent repository, so a negative entry would leak
		// incorrectly to deeper directories.
		return String();
	};

	auto PrefixOf = [&](String origin) {
		if(!origin.GetCount())
			return String("./");
		while(origin.EndsWith(".git")) origin.TrimEnd(".git");
		while(origin.EndsWith("/"))   origin.TrimEnd("/");
		int q = max(origin.ReverseFind('/'), origin.ReverseFind(':'));
		return origin.Mid(q + 1) + "/";
	};

	auto PkgName = [&](const String& s) {
	    String n = Filter(s, [](int c) { return c == '\\' ? '/' : c; });
	    int q = repo_prefix.Find(n);
	    if(q < 0) {
	        bool in_ws = false;
	        for(int wi = 0; wi < wspc.GetCount(); wi++)
	            if(wspc[wi] == n) { in_ws = true; break; }
	        String prefix = in_ws ? PrefixOf(OriginOf(PackageDirectory(n))) : String("./");
	        q = repo_prefix.GetCount();
	        repo_prefix.Add(n, prefix);
	    }
	    return repo_prefix[q] + n;
	};
	
/*	VectorMap<String, String> repo_prefix;

	auto OriginOf = [&](const String& dir) {
		String origin = TrimBoth(Sys(git + " -C " + dir + " config --get remote.origin.url"));
		if(origin.StartsWith("git@github.com:"))
			origin.Replace("git@github.com:", "https://github.com/");
		return origin;
	};

	auto PrefixOf = [&](String origin) {
		if(!origin.GetCount())
			return String("./");
		while(origin.EndsWith(".git")) origin.TrimEnd(".git");
		while(origin.EndsWith("/"))   origin.TrimEnd("/");
		int q = max(origin.ReverseFind('/'), origin.ReverseFind(':'));
		return origin.Mid(q + 1) + "/";
	};

	auto PkgName = [&](const String& s) {
		int q = repo_prefix.Find(s);
		if(q < 0) {
			bool in_ws = false;
			for(int wi = 0; wi < wspc.GetCount(); wi++)
				if(wspc[wi] == s) { in_ws = true; break; }
			String prefix = in_ws ? PrefixOf(OriginOf(PackageDirectory(s))) : String("./");
			q = repo_prefix.GetCount();
			repo_prefix.Add(s, prefix);
		}
		return repo_prefix[q] + Filter(s, [](int c) { return c == '\\' ? '/' : c; });
	};
*/
	int done = 0;
	int total = wspc.GetCount();

	auto Tick = [&]() -> bool {
		if(done >= total)
			total = done + 1;
		bool cancel = progress(done, total);
		done++;
		return cancel;
	};

	// ------------------------------------------------------------------
	// Fragment reader — now after own_ci is declared, so edges can reference it.
	// Reads one optional sbom.json per package (listed in its file set).
	// Component is registered through CompIdxExt; an edge from the OWNING
	// package to the component is added automatically.
	// ------------------------------------------------------------------
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

	// Step 2 — per-package gather
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

			String origin = OriginOf(PackageDirectory(n));
			repo_prefix.GetAdd(n, PrefixOf(origin));
			m.bom_ref = m.name = PkgName(n);
			if(i == 0)
				m.type = "application";

			if(origin.GetCount()) {
				m.originUrl = "git+" + origin;
				origin.TrimEnd(".git");
				m.homepage = origin;
			}
			String ts, hash, sha1;
			if(SplitTo(TrimBoth(Sys(git + " -C " + PackageDirectory(n) + " log -1 --date=unix --format=\"%h %H %cd\"")), " ", hash, sha1, ts)) {
				m.version = Format8601Z(Atoi64(ts) + Time(1970, 1, 1)) + "#" + hash;
				if(m.originUrl.GetCount())
					m.sourceDistributions << MakeTuple<String, String, String>(m.originUrl + "@" + hash, "SHA-1", sha1);
			}
			m.licenses << Nvl(pk.license_id, "BSD-2-Clause");

			for(const OptItem& u : pk.uses)
				AddDependency(m, PkgName(u.text));
		}

		// sbom.json fragments
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

		// Library declarations — SKIPPED for win_target
		if(!win_target) {
			Index<String> cfg = PackageConfig(wspc, i, bm, mainconfigparam, host, *b);

			auto GatherNames = [&](const Array<OptItem>& set, int decl) {
				for(String s : Split(Gather(set, cfg.GetKeys()), ' '))
					if(s.GetCount())
						decls.Add(Decl(decl, s, i));
			};
			GatherNames(pk.static_library, DECL_STATIC_LIBRARY);
			GatherNames(pk.dynamic_library, DECL_DYNAMIC_LIBRARY);
			GatherNames(pk.library, DECL_LIBRARY);

			for(String mod : Split(Gather(pk.pkg_config, cfg.GetKeys()), ' '))
				for(String s : Split(b->EvalCmdX("pkg-config --libs " + mod), CharFilterWhitespace))
					if(s.TrimStart("-l") && s.GetCount())
						decls.Add(Decl(DECL_LIBRARY, s, i));
		}

		for(auto s : RequiredExternalDependenciesInfo(pk)) {
			String nm = s.name;
			int q = nm.Find('|');
			if(q >= 0) {
				nm.Clear();
				for(String a : Split(s.name, '|')) {
					a = TrimBoth(a);
					if(a.GetCount() && (nm.IsEmpty() || all_host.Find(a) >= 0))
						nm = a;
				}
			}
			if(!nm.GetCount())
				continue;
			AddDependency(cs[own_ci[i]], nm);
			decls.Add(Decl(DECL_EXTERNAL_DEPENDENCY, nm, i));
			if(s.license.GetCount())
				override_licenses.GetAdd(nm) = s.license;
		}
	}

	total = done + decls.GetCount();

	// Step 3 — resolution + tier assignment
	// For win_target only DECL_EXTERNAL_DEPENDENCY decls exist.
	Index<String> so_files;  // Placeholder for POSIX NEEDED closure
	for(const Decl& d : decls) {
		if(Tick()) {
			cs.Clear();
			return cs;
		}
		int tier;
		String nm = d.name;
		

		if(d.decl == DECL_EXTERNAL_DEPENDENCY)
		    tier = (win_target && IsVCPKG()) ? VcpkgTier(link_mode) : T_STATIC_THIRD_PARTY;

		else {
			String ext = ToLower(GetFileExt(d.name));
			bool dynq = ext == ".so" || ext == ".dll"
			         || d.decl == DECL_DYNAMIC_LIBRARY
			         || d.decl == DECL_LIBRARY && link_mode == LINK_DYNAMIC;
			String file, owner, sta;
			if(dynq) {
				file = DynOwnerFile(d.name);
				owner = OwnerOfFile(file);
			}
			else
				sta = StaticOwner(d.name, cc);
			tier = DeclTier(d.decl, d.name, link_mode, DynTierOf(owner));
			nm = tier == T_DYNAMIC_PLATFORM ? owner
			  : tier == T_STATIC_THIRD_PARTY && sta.GetCount() ? sta
			  : d.name;
			// so_files tracking for POSIX NEEDED closure (not used on Windows)
		}
		bool fresh;
		int c = CompIdxExt(nm, fresh);
		if(fresh) {
			SBOMComponent& m = cs.Add();
			m.bom_ref = m.name = nm;
			m.external = tier;
			
#ifdef PLATFORM_POSIX
			if(!win_target && (IsRPM() || IsDPKG())
			   && (tier == T_STATIC_THIRD_PARTY || tier == T_DYNAMIC_PLATFORM))
			    FillPkgMeta(m);
#endif
			if(win_target && tier == T_DYNAMIC_BUNDLED)
			    FillFromVcpkgList(m, nm, triplet);

		}
		if(own_ci[d.pk] >= 0)
			AddDependency(cs[own_ci[d.pk]], nm);
	}

	// Windows platform component — UNCONDITIONAL
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

	if(win_target && IsVCPKG()) {
		Index<String> vnames;
		for(const Decl& d : decls)
			if(d.decl == DECL_EXTERNAL_DEPENDENCY)
				vnames.FindAdd(d.name);
		total = done + vnames.GetCount();
		for(int i = 0; i < vnames.GetCount(); i++) {
			if(Tick()) {
				cs.Clear();
				return cs;
			}
			String name = vnames[i];
			Value spdx = ParseJSON(LoadFile(
				GetExeDirFile("vcpkg") + "/installed/" + triplet + "/share/" + name + "/vcpkg.spdx.json"));
			for(Value p : spdx["packages"])
				if(p["SPDXID"] == "SPDXRef-port") {
					int q = ci_ext.Get(name, -1);
					if(q < 0)
						break;
					SBOMComponent& m = cs[q];
					m.version = p["versionInfo"];
					String lic = p["licenseConcluded"];
					if(!IsNull(lic)) {
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
							int c = CompIdxExt(dep, fresh);
							if(fresh) {
								SBOMComponent& mc = cs.Add();
								mc.bom_ref = mc.name = dep;
								mc.external = m.external;
								FillFromVcpkgList(mc, dep, triplet);
							}
							int fq = ci_ext.Get(name, -1);
							if(fq >= 0)
								AddDependency(cs[fq], dep);
							if(vnames.Find(dep) < 0) {
								vnames.Add(dep);
								total++;
							}
						}
					}
					for(Value p2 : spdx["packages"]) {
						String id = p2["SPDXID"];
						if(id.StartsWith("SPDXRef-resource-")) {
							String url = p2["downloadLocation"];
							if(!IsNull(url) && url != "NONE")
								for(Value ch : p2["checksums"])
									cs[q].sourceDistributions
									  << MakeTuple<String, String, String>(~url, ~ch["algorithm"], ~ch["checksumValue"]);
						}
					}
					break;
				}
		}
	}

	// MSVC runtime / UCRT
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
	// llvm-mingw: statically linked C/C++ runtime stack.
	// Mirrors the MSVC vcruntime entry for the clang toolchain. Gate on the
	// builder == "CLANG" so a plain MinGW-GCC build does not get llvm components
	// (a GCC build would want libstdc++/libgcc/winpthread instead).
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
			}
			if(own_ci[0] >= 0)
				AddDependency(cs[own_ci[0]], r.ref);
		}
	}

#ifdef PLATFORM_POSIX
	if(!win_target){
		// Step 4 — T4: NEEDED closure (POSIX builds only)
		Vector<Tuple<String, String>> so_files_list;
		Index<String> walked;
		for(const Decl& d : decls) {
			// Re-populate so_files_list from the original declaration data
			String ext = ToLower(GetFileExt(d.name));
			bool dynq = ext == ".so" || ext == ".dll"
			         || d.decl == DECL_DYNAMIC_LIBRARY
			         || d.decl == DECL_LIBRARY && link_mode == LINK_DYNAMIC;
			if(dynq && d.decl != DECL_EXTERNAL_DEPENDENCY) {
				String file = DynOwnerFile(d.name);
				if(file.GetCount()) {
					String owner = OwnerOfFile(file);
					String nm = owner.GetCount() ? owner : d.name;
					so_files_list.Add(MakeTuple(file, nm));
				}
			}
		}
	
		total = done + so_files_list.GetCount();
		for(int i = 0; i < so_files_list.GetCount(); i++) {
			if(Tick()) {
				cs.Clear();
				return cs;
			}
			String file = so_files_list[i].a, from = so_files_list[i].b;
			if(!file.GetCount() || walked.Find(file) >= 0)
				continue;
			walked.Add(file);
			for(String l : Split(Sys("readelf -d " + file), '\n')) {
				if(l.Find("(NEEDED)") < 0)
					continue;
				int q1 = l.Find('['), q2 = l.Find(']', q1);
				if(q1 < 0 || q2 <= q1)
					continue;
				String soname = l.Mid(q1 + 1, q2 - q1 - 1);
				String nfile = RealFile(FindSonameFile(soname));
				String owner = OwnerOfFile(nfile);
				String nm = owner.GetCount() ? owner : soname;
				bool fresh;
				int c = CompIdxExt(nm, fresh);
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
				if(nfile.GetCount() && walked.Find(nfile) < 0) {
					so_files_list.Add(MakeTuple(nfile, nm));
					total++;
				}
			}
		}
	
		// Step 5 — toolchain baseline (POSIX only)
		if(!msvc) {
			if(main_cfg.Find("PORTABLE_HYBRID") >= 0) {
				total = done + 1;
				if(Tick()) {
					cs.Clear();
					return cs;
				}
				String nm = IsRPM() ? "glibc" : "libc6";
				bool fresh;
				int c = CompIdxExt(nm, fresh);
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
			else
			if(link_mode == LINK_DYNAMIC) {
				total = done + 4;
				for(const char *rt : {"c", "m", "stdc++", "gcc_s"}) {
					if(Tick()) {
						cs.Clear();
						return cs;
					}
					String owner = OwnerOfFile(DynOwnerFile(rt));
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
					if(own_ci[0] >= 0)
						AddDependency(cs[own_ci[0]], nm);
				}
			}
		}
	}
#endif
	for(SBOMComponent& c : cs) {
		String ol = override_licenses.Get(c.name, Null);
		if(ol.GetCount()) {
			c.licenses.Clear();
			c.licenses << ol;
		}
	}

	return cs;
}

String MakeBuild::CreateSBOM(const String& triplet, int mode)
{
	return CreateSBOM(CreateSBOMComponents(triplet), mode);
}

// ============================================================================
// Emission (Plan §6)
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
			continue;  // skip malformed components

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
			if(!IsNull(s))
				EmitLicense(licenses, s);
		if(licenses)
			component("licenses", licenses);

		JsonArray extRefs;
		if(!IsNull(c.homepage))
			extRefs << Json("type", "website")("url", c.homepage);
		if(!IsNull(c.originUrl))
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
			if(!IsNull(s.a))
				extRefs << SourceRef(s);

		if(extRefs)
			component("externalReferences", extRefs);
		if(!IsNull(c.purl))
			component("purl", c.purl);

		// An operating system is never "bundled by us" — exempt the OS
		// component from the CONTAINER tier clamp.
		int tier_value = c.type == "operating-system"
			? c.external
			: clamp(c.external, (int)T_STATIC_IN_REPO, tier_clamp);
		component("properties", JsonArray()
			<< Json("name", "upp:deployment-mode")("value", sbom_tier[tier_value]));

//		component("properties", JsonArray()
//			<< Json("name", "upp:deployment-mode")("value", sbom_tier[clamp(c.external, (int)T_STATIC_IN_REPO, tier_clamp)]));

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
