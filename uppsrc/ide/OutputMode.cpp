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

	typedef OutMode CLASSNAME;

	OutMode(Ide& ide);
};

void OutMode::Load()
{
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
	ide.method = ~method;
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
	method <<= THISBACK(Preset);
	SyncLock();
	DirSel(export_dir, dsb);
	export_all <<= THISBACK1(Export, 1);
	export_used <<= THISBACK1(Export, 0);
	export_makefile <<= THISBACK1(Export, 2);
}

void Ide::SetupOutputMode()
{
	SetupDefaultMethod();
	OutMode m(*this);
	m.Load();
	if(m.Execute() != IDOK)
		return;
	m.Save();
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

void Ide::SelectMethod()
{
	method = methodlist.GetKey();
	int q = recent_buildmode.Find(~method);
	if(q >= 0) {
		StringStream ss(recent_buildmode[q]);
		SerializeOutputMode(ss);
	}
	SyncBuildMode();
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
		method = GetDefaultMethod();
		if(IsNull(method)) {
			FindFile ff(ConfigFile("*.bm"));
			if(!ff)
				return;
			method = GetFileTitle(ff.GetName());
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
