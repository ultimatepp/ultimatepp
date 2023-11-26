#include "Builders.h"
#include "AndroidBuilder.h"

#include <plugin/bz2/bz2.h>

#define METHOD_NAME "MakeBuild::" << UPP_FUNCTION_NAME << "(): "

#define LDUMP(x) // DUMP(x)

MakeBuild::MakeBuild()
{
	targetmode = 0;
	stoponerrors = true;
	use_target = false;
}

const TargetMode& MakeBuild::GetTargetMode()
{
	return (targetmode == 0 ? debug : release);
}

Index<String> MakeBuild::PackageConfig(const Workspace& wspc, int package,
                                 const VectorMap<String, String>& bm, String mainparam,
                                 Host& host, Builder& b, String *target)
{
	String packagepath = PackagePath(wspc[package]);
	const Package& pkg = wspc.package[package];
	cfg.Clear();
	MergeWith(mainparam, " ", bm.Get(targetmode ? "RELEASE_FLAGS" : "DEBUG_FLAGS", String()),
	                          bm.Get("COMMON_FLAGS", String()));
	cfg = SplitFlags(mainparam, package == 0, wspc.GetAllAccepts(package));
	cfg.FindAdd(bm.Get("BUILDER", "GCC"));
	const TargetMode& m = GetTargetMode();
	if(targetmode == 0)
		cfg.FindAdd("DEBUG");
	switch(m.linkmode) {
	case 2:
		cfg.FindAdd("SO");
	case 1:
		cfg.FindAdd("SHARED");
	}
	int q = m.package.Find(wspc[package]);
	if(q >= 0) {
		const PackageMode& p = m.package[q];
		switch(p.debug >= 0 ? p.debug : m.def.debug) {
		case 1:  cfg.FindAdd("DEBUG_MINIMAL"); break;
		case 2:  cfg.FindAdd("DEBUG_FULL"); break;
		}
		if(!pkg.noblitz && (p.blitz >= 0 ? p.blitz : m.def.blitz) && bm.Get("DISABLE_BLITZ", "") != "1")
			cfg.FindAdd("BLITZ");
	}
	else {
		switch(m.def.debug) {
		case 1:  cfg.FindAdd("DEBUG_MINIMAL"); break;
		case 2:  cfg.FindAdd("DEBUG_FULL"); break;
		}
		if(!pkg.noblitz && m.def.blitz && bm.Get("DISABLE_BLITZ", "") != "1")
			cfg.FindAdd("BLITZ");
	}
	b.AddFlags(cfg);
	host.AddFlags(cfg);
	for(int i = 0; i < pkg.flag.GetCount(); i++) {
		if(MatchWhen(pkg.flag[i].when, cfg.GetKeys())) {
			Vector<String> h = Split(pkg.flag[i].text, ' ');
			for(int i = 0; i < h.GetCount(); i++)
				if(*h[i] == '-')
					cfg.RemoveKey(h[i].Mid(1));
				else
					cfg.FindAdd(h[i]);
		}
	}
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pk = wspc.package[i];
		for(int j = 0; j < pk.GetCount(); j++)
			if(pk[j] == "main.conf")
				cfg.FindAdd(Filter(wspc.package.GetKey(i), [](int c) { return iscid(c) ? c : '_'; }) + "_conf");
	}
	Index<String> h;
	h = clone(cfg); // Retain deep copy (h will be picked)
	return h;
}

String NoCr(const char *s)
{
	String out;
	while(*s)
	{
		const char *b = s;
		while(*s && *s != '\r')
			s++;
		out.Cat(b, int(s - b));
		if(*s == '\r')
			s++;
	}
	return out;
}

void MakeBuild::CreateHost(Host& host, const String& method, bool darkmode, bool disable_uhd)
{
	VectorMap<String, String> bm = GetMethodVars(method);
	{
		VectorMap<String, String> env = clone(Environment());
		host.exedirs = SplitDirs(bm.Get("PATH", "") + ';' + env.Get("PATH", ""));
#ifdef PLATFORM_WIN32
		host.AddExecutable(GetExeDirFile("bin/mingit/cmd"), "git.exe");
		host.AddExecutable(GetExeDirFile("bin/llvm/bin"), "clang-format.exe");
		
		env.GetAdd("PATH") = Join(host.exedirs, ";");
#else
		env.GetAdd("PATH") = Join(host.exedirs, ":");
#endif
		env.GetAdd("UPP_MAIN__") = GetFileDirectory(PackagePath(GetMain()));
		env.GetAdd("UPP_ASSEMBLY__") = GetVar("UPP");
		if(disable_uhd)
			env.GetAdd("UPP_DISABLE_UHD__") = "1";
		if(darkmode)
			env.GetAdd("UPP_DARKMODE__") = "1";
		// setup LD_LIBRARY_PATH on target dir, needed for all shared builds on posix
#ifdef PLATFORM_POSIX
		if(target != "")
		{
			String ldPath = GetFileFolder(target) + ";" + env.Get("LD_LIBRARY_PATH", "");
			env.GetAdd("LD_LIBRARY_PATH") = ldPath;
		}
#endif
#ifdef PLATFORM_COCOA
		host.exedirs.Append(SplitDirs("/opt/local/bin:/opt/local/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/opt/homebrew/bin:/opt/homebrew/sbin")); // sometimes some of these are missing..
#endif
		
		if (IsAndroidMethod(method)) {
			auto jdkPath = bm.Get("JDK_PATH", "");
			if (!jdkPath.IsEmpty()) {
				Cout() << "JDK Path: " << jdkPath << "\n";
				env.GetAdd("JAVA_HOME") = jdkPath;
			}
		}
		
		for(int i = 0; i < env.GetCount(); i++) {
			LDUMP(env.GetKey(i));
			LDUMP(env[i]);
			host.environment << env.GetKey(i) << '=' << env[i] << '\0';
		}
		
		host.environment.Cat(0);
		host.cmdout = &cmdout;
	}
}

void MakeBuild::CreateHost(Host& host, bool darkmode, bool disable_uhd)
{
	SetupDefaultMethod();
	CreateHost(host, method, darkmode, disable_uhd);
}

One<Builder> MakeBuild::CreateBuilder(Host *host)
{
	SetupDefaultMethod();
	VectorMap<String, String> bm = GetMethodVars(method);
	String builder = bm.Get("BUILDER", "GCC");
	int q = BuilderMap().Find(builder);
	if(q < 0) {
		PutConsole("Invalid builder " + builder);
		ConsoleShow();
		return nullptr;
	}
	Builder* b = (*BuilderMap().Get(builder))();
	b->host = host;
	b->script = bm.Get("SCRIPT", "");
	if(AndroidBuilder::GetBuildersNames().Find(builder) > -1) {
		AndroidBuilder* pAb = dynamic_cast<AndroidBuilder*>(b);
		if (!pAb) {
			Loge() << METHOD_NAME << "Converting builder to android builder failed.";
			return nullptr;
		}
		pAb->sdk.SetPath((bm.Get("SDK_PATH", "")));
		pAb->ndk.SetPath((bm.Get("NDK_PATH", "")));
		pAb->SetJdk(One<Jdk>(new Jdk(bm.Get("JDK_PATH", ""), host)));
		
		String platformVersion = bm.Get("SDK_PLATFORM_VERSION", "");
		if(!platformVersion.IsEmpty())
			pAb->sdk.SetPlatform(platformVersion);
		else
			pAb->sdk.DeducePlatform();
		String buildToolsRelease = bm.Get("SDK_BUILD_TOOLS_RELEASE", "");
		if(!buildToolsRelease.IsEmpty())
			pAb->sdk.SetBuildToolsRelease(buildToolsRelease);
		else
			pAb->sdk.DeduceBuildToolsRelease();
		
		pAb->ndk_blitz = bm.Get("NDK_BLITZ", "") == "1";
		if(bm.Get("NDK_ARCH_ARMEABI_V7A", "") == "1")
			pAb->ndkArchitectures.Add("armeabi-v7a");
		if(bm.Get("NDK_ARCH_ARM64_V8A", "") == "1")
			pAb->ndkArchitectures.Add("arm64-v8a");
		if(bm.Get("NDK_ARCH_X86", "") == "1")
			pAb->ndkArchitectures.Add("x86");
		if(bm.Get("NDK_ARCH_X86_64", "") == "1")
			pAb->ndkArchitectures.Add("x86_64");
		pAb->ndkToolchain = bm.Get("NDK_TOOLCHAIN", "");
		pAb->ndkCppRuntime = bm.Get("NDK_CPP_RUNTIME", "");
		pAb->ndkCppFlags = bm.Get("NDK_COMMON_CPP_OPTIONS", "");
		pAb->ndkCFlags = bm.Get("NDK_COMMON_C_OPTIONS", "");
		
		b = pAb;
	}
	else {
		// TODO: cpp builder variables only!!!
		b->compiler = bm.Get("COMPILER", "");
		b->include = SplitDirs(Join(GetUppDirs(), ";") + ';' + bm.Get("INCLUDE", "") + ';' + add_includes);
		const Workspace& wspc = GetIdeWorkspace();
		for(int i = 0; i < wspc.GetCount(); i++) {
			const Package& pkg = wspc.GetPackage(i);
			for(int j = 0; j < pkg.include.GetCount(); j++)
				b->include.Add(SourcePath(wspc[i], pkg.include[j].text));
		}
		b->libpath = SplitDirs(bm.Get("LIB", ""));
		b->cpp_options = bm.Get("COMMON_CPP_OPTIONS", "");
		b->c_options = bm.Get("COMMON_C_OPTIONS", "");
		b->debug_options = Join(bm.Get("COMMON_OPTIONS", ""), bm.Get("DEBUG_OPTIONS", ""));
		b->release_options = Join(bm.Get("COMMON_OPTIONS", ""), bm.Get("RELEASE_OPTIONS", ""));
		b->common_link = bm.Get("COMMON_LINK", "");
		b->debug_link = bm.Get("DEBUG_LINK", "");
		b->release_link = bm.Get("RELEASE_LINK", "");
		
		b->main_conf = !!main_conf.GetCount();
		b->allow_pch = bm.Get("ALLOW_PRECOMPILED_HEADERS", "") == "1";
		b->start_time = start_time;
	}
	return b;
}

int CharFilterSlash(int c)
{
	return c == '\\' ? '/' : c;
}

bool output_per_assembly;

String MakeBuild::OutDir(const Index<String>& cfg, const String& package, const VectorMap<String, String>& bm,
                   bool use_target)
{
	Index<String> excl;
	excl.Add(bm.Get("BUILDER", "GCC"));
	excl.Add("MSC");
	Host().AddFlags(excl);
	Vector<String> x;
	bool dbg = cfg.Find("DEBUG_FULL") >= 0 || cfg.Find("DEBUG_MINIMAL") >= 0;
	if(cfg.Find("DEBUG") >= 0) {
		excl.Add("BLITZ");
		if(cfg.Find("BLITZ") < 0)
			x.Add("NOBLITZ");
	}
	else
		if(dbg)
			x.Add("RELEASE");
	if(use_target)
		excl.Add("MAIN");
	for(int i = 0; i < cfg.GetCount(); i++)
		if(excl.Find(cfg[i]) < 0)
			x.Add(cfg[i]);
	Sort(x);
	for(int i = 0; i < x.GetCount(); i++)
		x[i] = InitCaps(x[i]);
	String outdir = GetVar("OUTPUT");
	if(output_per_assembly)
		outdir = AppendFileName(outdir, GetAssemblyId());
	if(!use_target)
		outdir = AppendFileName(outdir, package);
	outdir = AppendFileName(outdir, GetFileTitle(method) + "." + Join(x, "."));
	outdir = Filter(outdir, CharFilterSlash);
	return outdir;
}

void MakeBuild::PkgConfig(const Workspace& wspc, const Index<String>& config, Index<String>& pkg_config)
{
	for(int i = 0; i < wspc.GetCount(); i++)
		for(String h : Split(Gather(wspc.GetPackage(i).pkg_config, config.GetKeys()), ' '))
			pkg_config.FindAdd(h);
}

String SaveMainConf(const String& main_conf)
{
	String main_conf_dir = CacheFile("main_conf_" + SHA1String(main_conf));
	RealizeDirectory(main_conf_dir);
	String path = AppendFileName(main_conf_dir, "main.conf.h");
	SaveChangedFile(path, main_conf);
	return path;
}

String MainConf(const Workspace& wspc, String& add_includes)
{
	String main_conf;
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pk = wspc.package[i];
		for(int j = 0; j < pk.GetCount(); j++)
			if(pk[j] == "main.conf") {
				String pn = wspc[i];
				String p = SourcePath(pn, "main.conf");
				main_conf << "// " << pn << "\r\n" << LoadFile(p) << "\r\n";
			}
	}

	if(main_conf.GetCount()) {
		String path = SaveMainConf(main_conf);
		MergeWith(add_includes, ";", GetFileFolder(path));
	}
	return main_conf;
}

void MakeBuild::MainConf(const Workspace& wspc)
{
	main_conf = ::MainConf(wspc, add_includes);
}

bool MakeBuild::BuildPackage(const Workspace& wspc, int pkindex, int pknumber, int pkcount,
	String mainparam, String outfile, Vector<String>& linkfile, Vector<String>& immfile,
	String& linkopt, bool link)
{
	String package = wspc[pkindex];
	String mainpackage = wspc[0];
	const Package& pkg = wspc.package[pkindex];
	VectorMap<String, String> bm = GetMethodVars(method);
	if(bm.GetCount() == 0) {
		PutConsole(GetInvalidBuildMethodError(method));
		ConsoleShow();
		return false;
	}
	Host host;
	CreateHost(host, false, false);
	One<Builder> b = CreateBuilder(&host);
	b->onefile = onefile;
	if(!b)
		return false;
	b->config = PackageConfig(wspc, pkindex, bm, mainparam, host, *b);
	PkgConfig(wspc, b->config, b->pkg_config);
	const TargetMode& m = targetmode == 0 ? debug : release;
	b->version = m.version;
	b->method = method;
	b->outdir = OutDir(b->config, package, bm);
	host.RealizeDir(b->outdir);
	String mainfn = Null;
	Index<String> mcfg = PackageConfig(wspc, 0, bm, mainparam, host, *b, &mainfn);
	HdependClearDependencies();
	for(int i = 0; i < pkg.GetCount(); i++) {
		const Array<OptItem>& f = pkg[i].depends;
		for(int j = 0; j < f.GetCount(); j++)
			if(MatchWhen(f[j].when, mcfg.GetKeys()))
				HdependAddDependency(SourcePath(package, pkg[i]), SourcePath(package, f[j].text));
	}
	String tout = OutDir(mcfg, mainpackage, bm, use_target);
	host.RealizeDir(tout);
	if(IsNull(mainfn))
		mainfn = GetFileTitle(mainpackage) + b->GetTargetExt();
	if(!IsNull(outfile))
		target = NormalizePath(outfile, tout);
	else {
	#ifdef PLATFORM_COCOA
		if(m.target_override && !IsNull(m.target)
		   && IsFolder(m.target) && GetFileExt(m.target) == ".app")
			target = NormalizePath(m.target);
		else
	#endif
		if(m.target_override && !IsNull(m.target) && IsFolder(m.target))
			target = NormalizePath(AppendFileName(m.target, mainfn));
		else
		if(m.target_override && (IsFullPath(m.target) || *m.target == '/' || *m.target == '\\'))
			target = m.target;
		else
		if(m.target_override && !IsNull(m.target))
			target = NormalizePath(AppendFileName(tout, m.target));
		else
		if(IsFullPath(mainfn))
			target = mainfn;
		else
			target = NormalizePath(AppendFileName(tout, mainfn));
	}
	b->target = target;
	b->mainpackage = mainpackage;
	if(IsNull(onefile)) {
		String out;
		out << "----- " << package << " ( " << Join(b->config.GetKeys(), " ") << " )";
		if(pkcount > 1)
			out << " (" << (pknumber + 1) << " / " << pkcount << ')';
		PutConsole(out);
	}
	else
		b->config.FindAdd("NOLIB");
	bool ok = b->BuildPackage(package, linkfile, immfile, linkopt,
		                      GetAllUses(wspc, pkindex, bm, mainparam, host, *b),
		                      GetAllLibraries(wspc, pkindex, bm, mainparam, host, *b),
		                      targetmode - 1);
	target = b->target; // apple app bundle can change target
	Vector<String> errors = PickErrors();
	for(String p : errors)
		DeleteFile(p);
	if(!ok || !errors.IsEmpty())
		return false;
	if(link) {
		ok = b->Link(linkfile, linkopt, GetTargetMode().createmap);
		PutLinkingEnd(ok);
		errors = PickErrors();
		for(String p : errors)
			DeleteFile(p);
		if(!ok || !errors.IsEmpty())
			return false;
	}
	return true;
}

void MakeBuild::SetHdependDirs()
{
	Vector<String> include = SplitDirs(GetVar("UPP") + ';'
		+ GetMethodVars(method).Get("INCLUDE", "") + ';'
		+ Environment().Get("INCLUDE", "")
#ifdef PLATFORM_POSIX
		+ ";/usr/include;/usr/local/include;"
#endif
		+ add_includes
		);
	// Also adding internal includes
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pkg = wspc.GetPackage(i);
		for(int j = 0; j < pkg.include.GetCount(); j++)
			include.Add(SourcePath(wspc[i], pkg.include[j].text));
	}

	HdependSetDirs(pick(include));
}

Vector<String> MakeBuild::GetAllUses(const Workspace& wspc, int f,
	const VectorMap<String, String>& bm, String mainparam, Host& host, Builder& builder)
{ // Gathers all uses, including subpackages, to support SO builds
	String package = wspc[f];
	Index<String> all_uses;
	bool warn = true;
	int n = 0;
	while(f >= 0) {
		const Package& p = wspc.package[f];
		Index<String> config = PackageConfig(wspc, f, bm, mainparam, host, builder);
		for(int fu = 0; fu < p.uses.GetCount(); fu++) {
			if(MatchWhen(p.uses[fu].when, config.GetKeys())) {
				if(p.uses[fu].text != package)
					all_uses.FindAdd(p.uses[fu].text);
				else if(warn) {
					PutConsole(Format("%s: circular 'uses' chain", package));
					warn = false;
				}
			}
		}
		f = -1;
		while(n < all_uses.GetCount() && (f = wspc.package.Find(all_uses[n++])) < 0)
			;
	}
	return all_uses.PickKeys();
}

Vector<String> MakeBuild::GetAllLibraries(const Workspace& wspc, int index,
	const VectorMap<String, String>& bm, String mainparam,
	Host& host, Builder& builder)
{
	Vector<String> uses = GetAllUses(wspc, index, bm, mainparam, host, builder);
	uses.Add(wspc[index]);
	Index<String> libraries;
	
	for(int i = 0; i < uses.GetCount(); i++) {
		int f = wspc.package.Find(UnixPath(uses[i]));
		if(f >= 0) {
			const Package& pk = wspc.package[f];
			Index<String> config = PackageConfig(wspc, f, bm, mainparam, host, builder);
			Vector<String> pklibs = Split(Gather(pk.library, config.GetKeys()), ' ');
			FindAppend(libraries, pklibs);
		}
	}
	return libraries.PickKeys();
}

bool MakeBuild::Build(const Workspace& wspc, String mainparam, String outfile, bool clear_console)
{
	InitBlitz();
	Builder::cmdx_cache.Clear();

	String hfile = outfile + ".xxx";
	SaveFile(hfile, "");
	start_time = GetFileTime(hfile); // Defensive way to get correct filetime of start
	DeleteFile(hfile);
	
	BeginBuilding(clear_console);

	bool ok = true;
	main_conf.Clear();
	add_includes.Clear();
	MainConf(wspc);
	PutConsole("Saving " + add_includes);
	PutVerbose(main_conf);
	if(wspc.GetCount()) {
		Vector<int> build_order;
		if(cfg.Find("SO") < 0) {
			for(int i = 1; i < wspc.GetCount(); i++)
				build_order.Add(i);
		}
		else {
			Index<int> remaining;
			for(int i = 1; i < wspc.GetCount(); i++)
				remaining.Add(i);
			while(!remaining.IsEmpty()) {
				int t;
				for(t = 0; t < remaining.GetCount(); t++) {
					const Package& pk = wspc.package[remaining[t]];
					bool delay = false;
					for(int u = 0; u < pk.uses.GetCount(); u++) {
						if(remaining.Find(wspc.package.Find(UnixPath(pk.uses[u].text))) >= 0) {
							delay = true;
							break;
						}
					}
					if(!delay)
						break;
				}
				if(t >= remaining.GetCount()) // Progress even if circular references present
					t = 0;
				build_order.Add(remaining[t]);
				remaining.Remove(t);
			}
		}

		String mainpackage = wspc[0];
		Vector<String> linkfile, immfile;
		String linkopt = Merge(" ", GetMethodVars(method).Get("COMMON_LINK", Null),
		                       GetMethodVars(method).Get(targetmode ? "RELEASE_LINK" : "DEBUG_LINK", Null));
		if(linkopt.GetCount())
			linkopt << ' ';
		ok = true;
		int ms = msecs();
		for(int i = 0; i < build_order.GetCount() && (ok || !stoponerrors); i++) {
			int px = build_order[i];
			ok = BuildPackage(wspc, px, i, build_order.GetCount() + 1,
				              mainparam, Null, linkfile, immfile, linkopt) && ok;
			if(msecs() - ms >= 200) {
				DoProcessEvents();
				ms = msecs();
			}
		}
		if(ok || !stoponerrors) {
			ok = BuildPackage(wspc, 0, build_order.GetCount(), build_order.GetCount() + 1,
			                  mainparam, outfile, linkfile, immfile, linkopt, ok) && ok;
		}
	}
	EndBuilding(ok);
	SetErrorEditor();
	return ok;
}

void MakeBuild::BuildWorkspace(Workspace& wspc, Host& host, Builder& builder)
{
	Index<String> p = PackageConfig(GetIdeWorkspace(), 0, GetMethodVars(method), mainconfigparam,
	                                host, builder);
	wspc.Scan(GetMain(), p.GetKeys());
}

bool MakeBuild::Build()
{
	if(GetMethodVars(method).GetCount() == 0) {
		PutConsole(GetInvalidBuildMethodError(method));
		ConsoleShow();
		return false;
	}
	Host host;
	CreateHost(host, false, false);
	One<Builder> builder = CreateBuilder(&host);
	if(!builder)
		return false;
	Workspace wspc;
	BuildWorkspace(wspc, host, *builder);
	return Build(wspc, mainconfigparam, Null);
}

void MakeBuild::CleanPackage(const Workspace& wspc, int package)
{
	PutConsole(Format("Cleaning %s", wspc[package]));
	Host host;
	CreateHost(host, false, false);
	One<Builder> builder = CreateBuilder(&host);
	if(!builder)
		return;
	String outdir = OutDir(PackageConfig(wspc, package, GetMethodVars(method), mainconfigparam,
	                       host, *builder), wspc[package], GetMethodVars(method));
	// TODO: almost perfect, but target will be detected after build. if build does not occur the target is empty :(
	// How to make sure we know target? Target directory is where android project sandbox is.
	builder->target = target;
	builder->CleanPackage(wspc[package], outdir);
}

void MakeBuild::Clean()
{
	ConsoleClear();

	Host host;
	CreateHost(host, false, false);
	One<Builder> builder = CreateBuilder(&host);
	if(!builder)
		return;
	builder->target = target;
	
	Workspace wspc;
	BuildWorkspace(wspc, host, *builder);
	for(int i = 0; i < wspc.GetCount(); i++)
		CleanPackage(wspc, i);
	
	builder->AfterClean();
	
	PutConsole("...done");
}

void MakeBuild::RebuildAll()
{
	Clean();
	Build();
}

String MakeBuild::GetInvalidBuildMethodError(const String& method)
{
	return "Invalid build method " + method + " (" + GetMethodPath(method) + ").";
}

bool MakeBuild::IsAndroidMethod(const String& method) const
{
	VectorMap<String, String> bm = GetMethodVars(method);
	String builder = bm.Get("BUILDER", "");
	if (builder.IsEmpty())
		return false;
	
	One<Builder> pBuilder = (*BuilderMap().Get(builder))();
	if (!pBuilder)
		return false;
	
	return AndroidBuilder::GetBuildersNames().Find(builder) > -1;
}


int HostSys(const char *cmd, String& out)
{
	MakeBuild *mb = dynamic_cast<MakeBuild *>(TheIdeContext());
	if(!mb)
		return Null;
	Host host;
	mb->CreateHost(host, false, false);
	LocalProcess p;
	host.canlog = false;
	if(host.StartProcess(p, cmd))
		return p.Finish(out);
	return Null;
}

String HostSys(const char *cmd)
{
	String out;
	return HostSys(cmd, out) == 0 ? out : String::GetVoid();
}
