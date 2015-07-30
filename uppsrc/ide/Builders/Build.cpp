#include "Builders.h"

#include <plugin/bz2/bz2.h>

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
	mainparam << ' ' << bm.Get(targetmode ? "RELEASE_FLAGS" : "DEBUG_FLAGS", NULL);
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
	if(targetmode == 2)
		cfg.FindAdd("FORCE_SPEED");
	if(targetmode == 3)
		cfg.FindAdd("FORCE_SIZE");
	int q = m.package.Find(wspc[package]);
	if(q >= 0) {
		const PackageMode& p = m.package[q];
		switch(p.debug >= 0 ? p.debug : m.def.debug) {
		case 1:  cfg.FindAdd("DEBUG_MINIMAL"); break;
		case 2:  cfg.FindAdd("DEBUG_FULL"); break;
		}
		if(!pkg.noblitz && (p.blitz >= 0 ? p.blitz : m.def.blitz))
			cfg.FindAdd("BLITZ");
	}
	else {
		switch(m.def.debug) {
		case 1:  cfg.FindAdd("DEBUG_MINIMAL"); break;
		case 2:  cfg.FindAdd("DEBUG_FULL"); break;
		}
		if(!pkg.noblitz && m.def.blitz)
			cfg.FindAdd("BLITZ");
	}
	host.AddFlags(cfg);
	b.AddFlags(cfg);
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
	if(target)
		*target = Gather(pkg.target, cfg.GetKeys(), true);
	Index<String> h;
	h <<= cfg; // Retain deep copy (h will be picked)
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

One<Host> MakeBuild::CreateHost(bool sync_files)
{
	SetupDefaultMethod();
	VectorMap<String, String> bm = GetMethodVars(method);
	One<Host> outhost;
	{
		One<LocalHost> host = new LocalHost;
		VectorMap<String, String> env(Environment(), 1);
		host->exedirs = SplitDirs(bm.Get("PATH", "") + ';' + env.Get("PATH", ""));
		env.GetAdd("PATH") = Join(host->exedirs, ";");
		env.GetAdd("UPP_MAIN__") = GetFileDirectory(PackagePath(GetMain()));
		env.GetAdd("UPP_ASSEMBLY__") = GetVar("UPP");
		
		// setup LD_LIBRARY_PATH on target dir, needed for all shared builds on posix
#ifdef PLATFORM_POSIX
		if(target != "")
		{
			String ldPath = GetFileFolder(target) + ";" + env.Get("LD_LIBRARY_PATH", "");
			env.GetAdd("LD_LIBRARY_PATH") = ldPath;
		}
#endif
		for(int i = 0; i < env.GetCount(); i++) {
			LDUMP(env.GetKey(i));
			LDUMP(env[i]);
			host->environment << env.GetKey(i) << '=' << env[i] << '\0';
		}
		host->environment.Cat(0);
		host->cmdout = &cmdout;
		outhost = -host;
	}
	return outhost;
}

One<Builder> MakeBuild::CreateBuilder(Host *host)
{
	SetupDefaultMethod();
	InitBlitz();
	VectorMap<String, String> bm = GetMethodVars(method);
	String builder = bm.Get("BUILDER", "GCC");
	int q = BuilderMap().Find(builder);
	if(q < 0) {
		PutConsole("Invalid builder " + builder);
		ConsoleShow();
		return NULL;
	}
	Builder* b = (*BuilderMap().Get(builder))();
	b->host = host;
	b->script = bm.Get("SCRIPT", "");
	if(AndroidBuilder::GetBuildersNames().Find(builder) > -1) {
		AndroidBuilder* ab = dynamic_cast<AndroidBuilder*>(b);
		ab->androidNDK.SetPath((bm.Get("NDK_PATH", "")));
		ab->jdk.SetPath((bm.Get("JDK_PATH", "")));
		
		String platformVersion = bm.Get("SDK_PLATFORM_VERSION", "");
		if(!platformVersion.IsEmpty())
			ab->androidSDK.SetPlatform(platformVersion);
		else
			ab->androidSDK.DeducePlatform();
		String buildToolsRelease = bm.Get("SDK_BUILD_TOOLS_RELEASE", "");
		if(!buildToolsRelease.IsEmpty())
			ab->androidSDK.SetBuildToolsRelease(buildToolsRelease);
		else
			ab->androidSDK.DeduceBuildToolsRelease();
		
		ab->ndk_blitz = bm.Get("NDK_BLITZ", "") == "1";
		if(bm.Get("NDK_ARCH_ARMEABI", "") == "1")
			ab->ndkArchitectures.Add("armeabi");
		if(bm.Get("NDK_ARCH_ARMEABI_V7A", "") == "1")
			ab->ndkArchitectures.Add("armeabi-v7a");
		if(bm.Get("NDK_ARCH_ARM64_V8A", "") == "1")
			ab->ndkArchitectures.Add("arm64-v8a");
		if(bm.Get("NDK_ARCH_X86", "") == "1")
			ab->ndkArchitectures.Add("x86");
		if(bm.Get("NDK_ARCH_X86_64", "") == "1")
			ab->ndkArchitectures.Add("x86_64");
		if(bm.Get("NDK_ARCH_MIPS", "") == "1")
			ab->ndkArchitectures.Add("mips");
		if(bm.Get("NDK_ARCH_MIPS64", "") == "1")
			ab->ndkArchitectures.Add("mips64");
		ab->ndkToolchain = bm.Get("NDK_TOOLCHAIN", "");
		ab->ndkCppRuntime = bm.Get("NDK_CPP_RUNTIME", "");
		ab->ndkCppFlags = bm.Get("NDK_COMMON_CPP_OPTIONS", "");
		ab->ndkCFlags = bm.Get("NDK_COMMON_C_OPTIONS", "");
		
		b = ab;
	}
	else {
		// TODO: cpp builder variables only!!!
		b->compiler = bm.Get("COMPILER", "");
		b->include = SplitDirs(GetVar("UPP") + ';' + bm.Get("INCLUDE", "") + ';' + add_includes);
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
		b->release_size_options = Join(bm.Get("COMMON_OPTIONS", ""), bm.Get("RELEASE_SIZE_OPTIONS", ""));
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
	LocalHost().AddFlags(excl);
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
		outdir = AppendFileName(outdir, GetVarsName());
	if(!use_target)
		outdir = AppendFileName(outdir, package);
	outdir = AppendFileName(outdir, GetFileTitle(method) + "." + Join(x, "."));
	outdir = Filter(outdir, CharFilterSlash);
	return outdir;
}

struct OneFileHost : Host {
	One<Host> host;
	String    onefile;

	virtual String GetEnvironment()                { return host->GetEnvironment(); }
	virtual String GetHostPath(const String& path) { return host->GetHostPath(path); }
	virtual String GetLocalPath(const String& path) { return host->GetLocalPath(path); }
	virtual String NormalizePath(const String& path) { return host->NormalizePath(path); }
	virtual void   DeleteFile(const Vector<String>& path) { host->DeleteFile(path); }
	virtual void   DeleteFolderDeep(const String& folder) { host->DeleteFolderDeep(folder); }
	virtual void   ChDir(const String& path) { host->ChDir(path); }
	virtual void   RealizeDir(const String& path) { host->RealizeDir(path); }
	virtual void   SaveFile(const String& path, const String& data) { host->SaveFile(path, data); }
	virtual String LoadFile(const String& path) { return host->LoadFile(path); }
	virtual int    Execute(const char *c) { return host->Execute(c); }
	virtual int    ExecuteWithInput(const char *c, bool noconvert) { return host->ExecuteWithInput(c, noconvert); }
	virtual int    Execute(const char *c, Stream& o, bool noconvert) { return host->Execute(c, o, noconvert); }
	virtual int    AllocSlot() { return host->AllocSlot(); }
	virtual bool   Run(const char *cmdline, int slot, String key, int blitz_count) { return host->Run(cmdline, slot, key, blitz_count); }
	virtual bool   Run(const char *cmdline, Stream& out, int slot, String key, int blitz_count) { return host->Run(cmdline, out, slot, key, blitz_count); }
	virtual bool   Wait() { return host->Wait(); }
	virtual bool   Wait(int slot) { return host->Wait(slot); }
	virtual void   OnFinish(Callback cb) { return host->OnFinish(cb); }
	virtual One<AProcess> StartProcess(const char *c) { return host->StartProcess(c); }
	virtual void   Launch(const char *cmdline, bool) { host->Launch(cmdline); }
	virtual void   AddFlags(Index<String>& cfg) { host->AddFlags(cfg); }

	virtual Vector<FileInfo> GetFileInfo(const Vector<String>& path) {
		Vector<FileInfo> fi = host->GetFileInfo(path);
		for(int i = 0; i < path.GetCount(); i++)
			if(path[i] == onefile)
				(Time &)fi[i] = GetSysTime();
			else
				(Time &)fi[i] = Time::Low();
		return fi;
	}
};

bool MakeBuild::BuildPackage(const Workspace& wspc, int pkindex, int pknumber, int pkcount,
	String mainparam, String outfile, Vector<String>& linkfile, Vector<String>& immfile,
	String& linkopt, bool link)
{
	String package = wspc[pkindex];
	String mainpackage = wspc[0];
	const Package& pkg = wspc.package[pkindex];
	VectorMap<String, String> bm = GetMethodVars(method);
	if(bm.GetCount() == 0) {
		PutConsole("Invalid build method");
		ConsoleShow();
		return false;
	}
	One<Host> host = CreateHost(false);
	if(!IsNull(onefile)) {
		OneFileHost *h = new OneFileHost;
		h->host = pick(host);
		h->onefile = onefile;
		host = h;
	}
	One<Builder> b = CreateBuilder(~host);
	if(!b)
		return false;
	b->config = PackageConfig(wspc, pkindex, bm, mainparam, *host, *b);
	const TargetMode& m = targetmode == 0 ? debug : release;
	b->version = m.version;
	b->method = method;
	b->outdir = OutDir(b->config, package, bm);
	host->RealizeDir(b->outdir);
	String mainfn = Null;
	Index<String> mcfg = PackageConfig(wspc, 0, bm, mainparam, *host, *b, &mainfn);
	HdependClearDependencies();
	for(int i = 0; i < pkg.GetCount(); i++) {
		const Array<OptItem>& f = pkg[i].depends;
		for(int j = 0; j < f.GetCount(); j++)
			if(MatchWhen(f[j].when, mcfg.GetKeys()))
				HdependAddDependency(SourcePath(package, pkg[i]), SourcePath(package, f[j].text));
	}
	String tout = OutDir(mcfg, mainpackage, bm, use_target);
	host->RealizeDir(tout);
	if(IsNull(mainfn))
		mainfn = GetFileTitle(mainpackage) + b->GetTargetExt();
	if(!IsNull(outfile))
		target = NormalizePath(outfile, tout);
	else {
		if(m.target_override && !IsNull(m.target) && IsFolder(m.target))
			target = host->NormalizePath(AppendFileName(m.target, mainfn));
		else
		if(m.target_override && (IsFullPath(m.target) || *m.target == '/' || *m.target == '\\'))
			target = m.target;
		else
		if(m.target_override && !IsNull(m.target))
			target = host->NormalizePath(AppendFileName(tout, m.target));
		else
		if(IsFullPath(mainfn))
			target = mainfn;
		else
			target = host->NormalizePath(AppendFileName(tout, mainfn));
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
		                      GetAllUses(wspc, pkindex, bm, mainparam, *host, *b),
		                      GetAllLibraries(wspc, pkindex, bm, mainparam, *host, *b),
		                      targetmode - 1);
	Vector<String> errors = PickErrors();
	host->DeleteFile(errors);
	if(!ok || !errors.IsEmpty())
		return false;
	if(link) {
		ok = b->Link(linkfile, linkopt, GetTargetMode().createmap);
		PutLinkingEnd(ok);
		errors = PickErrors();
		host->DeleteFile(errors);
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
					PutConsole(NFormat("%s: circular 'uses' chain", package));
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
{ // Warning: This does not seem to do what it is supposed to do...
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
	String hfile = outfile + ".xxx";
	SaveFile(hfile, "");
	start_time = GetFileTime(hfile); // Defensive way to get correct filetime of start
	DeleteFile(hfile);
	
	ClearErrorEditor();
	BeginBuilding(true, clear_console);
	bool ok = true;
	main_conf.Clear();
	if(wspc.GetCount()) {
		for(int i = 0; i < wspc.GetCount(); i++) {
			const Package& pk = wspc.package[i];
			for(int j = 0; j < pk.GetCount(); j++)
				if(pk[j] == "main.conf") {
					String pn = wspc[i];
					String p = SourcePath(pn, "main.conf");
					main_conf << "// " << pn << "\r\n" << LoadFile(p) << "\r\n";
					PutConsole("Found " + p);
				}
		}

		if(main_conf.GetCount()) {
			VectorMap<String, String> bm = GetMethodVars(method);
			One<Host> host = CreateHost(false);
			One<Builder> b = CreateBuilder(~host);
			if(b) {
				Index<String> mcfg = PackageConfig(wspc, 0, bm, mainparam, *host, *b, NULL);
				String outdir = OutDir(mcfg, wspc[0], bm, false);
				String path = AppendFileName(outdir, "main.conf.h");
				RealizePath(path);
				SaveChangedFile(path, main_conf);
				PutConsole("Saving " + path);
				add_includes << outdir << ';';
			}
		}

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
		String linkopt = GetMethodVars(method).Get(targetmode ? "RELEASE_LINK" : "DEBUG_LINK", Null);
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
			// Set the time of target and intermediates to start-time, so that if any file
			// changes during compilation, it is recompiled during next build
			SetFileTime(target, start_time);
			for(int i = 0; i < immfile.GetCount(); i++)
				SetFileTime(immfile[i], start_time); 
		}
	}
	EndBuilding(ok);
	ReQualifyCodeBase();
	SetErrorEditor();
	return ok;
}

bool MakeBuild::Build()
{
	VectorMap<String, String> bm = GetMethodVars(method);
	if(bm.GetCount() == 0) {
		PutConsole("Invalid build method");
		ConsoleShow();
		return false;
	}
	One<Host> host = CreateHost(false);
	One<Builder> builder = CreateBuilder(~host);
	if(!builder)
		return false;
	Index<String> p = PackageConfig(GetIdeWorkspace(), 0, bm, mainconfigparam,
	                                *host, *builder);
	Workspace wspc;
	wspc.Scan(GetMain(), p.GetKeys());
	return Build(wspc, mainconfigparam, Null);
}

void MakeBuild::CleanPackage(const Workspace& wspc, int package)
{
	PutConsole(NFormat("Cleaning %s", wspc[package]));
	One<Host> host = CreateHost(false);
	One<Builder> builder = CreateBuilder(~host);
	if(!builder)
		return;
	builder->outdir = OutDir(PackageConfig(wspc, package, GetMethodVars(method), mainconfigparam,
	                         *host, *builder), wspc[package], GetMethodVars(method));
	// TODO: almost perfect, but target will be detected after build. if build dosen't occure the target is empty :(
	// How to make sure we know target? Target directory is where android project sandbox is.
	builder->target = target;
	builder->CleanPackage(wspc[package]);
	// TODO: Each builder should call delete folder by itself.
	// For example in CppBuilder DeleteFolderDeep(outdir).
	host->DeleteFolderDeep(OutDir(PackageConfig(wspc, package, GetMethodVars(method), mainconfigparam,
		*host, *builder), wspc[package], GetMethodVars(method)));
}

void MakeBuild::Clean()
{
	ConsoleClear();
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++)
		CleanPackage(wspc, i);
	PutConsole("...done");
}

void MakeBuild::RebuildAll()
{
	Clean();
	Build();
}

void MakeBuild::SaveMakeFile(const String& fn, bool exporting)
{
	BeginBuilding(false, true);

	VectorMap<String, String> bm = GetMethodVars(method);
	One<Host> host = CreateHost(false);
	One<Builder> b = CreateBuilder(~host);
	
	if(!b)
		return;
	
	const TargetMode& tm = GetTargetMode();

	String makefile;

	Vector<String> uppdirs = GetUppDirs();
	String uppout = exporting ? host->GetHostPath(GetVar("OUTPUT")) : "_out/";
	String inclist;

	Index<String> allconfig = PackageConfig(GetIdeWorkspace(), 0, bm, mainconfigparam, *host, *b);
	bool win32 = allconfig.Find("WIN32") >= 0;

	Workspace wspc;
	wspc.Scan(GetMain(), allconfig.GetKeys());

	for(int i = 1; i < wspc.GetCount(); i++) {
		Index<String> modconfig = PackageConfig(wspc, i, bm, mainconfigparam, *host, *b);
		for(int a = allconfig.GetCount(); --a >= 0;)
			if(modconfig.Find(allconfig[a]) < 0)
				allconfig.Remove(a);
	}

	if(!exporting)
		for(int i = 0; i < uppdirs.GetCount(); i++) {
			String srcdir = GetMakePath(AdjustMakePath(host->GetHostPath(AppendFileName(uppdirs[i], ""))), win32);
			makefile << "UPPDIR" << (i + 1) << " = " << srcdir << "\n";
			inclist << " -I$(UPPDIR" << (i + 1) << ")";
		}
	else
		inclist << "-I./";
	Vector<String> includes = SplitDirs(bm.Get("INCLUDE",""));
	for(int i = 0; i < includes.GetCount(); i++)
		inclist << " -I" << includes[i];

	makefile << "\n"
		"UPPOUT = " << (exporting ? "_out/" : GetMakePath(AdjustMakePath(host->GetHostPath(AppendFileName(uppout, ""))), win32)) << "\n"
		"CINC = " << inclist << "\n"
		"Macro = ";

	for(int i = 0; i < allconfig.GetCount(); i++)
		makefile << " -Dflag" << allconfig[i];
	makefile << "\n";

	String output, config, install, rules, linkdep, linkfiles, linkfileend;

	for(int i = 0; i < wspc.GetCount(); i++) {
		b->config = PackageConfig(wspc, i, bm, mainconfigparam, *host, *b);
		b->version = tm.version;
		b->method = method;
		MakeFile mf;
		b->AddMakeFile(mf, wspc[i], GetAllUses(wspc, i, bm, mainconfigparam, *host, *b),
		               GetAllLibraries(wspc, i, bm, mainconfigparam, *host, *b), allconfig,
		               exporting);
		if(!i) {
			String tdir = mf.outdir;
			String trg;
			if(tm.target_override) {
				trg = GetMakePath(AdjustMakePath(tm.target), win32);
				if(!trg.IsEmpty() && *trg.Last() == (win32 ? '\\' : '/'))
					trg << mf.outfile;
				else if(trg.Find(win32 ? '\\' : '/') < 0)
					trg.Insert(0, "$(OutDir)");
			}
			output = Nvl(trg, mf.output);
			if(exporting)
				output = wspc[i] + ".out";
			install << "\n"
				"OutDir = " << tdir << "\n"
				"OutFile = " << output << "\n"
				"\n"
				".PHONY: all\n"
				"all: prepare $(OutFile)\n"
				"\n"
				".PHONY: build_info\n"
				"build_info:\n"
				"	date '+#define bmYEAR    %y%n'\\\n"
				"	'#define bmMONTH   %m%n'\\\n"
				"	'#define bmDAY     %d%n'\\\n"
				"	'#define bmHOUR    %H%n'\\\n"
				"	'#define bmMINUTE  %M%n'\\\n"
				"	'#define bmSECOND  %S%n'\\\n"
				"	'#define bmTIME    Time(%y, %m, %d, %H, %M, %S)' > build_info.h\n"
				"	echo '#define bmMACHINE \"'`hostname`'\"' >> build_info.h\n"
				"	echo '#define bmUSER    \"'`whoami`'\"' >> build_info.h\n"
				"\n"
				".PHONY: prepare\n"
				"prepare: build_info\n";
		}
		config << mf.config;
		install << mf.install;
		rules << mf.rules;
		linkdep << mf.linkdep;
		linkfiles << mf.linkfiles;
		linkfileend << mf.linkfileend;
	}

	makefile
		<< config
		<< install
		<< "\n"
		"$(OutFile): " << linkdep << "\n\t" << linkfiles << linkfileend << " -Wl,--end-group\n\n"
		<< rules
		<< ".PHONY: clean\n"
		<< "clean:\n"
		<< "\tif [ -d $(UPPOUT) ]; then rm -rf $(UPPOUT); fi;\n";

	bool sv = ::SaveFile(fn, makefile);
	if(!exporting) {
		if(sv)
			PutConsole(NFormat("%s(1): makefile generation complete", fn));
		else
			PutConsole(NFormat("%s: error writing makefile", fn));
	}
	EndBuilding(true);
}
