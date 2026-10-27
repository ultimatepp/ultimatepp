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

	String id, id_like, ubuntu_codename;
	for(String l : Split(LoadFile("/etc/os-release"), '\n')) {
		l = TrimBoth(l);
		if(l.TrimStart("ID="))
			id = TrimBoth(l);
		else
		if(l.TrimStart("ID_LIKE="))
			id_like = TrimBoth(l);
		else
		if(l.TrimStart("UBUNTU_CODENAME="))
			ubuntu_codename = TrimBoth(l);
	}
	id.TrimStart("\"");       id.TrimEnd("\"");
	id_like.TrimStart("\"");  id_like.TrimEnd("\"");

	if(id == "ubuntu" || id_like.Find("ubuntu") >= 0)
		return cached = "ubuntu";

	if((id == "linuxmint" || id == "mint") && ubuntu_codename.GetCount())
		return cached = "ubuntu";

	return cached = "debian";
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

// ============================================================================
// Win32 system import libraries.
//
// Extensionless LIBS / dynamic_library entries matching this table are
// provided by the OS image itself. They are intercepted BEFORE generic
// resolution and folded into a single "Microsoft Windows" operating-system
// component: their CVE exposure rolls up to OS-build-level MSRC bulletins,
// no purl addresses them individually, and their versions track the target
// OS build — per-DLL components would add noise, not queryability.
//
// Keep alphabetical. An unlisted system lib fails visibly as a mute stub
// rather than silently vanishing — extend the table when that happens.
// (Explicitly suffixed names like "kernel32.lib" are NOT intercepted.)
// ============================================================================
static const Index<String>& WinSystemLibs()
{
	static Index<String> l;
	if(l.IsEmpty()) {
		static const char *s[] = {
			"advapi32", "bcrypt", "comctl32", "comdlg32", "crypt32", "dbghelp",
			"dwmapi", "gdi32", "gdiplus", "glu32", "imm32", "iphlpapi",
			"kernel32", "mpr", "msimg32", "mswsock", "netapi32", "normaliz",
			"ntdll", "ole32", "oleaut32", "opengl32", "powrprof", "psapi",
			"rpcrt4", "secur32", "setupapi", "shell32", "shlwapi", "user32",
			"userenv", "usp10", "uxtheme", "uuid", "version", "wininet",
			"winmm", "winspool", "wldap32", "ws2_32", "wsock32", "wtsapi32",
		};
		for(const char *p : s)
			l.Add(p);
	}
	return l;
}

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

	auto ReadComponent = [&](Value p) {
		SBOMComponent& m = cs.Add();
		m.name = p["name"];
		m.version = p["versionInfo"];
		m.licenses << ~p["licenseConcluded"];
		m.homepage = p["homepage"];
		m.originUrl = p["downloadLocation"];
		for(Value r : p["externalRefs"])
			if(r["referenceType"] == "purl")
				m.purl = r["referenceLocator"];
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
	const bool win_target = true;
#else
	const bool win_target = main_cfg.Find("WIN32") >= 0; // POSIX cross build for Windows
#endif
	// Single decision points, consumed by GetVcpkgTriplet AND the CRT block
	// below, so triplet selection and SBOM CRT claims cannot diverge.
	bool msvc = bm.Get("BUILDER", "GCC").StartsWith("MSC");
	bool md   = mainconfigparam.Find("MD") >= 0;   // /MD vs /MT

	Index<String> all_host = AllHostPackagesList();
	VectorMap<String, String> override_licenses;

	Vector<Decl> decls;
	Vector<int>  own_ci(wspc.GetCount(), -1);  // package idx -> first-party component idx

#ifdef PLATFORM_WIN32
	String git = GetExeDirFile("bin/mingit/cmd/git");
#else
	String git = "git";
#endif

	// --- First-party repo prefixes --------------------------------------------
	// bom_ref prefix = owning git repository's name ("ultimatepp/", "myrepo/"),
	// from the package directory's origin URL; "./" when the directory is not
	// under any git checkout (codebase-local ownership). Cached per package so
	// 'uses' edges and component emission always agree on the same prefix.
	VectorMap<String, String> repo_prefix;

	auto OriginOf = [&](const String& dir) {
		String origin = TrimBoth(Sys(git + " -C " + dir + " config --get remote.origin.url"));
		if(origin.StartsWith("git@github.com:"))
			origin.Replace("git@github.com:", "https://github.com/");
		return origin;
	};

	auto PrefixOf = [&](String origin) {
		if(!origin.GetCount())
			return String("./");                    // local code, no git repo
		while(origin.EndsWith(".git")) origin.TrimEnd(".git");
		while(origin.EndsWith("/"))   origin.TrimEnd("/");
		// last path element; also cut scp-style "git@host:repo" at the colon
		int q = max(origin.ReverseFind('/'), origin.ReverseFind(':'));
		return origin.Mid(q + 1) + "/";
	};

	// Resolve the repo prefix of ANY workspace package name, on demand.
	// Unknown names (stray 'uses' targets) fall back to "./".
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

	// --- Progress bookkeeping ---------------------------------------------------
	// One monotonic 'done' counter spans every phase; 'total' starts at the
	// package count and grows as work is discovered (declaration count,
	// vcpkg depend-info expansion, NEEDED closure). The bar may briefly move
	// backwards when a phase discovers new work, but it never stalls and
	// never overshoots.
	int done = 0;
	int total = wspc.GetCount();

	auto Tick = [&]() -> bool {
		if(done >= total)
			total = done + 1;
		bool cancel = progress(done, total);
		done++;
		return cancel;
	};

	// Step 2 — per-package gather
	for(int i = 0; i < wspc.GetCount(); i++) {
		if(Tick()) {
			cs.Clear();
			return cs;
		}
		const Package& pk = wspc.GetPackage(i);
		String n = wspc[i];

		for(String fn : pk.file) {
			String file = SourcePath(n, fn);
			if(ToLower(GetFileName(file)) == "sbom.json") {
				Value sbom = ParseJSON(LoadFile(file));
				if(sbom.Is<ValueArray>())
					for(Value p : sbom)
						ReadComponent(p);
				else
					ReadComponent(sbom);
			}
		}

		// FIRST-PARTY COMPONENT (T0) — create this BEFORE any external deps
		{
			SBOMComponent& m = cs.Add();
			own_ci[i] = cs.GetCount() - 1;

			String origin = OriginOf(PackageDirectory(n));
			repo_prefix.GetAdd(n, PrefixOf(origin));  // prime cache — one git call
			m.bom_ref = m.name = PkgName(n);          // cache hit, no extra call
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

	// Step 3 — resolution + tier assignment (two disjoint name streams)
	Index<String> seen_ext;           // only for EXTERNAL components
	VectorMap<String, int> ci_ext;    // external name -> component index

	auto CompIdxExt = [&](const String& nm, bool& fresh) -> int {
		if(seen_ext.Find(nm) >= 0) {
			fresh = false;
			return ci_ext.Get(nm, -1);
		}
		seen_ext.Add(nm);
		ci_ext.Add(nm, cs.GetCount());
		fresh = true;
		return cs.GetCount();  // will be GetCount()-1 after Add()
	};

	// Single OS component standing in for all Win32 system DLLs (see
	// WinSystemLibs). Created lazily through CompIdxExt so edges, the
	// seen/ci bookkeeping and dependency emission need no special cases.
	auto WindowsOs = [&]() -> String {
		static const char *REF = "com.microsoft.windows";
		bool fresh;
		CompIdxExt(REF, fresh);
		if(fresh) {
			SBOMComponent& m = cs.Add();
			m.type     = "operating-system";
			m.bom_ref  = REF;
			m.name     = "Microsoft Windows";
			m.external = T_DYNAMIC_PLATFORM;
			// version: deliberately absent — DLL/CRT versions track the
			// TARGET OS build, which cannot be derived from the build host.
			// (Revisit if _WIN32_WINNT targeting becomes queryable.)
		}
		return REF;
	};

#ifdef PLATFORM_POSIX
	// RPM purl reference (OSV-proven, Rocky/xz-devel):
	//   pkg:rpm/rocky/xz-devel@5.2.5-8.el9_0?arch=x86_64&source=xz-5.2.5-8.el9_0.src.rpm
	// SUSE variant (also OSV-proven) additionally carries distro=sle15-sp5.
	// DEB purl reference (OSV-proven, Ubuntu/zlib1g):
	//   pkg:deb/ubuntu/zlib1g@1:1.3.dfsg-3.1ubuntu2.2?arch=amd64&source=zlib
	auto FillPkgMeta = [&](SBOMComponent& m) {
		if(IsRPM()) {
			// One rpm -q call: version, url, arch, license, source rpm.
			// A failed lookup writes to stderr only, so empty version == not a
			// PM package after all; the record keeps its name and no junk purl.
			String url, arch, lic, srcrpm, out = Sys(
				"rpm -q --qf '%{VERSION}-%{RELEASE}\n%{URL}\n%{ARCH}\n%{LICENSE}\n%{SOURCERPM}\n' "
				+ m.name);
			SplitTo(out, '\n', false, m.version, url, arch, lic, srcrpm);
			if(!m.version.GetCount()) {
				m.homepage = String();
				return;
			}
			m.homepage = TrimBoth(url);
			if(lic.GetCount() && lic != "(none)")
				m.licenses << TrimBoth(lic);

			// purl namespace (and SUSE-only distro=), resolved once from os-release
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

			srcrpm = TrimBoth(srcrpm);
			m.purl << "pkg:rpm/" << ns << '/' << m.name << '@' << m.version;
			String quals;
			if(arch.GetCount() && arch != "(none)")
				quals << "arch=" << TrimBoth(arch);
			if(distro.GetCount())
				quals << (quals.GetCount() ? '&' : '?') << "distro=" << distro;
			if(srcrpm.GetCount() && srcrpm != "(none)")
				quals << (quals.GetCount() ? '&' : '?') << "source=" << srcrpm;
			if(quals.GetCount())
				m.purl << quals;
			return;
		}

		// DEB — OSV indexes deb advisories by SOURCE package; without a source=
		// qualifier queries never match. Two dpkg quirks to absorb:
		//  - Source is omitted when binary name == source name  -> use name
		//  - older dpkg emits "src (epoch:ver)"                   -> trim "( ..."
		String arch, src, out = Sys(
			"dpkg-query -W -f='${Version}\n${Homepage}\n${Architecture}\n${Source}\n' " + m.name);
		SplitTo(out, '\n', false, m.version, m.homepage, arch, src);
		if(!m.version.GetCount()) {     // not a PM package after all —
			m.homepage = String();       // record keeps name only, no junk purl
			return;
		}
		String source = TrimBoth(src);
		int pq = source.Find(" (");
		if(pq >= 0)
			source.Trim(pq);
		if(!source.GetCount())
			source = m.name;

		m.purl << "pkg:deb/" << DebNamespace() << '/' << m.name << '@' << m.version;
		String quals;
		if(arch.GetCount())                              // never emit bare "?arch="
			quals << "?arch=" << TrimBoth(arch);
		quals << (quals.GetCount() ? '&' : '?') << "source=" << source;
		m.purl << quals;

		FileIn in("/usr/share/doc/" + m.name + "/copyright");
		while(!in.IsEof()) {
			String l = in.GetLine();
			if(l.TrimStart("License: ")) {
				m.licenses << l;
				break;
			}
		}
	};
#endif

	Vector<Tuple<String, String>> so_files;

	for(const Decl& d : decls) {
		if(Tick()) {
			cs.Clear();
			return cs;
		}
		int tier;
		String nm = d.name;
		if(win_target && GetFileExt(d.name).IsEmpty()
		&& WinSystemLibs().Find(ToLower(d.name)) >= 0) {
			// Win32 system import library — folded into the OS component,
			// no per-DLL record (see WinSystemLibs).
			if(own_ci[d.pk] >= 0)
				AddDependency(cs[own_ci[d.pk]], WindowsOs());
			continue;
		}
		if(d.decl == DECL_EXTERNAL_DEPENDENCY) {
#ifdef PLATFORM_WIN32
			tier = IsVCPKG() ? VcpkgTier(link_mode) : T_STATIC_THIRD_PARTY;
#else
			tier = T_STATIC_THIRD_PARTY;
#endif
		}
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
			if(dynq && file.GetCount() && (tier == T_DYNAMIC_BUNDLED || tier == T_DYNAMIC_PLATFORM))
				so_files.Add(MakeTuple(file, nm));
		}
		bool fresh;
		int c = CompIdxExt(nm, fresh);
		if(fresh) {
			SBOMComponent& m = cs.Add();
			m.bom_ref = m.name = nm;
			m.external = tier;
#ifdef PLATFORM_POSIX
			if((IsRPM() || IsDPKG()) && (tier == T_STATIC_THIRD_PARTY || tier == T_DYNAMIC_PLATFORM))
				FillPkgMeta(m);
#endif
#ifdef PLATFORM_WIN32
			if(tier == T_DYNAMIC_BUNDLED)
				FillFromVcpkgList(m, nm, triplet);
#endif
		}
		if(own_ci[d.pk] >= 0)
			AddDependency(cs[own_ci[d.pk]], nm);
	}

#ifdef PLATFORM_WIN32
	if(IsVCPKG()) {
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
								total++;   // depend-info expanded the work set
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
#endif

	// ------------------------------------------------------------------
	// MSVC runtime / UCRT
	//
	// UCRT: part of the OS on Win10+ (targeted via _WIN32_WINNT 0x0A00),
	// so com.microsoft.windows covers it for BOTH toolchains. No entry.
	//
	// VC runtime: MSVC only; llvm-mingw links UCRT and gets nothing.
	//   /MD (or dynamic link): vcruntime140.dll/msvcp140.dll supplied by
	//     the target environment -> dynamic-platform, FULL only.
	//   /MT: CRT object code baked into the binaries -> static-third-party,
	//     included in BASE like the static vcpkg libraries.
	// ------------------------------------------------------------------
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

#ifdef PLATFORM_POSIX
	// Step 4 — T4: NEEDED closure
	Index<String> walked;
	total = done + so_files.GetCount();
	for(int i = 0; i < so_files.GetCount(); i++) {
		if(Tick()) {
			cs.Clear();
			return cs;
		}
		String file = so_files[i].a, from = so_files[i].b;
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
				so_files.Add(MakeTuple(nfile, nm));
				total++;   // the closure discovered new work
			}
		}
	}

	// Step 5 — toolchain baseline
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
		// Win32 cross builds: advapi32 / ws2_32 and friends now arrive as
		// declarations and are intercepted by WinSystemLibs — the former
		// manual injection here is gone.
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
