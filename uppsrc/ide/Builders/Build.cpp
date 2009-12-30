#include "Builders.h"

#include <plugin/bz2/bz2.h>

#define LDUMP(x) // DUMP(x)

String GetDefaultMethod()
{
	return LoadFile(ConfigFile("default_method"));
}

VectorMap<String, String> GetMethodVars(const String& method)
{
	VectorMap<String, String> map;
	LoadVarFile(ConfigFile((String)~method + ".bm"), map);
	return map;
}

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
	Index<String> cfg;
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
	if(target)
		*target = Gather(pkg.target, cfg.GetKeys(), true);
	return cfg;
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

bool MakeBuild::SyncHostFiles(RemoteHost& host)
{
	HdependTimeDirty();
	String query = "<";
//	Vector<String> remotepath;
	Vector<String> fdata;
	ArrayMap<String, TransferFileInfo> info;
	Vector<int> nocrsize;
	const Workspace& wspc = GetIdeWorkspace();
	int p;
	for(p = 0; p < wspc.GetCount(); p++) {
		const Package& pkg = wspc.GetPackage(p);
		String pn = wspc[p];
		for(int f = -1; f < pkg.file.GetCount(); f++)
			if(f < 0 || !pkg.file[f].separator) {
				Vector<String> pkgfiles;
				String pk = (f >= 0 ? SourcePath(pn, pkg.file[f]) : PackagePath(pn));
				if(!FindFile(pk).IsFile())
					continue;
				pkgfiles.Add(f >= 0 ? SourcePath(pn, pkg.file[f]) : PackagePath(pn));
				pkgfiles.AppendPick(HdependGetDependencies(pkgfiles[0]));
				for(int d = 0; d < pkgfiles.GetCount(); d++) {
					TransferFileInfo newinfo;
					newinfo.sourcepath = pkgfiles[d];
					String rp = host.GetHostPath(newinfo.sourcepath);
					if(info.Find(rp) >= 0 || (d && UnixPath(rp) == UnixPath(newinfo.sourcepath)))
						continue;
					FindFile ff(newinfo.sourcepath);
					newinfo.filetime = (int)(Time(ff.GetLastWriteTime()) - RemoteHost::TimeBase());
					newinfo.filesize = (int)ff.GetLength();
					int pos = transferfilecache.Find(rp);
					if(pos < 0 || transferfilecache[pos].filetime != newinfo.filetime
						|| transferfilecache[pos].filesize != newinfo.filesize)
					{
						String content = NoCr(LoadFile(newinfo.sourcepath));
						query << rp << '\t' << newinfo.filetime << '\t' << content.GetLength() << '\n';
	//					remotepath.Add(rp);
						fdata.Add(content);
						info.Add(rp, newinfo);
						nocrsize.Add(content.GetLength());
						PutVerbose("Checking remote " + rp);
	/*
						if(msecs(ticks) >= 0)
						{
							ShowConsole();
							console.Sync();
							ticks = msecs(-200);
						}
	*/				}
				}
			}
	}
	if(info.IsEmpty())
		return true;
//	String host = GetVar("REMOTE_HOST");
//	int port = 2346;
//	int ppos = host.Find(':');
//	if(ppos >= 0)
//	{
//		port = ScanInt(host.GetIter(ppos + 1));
//		host.Trim(ppos);
//	}

	Index<String> ignore;
	{
		PutConsole(NFormat("Retrieving update list for remote filesystem %s", host.host));
		ConsoleShow();
		String out = host.RemoteExec(query);
		if(out[0] != 'O' || out[1] != 'K') {
			PutConsole(out);
			return false;
		}

		const char *s = out;
		while(*s && *s++ != '\n')
			;
		while(*s)
		{
			const char *b = s;
			while(*s && *s != '\n')
				s++;
			ignore.FindAdd(NormalizePath(NativePath(String(b, s))));
			if(*s)
				s++;
		}
		if(ignore.GetCount() == info.GetCount())
			PutConsole("Remote source tree is up to date.");
		else
		{
			PutConsole(NFormat("%d file(s) missing in remote source tree:", info.GetCount() - ignore.GetCount()));
			for(p = 0; p < info.GetCount(); p++)
			{
//				console << "   " << remotepath[p] << " - "
//					<< FormatInt(p) << " @ " << FormatInt(ignore.Find(remotepath[p])) << "\n";
				if(ignore.Find(NormalizePath(NativePath(info.GetKey(p)))) < 0)
					PutConsole(String().Cat() << "   " << info.GetKey(p));
			}
		}
	}

	Vector<String> update;
	for(p = 0; p < info.GetCount(); p++)
		if(ignore.Find(NormalizePath(NativePath(info.GetKey(p)))) < 0)
		{
			String rawdata = fdata[p];
			PutConsole(String().Cat() << "Compressing " << info[p].sourcepath << " (" << FormatInt(rawdata.GetLength()) << " B)\n");
			ConsoleSync();
			String bzdata = BZ2Compress(rawdata);
			String comp = ASCII85Encode(bzdata);
			if(update.IsEmpty() || update.Top().GetLength() + comp.GetLength() >= 500000)
				update.Add(">");
			update.Top() << info.GetKey(p) << '\t' << info[p].filetime << '\t' << nocrsize[p] << '\t' << comp << '\n';
		}

	for(p = 0; p < update.GetCount(); p++)
	{
		PutConsole(NFormat("Uploading block %d / %d (%d B)", p + 1, update.GetCount(), update[p].GetLength()));
		ConsoleShow();
		String result = host.RemoteExec(update[p]);
		if(result[0] != 'O' || result[1] != 'K') {
			PutConsole(result);
			return false;
		}
	}

	for(p = 0; p < info.GetCount(); p++)
		transferfilecache.GetAdd(info.GetKey(p)) = info[p];
	return true;
}

One<Host> MakeBuild::CreateHost(bool sync_files)
{
	SetupDefaultMethod();
	VectorMap<String, String> bm = GetMethodVars(method);
	String rm = bm.Get("REMOTE_HOST", Null);
	One<Host> outhost;
	if(!IsNull(rm)) {
		One<RemoteHost> host = new RemoteHost;
		host->host = rm;
		host->port = 2346;
		int f = rm.Find(':');
		if(f >= 0) {
			host->host = rm.Left(f);
			host->port = Nvl(ScanInt(rm.GetIter(f + 1)), host->port);
		}
		host->os_type = bm.Get("REMOTE_OS", "UNIX");
		Vector<String> path_map = Split(bm.Get("REMOTE_MAP", Null), ';');
		for(int p = 0; p < path_map.GetCount(); p++) {
			f = path_map[p].Find('>');
			if(f >= 0) {
				host->path_map_local.Add(path_map[p].Left(f));
				host->path_map_remote.Add(path_map[p].Mid(f + 1));
			}
		}
		VectorMap<String, String> env(Environment(), 1);
		Vector<String> exedirs = SplitDirs(bm.Get("PATH", "") + ';' + env.Get("PATH", ""));
		env.GetAdd("PATH") = Join(exedirs, ";");
		for(int i = 0; i < env.GetCount(); i++)
			host->environment << env.GetKey(i) << '=' << env[i] << '\0';
		host->environment.Cat(0);
		if(sync_files && bm.Get("REMOTE_TRANSFER", Null) != "0")
			SyncHostFiles(*host);
		outhost = -host;
	}
	else {
		One<LocalHost> host = new LocalHost;
		VectorMap<String, String> env(Environment(), 1);
		host->exedirs = SplitDirs(bm.Get("PATH", "") + ';' + env.Get("PATH", ""));
		env.GetAdd("PATH") = Join(host->exedirs, ";");
		env.GetAdd("UPP_MAIN__") = GetFileDirectory(PackagePath(GetMain()));
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
	VectorMap<String, String> bm = GetMethodVars(method);
	String builder = bm.Get("BUILDER", "GCC");
	One<Builder> b = (*BuilderMap().Get(builder))();
	b->host = host;
	b->compiler = bm.Get("COMPILER", "");
	b->include = SplitDirs(GetVar("UPP") + ';' + bm.Get("INCLUDE", ""));
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pkg = wspc.GetPackage(i);
		for(int j = 0; j < pkg.include.GetCount(); j++)
			b->include.Add(SourcePath(wspc[i], pkg.include[j].text));
	}	
	b->libpath = SplitDirs(bm.Get("LIB", ""));
	b->debug_options = bm.Get("DEBUG_OPTIONS", "");
	b->release_options = bm.Get("RELEASE_OPTIONS", "");
	b->release_size_options = bm.Get("RELEASE_SIZE_OPTIONS", "");
	b->script = bm.Get("SCRIPT", "");
	return b;
}

int CharFilterSlash(int c)
{
	return c == '\\' ? '/' : c;
}

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
	if(!use_target)
		outdir = AppendFileName(outdir, package);
	outdir = AppendFileName(outdir, method + "." + Join(x, "."));
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
	virtual int    ExecuteWithInput(const char *c) { return host->ExecuteWithInput(c); }
	virtual int    Execute(const char *c, Stream& o) { return host->Execute(c, o); }
	virtual int    AllocSlot() { return host->AllocSlot(); }
	virtual bool   Run(const char *cmdline, int slot, String key, int blitz_count) { return host->Run(cmdline, slot, key, blitz_count); }
	virtual bool   Run(const char *cmdline, Stream& out, int slot, String key, int blitz_count) { return host->Run(cmdline, out, slot, key, blitz_count); }
	virtual bool   Wait() { return host->Wait(); }
	virtual One<SlaveProcess> StartProcess(const char *c) { return host->StartProcess(c); }
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
	String mainparam, String outfile, Vector<String>& linkfile, String& linkopt, bool link)
{
	String package = wspc[pkindex];
	String mainpackage = wspc[0];
	const Package& pkg = wspc.package[pkindex];
	VectorMap<String, String> bm = GetMethodVars(method);
	if(bm.GetCount() == 0) {
		PutConsole("Invalid build method");
		return false;
	}
	One<Host> host = CreateHost(false);
	if(!IsNull(onefile)) {
		OneFileHost *h = new OneFileHost;
		h->host = host;
		h->onefile = onefile;
		host = h;
	}
	One<Builder> b = CreateBuilder(~host);
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
	if(IsNull(onefile)) {
		String out;
		out << "----- " << package << " ( " << Join(b->config.GetKeys(), " ") << " )";
		if(pkcount > 1)
			out << " (" << (pknumber + 1) << " / " << pkcount << ')';
		PutConsole(out);
	}
	else
		b->config.FindAdd("NOLIB");
	bool ok = b->BuildPackage(package, linkfile, linkopt,
		                      GetAllUses(wspc, pkindex),
		                      GetAllLibraries(wspc, pkindex, bm, mainparam, *host, *b),
		                      targetmode - 1);
	Vector<String> errors = PickErrors();
	host->DeleteFile(errors);
	if(!ok || !errors.IsEmpty())
		return false;
	if(link) {
		ok = b->Link(linkfile, linkopt, GetTargetMode().createmap);
		errors = PickErrors();
		host->DeleteFile(errors);
		if(!ok || !errors.IsEmpty())
			return false;
	}
	return true;
}

void MakeBuild::SetHdependDirs()
{
	HdependSetDirs(SplitDirs(GetVar("UPP") + ';'
	                         + GetMethodVars(method).Get("INCLUDE", "") + ';'
	                         + Environment().Get("INCLUDE", "")));
}

Vector<String> MakeBuild::GetAllUses(const Workspace& wspc, int f)
{
	String package = wspc[f];
	Index<String> all_uses;
	bool warn = true;
	int n = 0;
	while(f >= 0) {
		const Package& p = wspc.package[f];
		for(int fu = 0; fu < p.uses.GetCount(); fu++)
			if(p.uses[fu].text != package)
				all_uses.FindAdd(p.uses[fu].text);
			else if(warn) {
				PutConsole(NFormat("%s: circular 'uses' chain", package));
				warn = false;
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
	Vector<String> uses = GetAllUses(wspc, index);
	uses.Add(wspc[index]);
	Index<String> libraries;
	for(int i = 0; i < uses.GetCount(); i++) {
		int f = wspc.package.Find(uses[i]);
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
	ClearErrorEditor();
	BeginBuilding(true, clear_console);
	bool ok = true;
	if(wspc.GetCount()) {
		Vector<int> build_order;
		if(GetTargetMode().linkmode != 2) {
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
					for(int u = 0; u < pk.uses.GetCount(); u++)
						if(remaining.Find(wspc.package.Find(pk.uses[u].text)) >= 0) {
							delay = true;
							break;
						}
					if(!delay)
						break;
				}
				if(t >= remaining.GetCount())
					t = 0;
				build_order.Add(remaining[t]);
				remaining.Remove(t);
			}
		}
		String mainpackage = wspc[0];
		Vector<String> linkfile;
		String linkopt = GetMethodVars(method).Get(targetmode ? "RELEASE_LINK" : "DEBUG_LINK", Null);
		if(linkopt.GetCount())
			linkopt << ' ';
		ok = true;
		int ms = msecs();
		for(int i = 0; i < build_order.GetCount() && (ok || !stoponerrors); i++) {
			int px = build_order[i];
			ok = BuildPackage(wspc, px, i, build_order.GetCount() + 1,
				              mainparam, Null, linkfile, linkopt) && ok;
			if(msecs() - ms >= 200) {
				DoProcessEvents();
				ms = msecs();
			}
		}
		if(ok || !stoponerrors)
			ok = BuildPackage(wspc, 0, build_order.GetCount(), build_order.GetCount() + 1,
				mainparam, outfile, linkfile, linkopt, ok) && ok;
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
		return false;
	}
	One<Host> host = CreateHost(false);
	Index<String> p = PackageConfig(GetIdeWorkspace(), 0, bm, mainconfigparam,
	                                *host, *CreateBuilder(~host));
	Workspace wspc;
	wspc.Scan(GetMain(), p.GetKeys());
	return Build(wspc, mainconfigparam, Null);
}

void MakeBuild::CleanPackage(const Workspace& wspc, int package)
{
	PutConsole(NFormat("Cleaning %s", wspc[package]));
	One<Host> host = CreateHost(false);
	host->DeleteFolderDeep(OutDir(PackageConfig(wspc, package, GetMethodVars(method), mainconfigparam,
		*host, *CreateBuilder(~host)), wspc[package], GetMethodVars(method)));
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
	const Workspace& wspc = GetIdeWorkspace();

	BeginBuilding(false, true);

	VectorMap<String, String> bm = GetMethodVars(method);
	One<Host> host = CreateHost(false);
	One<Builder> b = CreateBuilder(~host);
	const TargetMode& tm = GetTargetMode();

	String makefile;

	Vector<String> uppdirs = GetUppDirs();
	String uppout = exporting ? host->GetHostPath(GetVar("OUTPUT")) : "_out/";
	String inclist;

	Index<String> allconfig = PackageConfig(wspc, 0, bm, mainconfigparam, *host, *b);
	bool win32 = allconfig.Find("WIN32") >= 0;

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
		b->AddMakeFile(mf, wspc[i], GetAllUses(wspc, i),
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
				"all: install $(OutFile)\n"
				"\n"
				".PHONY: install\n"
				"install:\n";
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
		"$(OutFile): " << linkdep << "\n\t" << linkfiles << linkfileend << "\n"
		<< rules;

	bool sv = ::SaveFile(fn, makefile);
	if(!exporting)
		if(sv)
			PutConsole(NFormat("%s(1): makefile generation complete", fn));
		else
			PutConsole(NFormat("%s: error writing makefile", fn));

	EndBuilding(true);
}
