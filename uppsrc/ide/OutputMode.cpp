#include "ide.h"

INITBLOCK
{
	RegisterWorkspaceConfig("outputmode-release");
	RegisterWorkspaceConfig("outputmode-debug");
}

void TargetMode::Serialize(Stream& s)
{
	int ver = 4;
	s / ver;
	s % target_override % target;
	if(ver < 3) {
		int dummy;
	 	s % dummy;
	}
	if(ver < 1) {
		String dummy;
		s % dummy;
	}
	else
		s % version;
	s % def % package;
	if(ver == 2) {
		int dummy;
		s % dummy;
	}
	if(ver >= 3)
		s % linkmode;
	if(ver >= 4)
		s % createmap;
}

struct ModePane : WithModePaneLayout<StaticRect> {
	DropList       debugs;
	DropList       blitzs;
	SaveFileButton target_browse;

	void  Serialize(Stream& s) { target.SerializeList(s); }

	void  Load(const TargetMode& m);
	void  Save(TargetMode& m);

	ModePane();
};

void ModePane::Load(const TargetMode& m)
{
	target_override = m.target_override;
	target <<= m.target;
	map = m.createmap;
	linkmode = m.linkmode;
	version = m.version.ToWString();
	debug = m.def.debug;
	blitz = m.def.blitz;
	const Workspace& wspc = GetIdeWorkspace();
	package.Clear();
	for(int i = 0; i < wspc.GetCount(); i++) {
		int q;
		String p = wspc[i];
		q = m.package.Find(p);
		if(q >= 0) {
			const PackageMode& k = m.package[q];
			package.Add(p, k.debug, k.blitz);
		}
		else
			package.Add(p);
	}
}

void ModePane::Save(TargetMode& m)
{
	m.target_override = target_override;
	m.target = ~target;
	m.createmap = map;
	m.linkmode = (int)~linkmode;
	m.version = ~version;
	m.def.debug = ~debug;
	m.def.blitz = ~blitz;
	m.package.Clear();
	for(int i = 0; i < package.GetCount(); i++) {
		PackageMode k;
		k.debug = package.Get(i, 1);
		k.blitz = package.Get(i, 2);
		if(!IsNull(k.debug) || !IsNull(k.blitz))
			m.package.Add(package.Get(i, 0)) = k;
	}
	target.AddHistory();
}

void DebugF(One<Ctrl>& ctrl)
{
	ctrl.Create<DropList>()
	   .Add(Null, "")
	   .Add(0, "None")
	   .Add(1, "Minimal")
	   .Add(2, "Full");
}

void BlitzF(One<Ctrl>& ctrl)
{
	ctrl.Create<Option>().ThreeState();
}

ModePane::ModePane()
{
	CtrlLayout(*this);
	package.AddColumn("Package");
	package.AddColumn("Debug").Ctrls(DebugF);
	package.AddColumn("Blitz").Ctrls(BlitzF);
	package.ColumnWidths("80 96 96");
	package.EvenRowColor();
	package.NoHorzGrid();
	package.SetLineCy(EditField::GetStdHeight() + 2);
	package.NoCursor();
	debug.Add(0, "None")
	     .Add(1, "Minimal")
	     .Add(2, "Full");
	target_browse.Attach(target);
}

struct OutMode : WithOutputModeLayout<TopWindow> {
	Ide& ide;
	ModePane debug;
	ModePane release;
	FrameRight<Button> dsb;

	void Load();
	void Save();
	void Preset();
	void SyncLock();

	void Export(int kind);

	void ConfigChange();
	void CmdOptions();

	typedef OutMode CLASSNAME;

	OutMode(Ide& ide);
};

void OutMode::Load()
{
	config.Clear();
	const DropList& list = ide.mainconfiglist;
	for (int i = 0; i < list.GetCount(); ++i)
		config.Add(list.GetKey(i), list[i]);
	config <<= ide.mainconfigparam;
	method.Clear();
	FindFile ff(ConfigFile("*.bm"));
	while(ff) {
		method.Add(GetFileTitle(ff.GetName()));
		ff.Next();
	}
	method <<= ide.method;
	mode <<= ide.targetmode;
	export_dir <<= ide.export_dir;
	debug.Load(ide.debug);
	LoadFromWorkspace(debug, "outputmode-debug");
	release.Load(ide.release);
	LoadFromWorkspace(release, "outputmode-release");
	SyncLock();
}

void OutMode::Save()
{
	ide.SetMethod(~method);
	ide.targetmode = ~mode;
	ide.export_dir = ~export_dir;
	debug.Save(ide.debug);
	StoreToWorkspace(debug, "outputmode-debug");
	release.Save(ide.release);
	StoreToWorkspace(release, "outputmode-release");
	StringStream ss;
	ide.SerializeOutputMode(ss);
	ide.recent_buildmode.GetAdd(ide.method) = ss.GetResult();
}

void OutMode::Export(int kind)
{
	String ep = ~export_dir;
	if(IsNull(ep)) {
		Exclamation("Missing output directory!");
		return;
	}
	if(kind == 2)
		ide.ExportMakefile(ep);
	else
		ide.ExportProject(ep, kind, true);
	Break(IDOK);
}

void OutMode::ConfigChange()
{
	if (config.GetIndex() < 0)
		return;
	ide.mainconfigparam = ~config;
}

class CmdBuildOptionsWindow : public WithCmdBuildOptionsLayout<TopWindow> {
private:
	String cmdAssembly, cmdBuildMode, cmdPackage, cmdMethod, cmdMainConfig;

	struct PathConvert : ConvertString {
		virtual int Filter(int chr) const
		{
			if (chr == '<' || chr == '>' || chr == '|' || chr == '*' || chr == '?' || chr == '\"') return Null;
			return ConvertString::Filter(chr);
		}
	};

public:
	typedef CmdBuildOptionsWindow CLASSNAME;

	CmdBuildOptionsWindow(const String& package, const String& method, const String& mainconfigparam, const String& output,
		int targetmode, int hydra1_threads, int linkmode, bool blitzbuild, bool createmap, bool verbosebuild) :
		cmdPackage(package), cmdMethod(method)
	{
		CtrlLayout(*this, t_("Command line options for building"));
		MinimizeBox().CloseBoxRejects();
		threads <<= hydra1_threads;
		threads.NotNull(false);
		cmdAssembly = GetVarsName();
		cmdBuildMode = (targetmode == 1 ? "r" : targetmode == 2 ? "1" : targetmode == 3 ? "2" : "d");
		if (mainconfigparam.GetCount()) {
			cmdMainConfig = mainconfigparam;
			cmdMainConfig.Replace(" ", ",");
			cmdMainConfig.Insert(0, " +");
		}
		if (blitzbuild) blitz <<= true;
		switch(linkmode) {
			case 1: shared <<= true;  break;
			case 2: sharedbuild <<= true; break;
		}
		exportproject <<= 0;
		map <<= createmap;
		verbose <<= verbosebuild;
		out <<= output;
		out.SetConvert(Single<PathConvert>());
		outoption <<= true;
		umk <<= true;

		rebuild.WhenAction = blitz.WhenAction = msgonfail.WhenAction = silent.WhenAction =
			map.WhenAction = verbose.WhenAction = makefile.WhenAction =
			savetargetdir.WhenAction = exportproject.WhenAction = umk.WhenAction =
			threads.WhenAction = out.WhenAction = outoption.WhenAction = THISBACK(GenerateCmd);
		shared.WhenAction = THISBACK(OnShared);
		sharedbuild.WhenAction << THISBACK(OnSharedBuild);
		btnCopy.WhenPush = callback(&cmd, &TextCtrl::Copy);
	}

	void OnShared()      { if (shared && sharedbuild) sharedbuild <<= false; GenerateCmd(); }
	void OnSharedBuild() { if (sharedbuild && shared) shared <<= false; GenerateCmd(); }

	void GenerateCmd()
	{
		String cmdBuild("-");
		if (rebuild) cmdBuild << 'a';
		if (blitz) cmdBuild << 'b';
		if (!umk && msgonfail) cmdBuild << 'e';
		if (silent) cmdBuild << 'l';
		if (map) cmdBuild << 'm';
		cmdBuild << cmdBuildMode;
		if (shared) cmdBuild << 's';
		else if (sharedbuild) cmdBuild << 'S';
		if (verbose) cmdBuild << 'v';
		if (savetargetdir) cmdBuild << 'k';
		switch (exportproject) {
			case 1: cmdBuild << 'x'; break;
			case 2: cmdBuild << 'X'; break;
		}
		if (makefile) cmdBuild << 'M';
		int numThreads = ~threads;
		if (umk && !IsNull(numThreads) && numThreads > 0)
			cmdBuild << 'H' << numThreads;
		String output;
		if (outoption) {
			output = TrimBoth(~out);
			if (output.Find(" ") >= 0)
				output = String().Cat() << "\"" << output << "\"";
		}
		cmd <<= TrimRight(Format("%s %s %s %s%s %s",
			cmdAssembly, cmdPackage, cmdMethod, cmdBuild, cmdMainConfig, output));
	}

	virtual void Serialize(Stream& s)
	{
		s % rebuild % msgonfail % silent % verbose % makefile % savetargetdir % exportproject
			% threads % outoption % umk ;
	}

	virtual bool Key(dword key, int count)
	{
		if (key == K_ESCAPE) {
			Close();
			return true;
		}
		return false;
	}
};

void OutMode::CmdOptions()
{
	const Workspace& wspc = ide.IdeWorkspace();
	int pi = wspc.GetCount() > 0 ? 0 : -1;
	if (pi < 0) {
		PromptOK("No main package");
		return;
	}
	VectorMap<String, String> bm = ide.GetMethodVars(~method);
	if (bm.GetCount() == 0) {
		PromptOK("Invalid build method");
		return;
	}
	One<Host> host = ide.CreateHost(false);
	One<Builder> b = ide.CreateBuilder(~host);
	const String& p = wspc[pi];
	String output = NativePath(ide.OutDir(ide.PackageConfig(wspc, pi, bm, ~config, *host, *b), p, bm, true));
	if (output.Right(1) == ".")
		output = output.Left(output.GetCount() - 1);
	const ModePane& pane = ~mode == 0 ? debug : release;
	int blitzpackage = pane.package.Get(0, 2);
	bool blitzbuild = !wspc.package[pi].noblitz && pane.blitz
		&& (IsNull(blitzpackage) ? true : blitzpackage);
	CmdBuildOptionsWindow window(p, ~method, ~config, output,
		~mode, ide.hydra1_threads, pane.linkmode, blitzbuild, pane.map, ide.console.verbosebuild);
	LoadFromGlobal(window, "CmdBuildOptionsWindow");
	window.GenerateCmd();
	window.Run();
	StoreToGlobal(window, "CmdBuildOptionsWindow");
}

bool MapFlag(const VectorMap<String, String>& map, const char *key)
{
	return map.Get(key, "0") == "1";
}

void Ide::SerializeOutputMode(Stream& s)
{
	int version = 1;
	s / version;
	s % method;
	s / targetmode;
	s % debug;
	s % release;
	s % recent_buildmode;
	if(version >= 1)
		s % export_dir;
}

void OutMode::SyncLock()
{
	bool b = GetMethodVars(~method).Get("LINKMODE_LOCK", "") != "1";
	release.linkmode.Enable(b);
	debug.linkmode.Enable(b);
}

void OutMode::Preset()
{
	int q = ide.recent_buildmode.Find(~method);
	SyncLock();
	if(q < 0) {
		VectorMap<String, String> map = GetMethodVars(~method);
		debug.linkmode = atoi(map.Get("DEBUG_LINKMODE", "0"));
		debug.debug = atoi(map.Get("DEBUG_INFO", "0"));
		debug.blitz = MapFlag(map, "DEBUG_BLITZ");
		release.linkmode = atoi(map.Get("RELEASE_LINKMODE", "0"));
		release.debug <<= 0;
		release.blitz = MapFlag(map, "RELEASE_BLITZ");
	}
	else {
		StringStream ss(ide.recent_buildmode[q]);
		TargetMode x;
		String dummy;
		int m;
		ss / m;
		ss % dummy;
		ss / m;
		mode = !!m;
		ss % x;
		debug.Load(x);
		ss % x;
		release.Load(x);
	}
}

OutMode::OutMode(Ide& ide)
:	ide(ide)
{
	CtrlLayoutOKCancel(*this, "Output mode");
	config <<= THISBACK(ConfigChange);
	method <<= THISBACK(Preset);
	cmd_options <<= THISBACK(CmdOptions);
	SyncLock();
	DirSel(export_dir, dsb);
	export_all <<= THISBACK1(Export, 1);
	export_used <<= THISBACK1(Export, 0);
	export_makefile <<= THISBACK1(Export, 2);
	reset_blitz <<= callback(ResetBlitz);
}

void Ide::SetupOutputMode()
{
	String prevmainconfig = mainconfigparam;
	SetupDefaultMethod();
	OutMode m(*this);
	m.Load();
	if(m.Execute() != IDOK) {
		mainconfigparam = prevmainconfig;
		return;
	}
	m.Save();
	if(prevmainconfig != mainconfigparam)
		SetMainConfigList();
	SyncBuildMode();
	SetBar();
}

void Ide::SyncBuildMode()
{
	SetupDefaultMethod();
	Vector<String> bmlist;
	for(FindFile ff(ConfigFile("*.bm")); ff; ff.Next())
		bmlist.Add(GetFileTitle(ff.GetName()));
	Sort(bmlist);
	methodlist.Clear();
	Append(methodlist, bmlist);
	String h = method + ' ';
	switch(targetmode) {
	case 1: h << "Optimal"; break;
	case 2: h << "Speed"; break;
	case 3: h << "Size"; break;
	default:
		h << "Debug";
	}
	buildmode <<= h;
}

void Ide::DropMethodList()
{
	methodlist.PopUp(&buildmode);
	int i = methodlist.Find(method);
	if(i >= 0)
		methodlist.SetCursor(i);
}

void Ide::SetMethod(const String& m)
{
	method = m;
	current_builder = GetMethodVars(method).Get("BUILDER", "");
	NewCodeBase();
}

void Ide::SelectMethod()
{
	SetMethod(methodlist.GetKey());
	int q = recent_buildmode.Find(~method);
	if(q >= 0) {
		StringStream ss(recent_buildmode[q]);
		SerializeOutputMode(ss);
	}
	SyncBuildMode();
	SetHdependDirs();
}

void Ide::DropModeList()
{
	modelist.PopUp(&buildmode);
	modelist.SetCursor(targetmode);
}

void Ide::SelectMode()
{
	targetmode = modelist.GetCursor();
	SyncBuildMode();
}

void Ide::SetupDefaultMethod()
{
	if(IsNull(method)) {
		SetMethod(GetDefaultMethod());
		if(IsNull(method)) {
			FindFile ff(ConfigFile("*.bm"));
			if(!ff)
				return;
			SetMethod(GetFileTitle(ff.GetName()));
		}
		VectorMap<String, String> map = GetMethodVars(method);
		debug.linkmode = atoi(map.Get("DEBUG_LINKMODE", "0"));
		debug.def.debug = atoi(map.Get("DEBUG_INFO", "0"));
		debug.def.blitz = MapFlag(map, "DEBUG_BLITZ");
		release.linkmode = atoi(map.Get("RELEASE_LINKMODE", "0"));
		release.def.debug <<= 0;
		release.def.blitz = MapFlag(map, "RELEASE_BLITZ");
	}
}
