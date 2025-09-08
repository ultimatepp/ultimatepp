#include "ide.h"

#ifdef _DEBUG
#define LSLOW()    // Sleep(20) // Simulate HD seeks to test package cache
#else
#define LSLOW()
#endif

void SelectPackageDlg::PackageMenu(Bar& menu)
{
	bool b = GetCurrentName().GetCount();
	menu.Add("New package..", [=] { OnNew(); });
	menu.Separator();
	if(IsExternalMode()) {
		menu.Add(b, "Delete package..", [=] {
			String p = PackageFile(GetCurrentName());
			if(FileExists(p) && PromptYesNo("Delete package?")) {
				DeleteFile(p);
				Load();
			}
		});
	}
	else {
		menu.Add(b, "Duplicate package..", [=] { RenamePackage(true); });
		menu.Add(b, "Rename package..", [=] { RenamePackage(false); });
		menu.Add(b, "Copy package to..", [=] { MovePackage(true); });
		menu.Add(b, "Move package to..", [=] { MovePackage(false); });
		menu.Add(b, "Delete package..", [=] { DeletePackage(); });
	}
	menu.Add(b, "Change description..", [=] { ChangeDescription(); });
	if(b) {
		menu.Separator();
		String dir = PackageDirectory(GetCurrentName());
		menu.Add(b, "Open package directory", [=] { ShellOpenFolder(dir); });
		menu.Add(b, "Terminal at package directory", [=] { TheIde()->LaunchTerminal(dir); });
	}
}

bool RenamePackageFs(const String& upp, const String& npf, const String& nupp, bool copy)
{
	String pf = GetFileFolder(upp);
	String temp_pf = AppendFileName(GetFileFolder(pf), AsString(Random()) + AsString(Random()));
	if(!FileMove(pf, temp_pf)) {
		Exclamation("Operation has failed.");
		return false;
	}
	RealizePath(GetFileFolder(npf));
	if(copy) {
		bool b = CopyFolder(npf, temp_pf);
		FileMove(temp_pf, pf);
		if(!b) {
			FileMove(temp_pf, pf);
			DeleteFolderDeep(npf);
			Exclamation("Duplicating package folder has failed.");
			return false;
		}
	}
	else
	if(!FileMove(temp_pf, npf)) {
		FileMove(temp_pf, pf);
		Exclamation("Renaming package folder has failed.");
		return false;
	}
	if(!FileMove(npf + "/" + GetFileName(upp), nupp)) {
		FileMove(npf, pf);
		Exclamation("Renaming .upp file has failed.");
		return false;
	}
	return true;
}

bool RenamePackageFs(const String& upp, const String& newname, bool duplicate)
{
	if(IsNull(newname)) {
		Exclamation("Wrong name.");
		return false;
	}
	String npf = AppendFileName(GetPackagePathNest(GetFileFolder(upp)), newname);
	String nupp = npf + "/" + GetFileName(newname) + ".upp";

	if(FileExists(nupp)) {
		Exclamation("Package [* \1" + newname + "\1] already exists!");
		return false;
	}

	return RenamePackageFs(upp, npf, nupp, duplicate);
}

void SelectPackageDlg::RenamePackage(bool duplicate)
{
	String n = GetCurrentName();
	if(IsNull(n))
		return;
again:
	if(!EditText(n, duplicate ? "Duplicate package:" : "Rename package", "Name", FilterPackageName))
		return;
	if(!RenamePackageFs(PackageFile(GetCurrentName()), n, duplicate))
		goto again;
	search <<= Null;
	Load(n);
	alist.ScrollIntoCursor();
}

void SelectPackageDlg::MovePackage(bool copy)
{
	if(IsExternalMode())
		return;

	WithMoveCopyPackageLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, copy ? "Copy package to" : "Move package to");

	String d0;
	for(int pass = 0; pass < 2; pass++) {
		Index<String> udir;
		FindFile ff(ConfigFile("*.var"));
		while(ff) {
			if(int(GetFileTitle(ff.GetName()) != base.GetKey()) == pass) {
				VectorMap<String, String> var;
				LoadVarFile(ff.GetPath(), var);
				for(String d : Split(var.Get("UPP", ""), ';'))
					if(DirectoryExists(d)) {
						udir.FindAdd(d);
						d0 = Nvl(d0, d);
					}
			}
			ff.Next();
		}

		Vector<String> sd = pick(udir.PickKeys());
		Sort(sd, [](const String& a, const String& b) { return ToUpper(a) < ToUpper(b); });
		for(String d : sd)
			dlg.dir.AddList(d);
	}

	dlg.dir <<= d0;
	DirSelect(dlg.dir, dlg.select);

	dlg.name <<= GetCurrentName();

again:
	if(dlg.Run() != IDOK)
		return;

	String dir = ~dlg.dir;
	if(!DirectoryExists(dir)) {
		Exclamation("Invalid target directory!");
		goto again;
	}
	String pkg = AppendFileName(dir, ~~dlg.name);
	if(DirectoryExists(pkg)) {
		Exclamation("Target package directory already exists!");
		goto again;
	}
	if(FileExists(pkg)) {
		Exclamation("Invalid target package directory - it is a file!");
		goto again;
	}

	if(!RenamePackageFs(PackageFile(GetCurrentName()), pkg, pkg + "/" + GetFileName(~~dlg.name) + ".upp", copy))
		goto again;

	Load(~~dlg.name);
}

void SelectPackageDlg::DeletePackage()
{
	String n = GetCurrentName();
	if(IsNull(n))
		return;
	String pp = PackageDirectory(GetCurrentName());
	if(!DirectoryExists(pp)) {
		Exclamation("Directory does not exist!");
		return;
	}
	if(!PromptYesNo("Do you really want to delete package [* \1" + GetCurrentName() + "\1]?&&"
	                "[/ Warning:] [* Package will not be removed "
	                "from uses of any other package!]"))
		return;
	if(!PromptYesNo("This operation is irreversible.&Do you really want to proceed?"))
		return;
	DeleteFolderDeep(pp);
	Load();
}

SelectPackageDlg::SelectPackageDlg(const char *title, bool selectvars_, bool main)
: selectvars(selectvars_)
{
	CtrlLayoutOKCancel(*this, title);
	Sizeable().Zoomable();
	Icon(IdeImg::MainPackage(), IdeImg::PackageLarge());
	base.AutoHideSb();
	base.NoGrid();
	base.AddIndex();
	base.AddColumn("Assembly");
	base.WhenCursor = THISBACK(OnBase);
	base.WhenBar = THISBACK(ToolBase);
	base.WhenLeftDouble = THISBACK(OnBaseEdit);
	ok.WhenAction = clist.WhenLeftDouble = alist.WhenLeftDouble = THISBACK(OnOK);
	cancel.WhenAction = WhenClose = THISBACK(OnCancel);
	clist.Columns(4);
	alist.AddColumn("Package").Add(3);
	alist.AddColumn("Nest");
	alist.AddColumn("Description");
	alist.AddIndex();
	alist.EvenRowColor();
	alist.SetLineCy(max(Zy(16), Draw::GetStdFontCy()));
	alist.ColumnWidths("108 79 317");
	list.Add(clist.SizePos());
	list.Add(alist.SizePos());

	lists_status.SetDefault("");
	lists_status.NoSizeGrip();
	lists_status.Height(Zy(10));
	lists_status.Add(progress.SizePos());

	parent.Add(list.SizePos());
	parent.AddFrame(splitter.Left(base, Zx(170)));

	recent.NoSb().NoHyperlinkDecoration();
	LoadLRU();
	if(main && selectvars && lru.GetCount()) {
		String text;
		for(int i = 0; i < lru.GetCount(); i++) {
			const auto& m = lru[i];
			MergeWith(text, ", ", "[@b^A" + AsString(i) + "^ \1" + m.a + "\1]" +
			                      ":_[*^ "+ AsString(i) + "^ \1" + m.b + "\1]");
		}
		if(text.GetCount())
			recent <<= "[gpw [@K/ Recent:] " + text;
		Rect r = recent.GetRect();
		Rect pg = recent.GetPage();
		RichText txt = ParseQTF(recent.GetQTF());
		int cy = 0;
		for(int i = 0; i < txt.GetLength(); i++) { // Find the height of text that completely fits
			Rect cr = txt.GetCaret(i, pg);
			if(cr.bottom > r.GetHeight()) // does not fit
				break;
			cy = cr.bottom;
		}
		r.bottom = r.top + cy;
		recent.SetRect(r);
		recent.WhenLink = [=](const String& s) {
			if(*s == 'A') {
				int i = Atoi(~s + 1);
				if(i >= 0 && lru.GetCount())
					base.FindSetCursor(lru[i].a);
			}
			else {
				int i = Atoi(s);
				if(i >= 0 && lru.GetCount()) {
					selected = lru[i].b;
					LoadVars(lru[i].a);
					selected_nest = GetPackagePathNest(PackageDirectory(selected));
					Break(IDYES);
				}
			}
		};
		LogPos p = parent.GetPos();
		p.y.SetA(r.bottom + DPI(3));
		parent.SetPos(p);
	}
	else {
		LogPos p = parent.GetPos();
		p.y.SetA(recent.GetPos().y.GetA());
		parent.SetPos(p);
	}

	if (!selectvars)
		splitter.Hide();

	newu << [=] { OnNew(); };
	kind.Add(MAIN, "Main packages");
	kind.Add(NONMAIN, "Non-main packages");
	kind.Add(ALL, "All packages");
	kind << [=] { OnFilter(); };
	kind <<= main ? MAIN : NONMAIN;
	nest << [=] { OnFilter(); };
	OnFilter();
	nest <<= main ? 0 : ALL;
	brief <<= THISBACK(SyncBrief);
	search.NullText("Search (Ctrl+K)", StdFont().Italic(), SColorDisabled());
	search << [=] { SyncList(Null); };
	search.SetFilter(CharFilterDefaultToUpperAscii);
	SyncBrief();
	ActiveFocus(brief ? (Ctrl&)clist : (Ctrl&)alist);
	clist.BackPaintHint();
	alist.BackPaintHint();
	base.BackPaintHint();
	loadi = 0;
	loading = false;
	clist.WhenBar = alist.WhenBar = THISBACK(PackageMenu);

	upphub.SetImage(IdeImg::UppHub());
	upphub << [=] {
		String p = UppHub();
		OnBase();
		if(p.GetCount()) {
			alist.FindSetCursor(p);
			clist.FindSetCursor(p);
		}
	};

	help << [&] { LaunchWebBrowser("https://www.ultimatepp.org/app$ide$PackagesAssembliesAndNests$en-us.html"); };

	String exf = VarFilePath("[external]");
	if(!FileExists(exf))
		SaveFile(exf, "SOURCE_MASKS = \"*.cpp *.h *.hpp *.c *.cxx *.cc *.m *.mm *.cs *.java *.js *.ts *.tsx *.jsx\";");
}

String SelectPackageDlg::LRUFilePath()
{
	return ConfigFile("main.lru");
}

void SelectPackageDlg::LoadLRU()
{
	LoadFromFile(lru, LRUFilePath());
	Index<Tuple<String, String>> lu; // prevent duplicates, \ vs /
	for(auto h : lru) {
		h.b = UnixPath(h.b);
		lu.FindAdd(h);
	}
	lru = lu.PickKeys();
}

void SelectPackageDlg::StoreLRU(const String& p)
{
	auto q = Tuple(GetVarsName(), p);
	LoadLRU();
	int i = FindIndex(lru, q);
	if(i >= 0)
		lru.Remove(i);
	lru.Insert(0, q);
	if(lru.GetCount() > 100)
		lru.Trim(100);
	StoreToFile(lru, LRUFilePath());
}

void SelectPackageDlg::SyncFilter()
{
	Value n = ~nest;
	nest.ClearList();
	Vector<String> upp = GetUppDirsRaw();
	nest.Add(ALL, AttrText("All").Italic().Bold().NormalInk(SBlue()));
	bool hub = false;
	for(int i = 0; i < upp.GetCount(); i++) {
		if(upp[i].StartsWith(GetHubDir()) && !hub) {
			nest.Add(UPPHUB|i, AttrText("UppHub").Italic().NormalInk(SBlue()));
			hub = true;
		}
		String fn = GetFileName(upp[i]);
		if(hub)
			nest.Add(i, AttrText(fn).NormalInk(SCyan()));
		else
			nest.Add(i, fn);
	}
	if(nest.HasKey(n))
		nest <<= n;
	else
		nest.GoBegin();
}

bool SelectPackageDlg::Key(dword key, int count)
{
	if(key == K_ALT_ENTER) {
		ChangeDescription();
		return true;
	}
	else if(key == K_CTRL_K) {
		search.SetFocus();
		return true;
	}
	if((clist.HasFocus() || alist.HasFocus()) && search.Key(key, count))
		return true;
	return TopWindow::Key(key, count);
}

void SelectPackageDlg::Serialize(Stream& s)
{
	SerializePlacement(s);
	s % brief;
}

String SelectPackageDlg::GetCurrentName()
{
	String n;
	if(clist.IsShown())
		n = clist.GetCurrentName();
	else
	if(alist.IsCursor())
		n = alist.Get(0);
	else
		return Null;
	return n;
}

String SelectPackageDlg::GetCurrentNest()
{
	int i = clist.IsShown() ? clist.GetCursor() : alist.GetCursor();
	String f = GetCurrentName();
	return i >= 0 && i < nest_list.GetCount() ? nest_list[i] : String();
}

int   SelectPackageDlg::GetCurrentIndex()
{
	String s = GetCurrentName();
	for(int i = 0; i < packages.GetCount(); i++)
		if(packages[i].package == s)
			return i;
	return -1;
}

void SelectPackageDlg::ChangeDescription()
{
	int ii = GetCurrentIndex();
	if(ii >= 0 && ii < packages.GetCount()) {
		PkInfo& p = packages[ii];
		WithDescriptionLayout<TopWindow> dlg;
		CtrlLayoutOKCancel(dlg, "Package description");
		String pp = PackageFile(p.package);
		Package pkg;
		if(!pkg.Load(pp) && !IsExternalMode()) {
			Exclamation("Package does not exist.");
			return;
		}
		dlg.text <<= pkg.description;
		if(dlg.Run() != IDOK)
			return;
		pkg.description = ~dlg.text;
		pkg.Save(pp);
		p.description = ~dlg.text;
		if(alist.IsCursor())
			alist.Set(2, ~dlg.text);
	}
}

void SelectPackageDlg::SyncBrief()
{
	bool b = brief;
	alist.Show(!b);
	clist.Show(b);
}

String SelectPackageDlg::Run(String& nest, String startwith)
{
	finished = canceled = false;
	if(!IsSplashOpen())
		Open();
	if(selectvars)
		SyncBase(GetVarsName());
	else
		OnBase();
	String bkvar = GetVarsName();
	if(finished)
		return GetCurrentName();
	if(canceled)
		return Null;
	alist.FindSetCursor(startwith);
	clist.FindSetCursor(startwith);
	ActiveFocus(alist.IsShown() ? (Ctrl&)alist : (Ctrl&)clist);
	switch(TopWindow::Run()) {
	case IDOK:
		nest = GetCurrentNest();
		return GetCurrentName();
	case IDYES:
		nest = selected_nest;
		return selected;
	default:
		LoadVars(bkvar);
		SyncFilter();
		SyncBase(GetVarsName());
		return Null;
	}
}

void SelectPackageDlg::OnOK()
{
	Package pkg;
	int fk = ~kind;
	String n = GetCurrentName();
	if(IsExternalMode() && (clist.IsShown() && clist.IsCursor() || alist.IsCursor())
	   ||
	   n.GetCount() && pkg.Load(PackageFile(n)) &&
	   (!(fk == MAIN) || pkg.config.GetCount()) &&
	   (!(fk == NONMAIN) || !pkg.config.GetCount())) {
		loading = false;
		finished = true;
		AcceptBreak(IDOK);
	}
}

void SelectPackageDlg::OnCancel()
{
	loading = false;
	canceled = true;
	RejectBreak(IDCANCEL);
}

void SelectPackageDlg::OnFilter()
{
	SyncList(Null);
}

String SelectExternalPackage(const String& from)
{
	FileSel fs;
	fs.ActiveDir(GetHomeDirectory());
	LoadFromGlobal(fs, "PackageDirSelector");
	if(from.GetCount())
		fs.ActiveDir(from);
	bool b = fs.ExecuteSelectDir("Select package directory");
	StoreToGlobal(fs, "PackageDirSelector");
	return b ? ~fs : String::GetVoid();
}

void SelectPackageDlg::OnBase()
{
	if(!finished && !canceled) {
		SyncFilter();
		if(splitter.IsShown())
			nest <<= nest.GetCount() ? 0 : ALL;
		Load();
		newu.SetLabel(IsExternalMode() ? "New project" : "New package");
	}
}

void SelectPackageDlg::OnNew() {
	if(IsExternalMode()) {
		String n = SelectExternalPackage();
		if(IsNull(n))
			return;
		String f = PackageFile(n);
		if(!FileExists(f)) {
			Package p;
			p.config.Add();
			p.Save(f);
		}
		selected = n;
		selected_nest.Clear();
		Break(IDYES);
		return;
	}
	TemplateDlg dlg;
	LoadFromGlobal(dlg, "NewPackage");
	dlg.Load(GetUppDirsRaw(), ~kind == MAIN);
	while(dlg.Run() == IDOK) {
		String nest = ~dlg.nest;
		String name = NativePath(String(~dlg.package));
		String path = AppendFileName(AppendFileName(nest, name), GetFileName(name) + ".upp");
		if(FileExists(path) && !PromptYesNo("Package [* \1" + path + "\1] already exists.&"
		                                    "Do you wish to recreate the files?"))
			continue;
		RealizePath(path);
		if(!SaveFile(path, Null)) {
			Exclamation("Error writing the file [* \1" + path + "\1].");
			continue;
		}
		dlg.Create();
		selected_nest = nest;
		selected = name;
		Break(IDYES);
		break;
	}
	StoreToGlobal(dlg, "NewPackage");
}

Vector<String> SelectPackageDlg::GetRepoDirs()
{
	Vector<String> r;
	Vector<String> dirs = SplitDirs(GetVar("UPP"));
	for(int i = 0; i < dirs.GetCount(); i++) {
		String d = NormalizePath(dirs[i]);
		if(GetRepoKind(d))
			r.Add(d);
	}
	return r;
}

void SelectPackageDlg::SyncRepoDir(const String& dir)
{
	RepoSyncDirs(Vector<String>() << dir);
	Load();
}

void SelectPackageDlg::SyncRepoDirs()
{
	RepoSyncDirs(GetRepoDirs());
	Load();
}

void SelectPackageDlg::ToolBase(Bar& bar)
{
	bar.Add("New assembly..", THISBACK(OnBaseAdd))
#ifdef PLATFORM_COCOA
		.Key(K_CTRL_N)
#else
		.Key(K_INSERT)
#endif
	;
	bar.Add(base.IsCursor(), "Edit assembly..", THISBACK(OnBaseEdit))
		.Key(K_CTRL_ENTER);
	if(!IsExternalMode()) {
		bar.Add(base.IsCursor(), "Remove assembly..", THISBACK(OnBaseRemove))
			.Key(K_CTRL_DELETE);
		bar.Add("Purge assemblies..", [=] { RemoveInvalid(); });
		Vector<String> dirs = SplitDirs(GetVar("UPP"));
		if(dirs.GetCount()) {
			bar.Separator();
			for(String s : dirs)
				bar.Add("Terminal at " + s, [=] { TheIde()->LaunchTerminal(s); });
		}
		Vector<String> d = GetRepoDirs();
		if(HasGit()) {
			bar.Separator();
			bar.Add("Clone U++ GitHub sources..", [=] {
				String vars = base.Get(0);
				SetupGITMaster();
				SyncBase(vars);
			});
		}
		if(d.GetCount()) {
			bar.Separator();
			for(int i = 0; i < d.GetCount(); i++)
				bar.Add("Synchronize " + d[i], IdeImg::svn_dir(), THISBACK1(SyncRepoDir, d[i]));
			bar.Add("Synchronize everything..", IdeImg::svn(), THISBACK(SyncRepoDirs));
		}
	}
}

void SelectPackageDlg::OnBaseAdd()
{
	String vars;
	if(BaseSetup(vars))
		SyncBase(vars);
}

void SelectPackageDlg::OnBaseEdit()
{
	if(!base.IsCursor())
		return;
	if(IsExternalMode()) {
		WithExtSetupLayout<TopWindow> dlg;
		CtrlLayoutOKCancel(dlg, "External mode settings");
		dlg.source_masks <<= GetVar("SOURCE_MASKS");
		if(dlg.ExecuteOK())
			SetVar("SOURCE_MASKS", ~dlg.source_masks);
		return;
	}
	String vars = base.Get(0), oldvars = vars;
	if(BaseSetup(vars)) {
		if(vars != oldvars)
			DeleteFile(VarFilePath(oldvars));
		DeleteFile(CachePath(vars));
		SyncBase(vars);
	}
}

void SelectPackageDlg::OnBaseRemove()
{
	int c = base.GetCursor();
	if(c < 0)
		return;
	String next;
	if(c + 1 < base.GetCount())
		next = base.Get(c + 1);
	else if(c > 0)
		next = base.Get(c - 1);
	String vars = base.Get(0);
	String varpath = VarFilePath(vars);
	if(PromptOKCancel(Format("Remove base file [* \1%s\1]?", varpath))) {
		if(!FileDelete(varpath))
			Exclamation(Format("Error deleting file [* \1%s\1].", varpath));
		else
			SyncBase(next);
	}
}

void SelectPackageDlg::RemoveInvalid()
{
	String vars = base.GetKey();
	WithRemoveInvalidAssembliesLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Remove assemblies");
	dlg.list.AddColumn("Remove")
	        .Ctrls([=](int, One<Ctrl> &c) { c.Create<Option>().NoWantFocus(); });
	dlg.list.AddColumn("Assembly");
	dlg.list.AddColumn("Error", 7);
	dlg.list.ColumnWidths("53 125 499");
	Vector<String> oks;
	for(int i = 0; i < base.GetCount(); i++) {
		String vars = base.Get(i, 0);
		VectorMap<String, String> var;
		LoadVarFile(VarFilePath(vars), var);
		Vector<String> dirs = Split(var.Get("UPP", ""), ';');
		String missing;
		for(String d : dirs)
			if(!DirectoryExists(d)) {
				MergeWith(missing, ", ", d);
				break;
			}
		if(dirs.GetCount() == 0)
			missing = "Empty";
		if(missing.GetCount())
			dlg.list.Add(true, vars, missing);
		else
			oks.Add(vars);
	}
	for(String s : oks)
		dlg.list.Add(false, s);
again:
	if(dlg.Run() != IDOK)
		return;
	int n = 0;
	for(int i = 0; i < dlg.list.GetCount(); i++)
		if((bool)dlg.list.Get(i, 0))
			n++;
	if(n) {
		if(!PromptYesNo("Remove " + AsString(n) + " assemblies?"))
			goto again;
		for(int i = 0; i < dlg.list.GetCount(); i++)
			if((bool)dlg.list.Get(i, 0))
				DeleteFile(VarFilePath(~dlg.list.Get(i, 1)));
	}
	SyncBase(vars);
}

int DirSep(int c)
{
	return c == '\\' || c == '/' ? c : 0;
}

struct PackageDisplay : Display {
	Font fnt;

	virtual Size GetStdSize(const Value& q) const {
		ValueArray va = q;
		Size sz = GetTextSize(String(va[0]), fnt);
		sz.cx += Zx(20);
		sz.cy = max(sz.cy, Zy(16));
		return sz;
	}

	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const {
		ValueArray va = q;
		String txt = va[0];
		Image icon = va[1];
		if(IsNull(icon))
			icon = IdeImg::Package();
		else
			icon = DPI(icon, 16);
		w.DrawRect(r, paper);
		w.DrawImage(r.left, r.top + (r.Height() - icon.GetHeight()) / 2, icon);
		w.DrawText(r.left + DPI(20), r.top + (r.Height() - Draw::GetStdFontCy()) / 2, txt, fnt, ink);
	}

	PackageDisplay() { fnt = StdFont(); }
};

void SelectPackageDlg::SyncList(const String& find)
{
	String n = Nvl(find, GetCurrentName());
	int asc = alist.GetScroll();
	int csc = clist.GetSbPos();

	packages.Clear();
	String s = ~search;
	int fn = ~nest;
	int fk = ~kind;
	Index<String> added;
	int from = 0;
	int to = data.GetCount() - 1;
	if(fn & UPPHUB)
		from = fn & NEST_MASK;
	else
	if(fn != ALL)
		from = to = fn & NEST_MASK;
	if(to < data.GetCount())
		for(int i = from; i <= to; i++) {
			const ArrayMap<String, PkData>& nest = data[i];
			for(int i = 0; i < nest.GetCount(); i++) {
				const PkData& d = nest[i];
				if(!nest.IsUnlinked(i) &&
				   d.ispackage &&
				   (!(fk == MAIN) || d.main) &&
				   (!(fk == NONMAIN) || !d.main) &&
				   ToUpper(d.package + d.description).Find(s) >= 0 &&
				   added.Find(d.package) < 0) {
					packages.Add() = d;
					if(!d.main)
						added.Add(d.package);
				}
			}
		}
	Sort(packages);
	alist.Clear();
	clist.Clear();
	nest_list.Clear();
	static PackageDisplay pd, bpd;
	bpd.fnt.Bold();
	for(int i = 0; i < packages.GetCount(); i++) {
		const PkInfo& pkg = packages[i];
		Image icon = pkg.icon;
		if(IsNull(icon)) {
			if(pkg.main)
				icon = pkg.upphub ? IdeImg::HubMainPackage() : IdeImg::MainPackage();
			else
				icon = pkg.upphub ? IdeImg::HubPackage() : IdeImg::Package();
		}
		nest_list.Add(pkg.nest);
		clist.Add(pkg.package, DPI(icon, 16));
		alist.Add(pkg.package, GetFileName(pkg.nest), pkg.description, icon);
		alist.SetDisplay(alist.GetCount() - 1, 0, pkg.main ? bpd : pd);
	}
	if(!alist.FindSetCursor(n))
		alist.GoBegin();
	if(!clist.FindSetCursor(n) && clist.GetCount())
		clist.SetCursor(0);
	alist.ScrollTo(asc);
	clist.SetSbPos(csc);
	alist.HeaderTab(0).SetText("Package (" + AsString(alist.GetCount()) + ")");
}

void SelectPackageDlg::ScanFolder(const String& path, ArrayMap<String, PkData>& nd,
                                  const String& nest, Index<String>& dir_exists,
                                  const String& prefix)
{
	for(FindFile ff(AppendFileName(path, "*.*")); ff; ff.Next())
		if(ff.IsFolder() && !ff.IsHidden()) {
			String p = ff.GetPath();
			dir_exists.Add(p);
			bool nw = nd.Find(p) < 0; // Do we have any info loaded about this package?
			PkData& d = nd.GetAdd(p);
			d.package = prefix + ff.GetName();
			d.nest = nest;
			d.upphub = InUppHub(p);
			if(nw) { // No cached info available about the folder
				d.ispackage = IsLetter(*d.package) && d.package.Find('.') < 0; // First heuristic guess
				d.main = d.ispackage && prefix.GetCount() == 0; // Expect it is main
			}
		}
}

String SelectPackageDlg::CachePath(const char *vn) const
{
	return AppendFileName(ConfigFile("cfg"), String(vn) + ".pkg_cache_new");
}

void SelectPackageDlg::Load(const String& find)
{
	SyncFilter();
	if(selectvars && !base.IsCursor())
		return;
	if(loading) { // If we are called recursively from ProcessEvents, stop current loading and change loadi
		loadi++;
		loading = false;
		return;
	}
	int current_loadi = -1;
	while(current_loadi != loadi) {
		current_loadi = loadi;
		if(selectvars) {
			String assembly = (String)base.Get(0);
			list.Enable(base.IsCursor());
			if(!base.IsCursor())
				return;
			LoadVars(assembly);
			if(GetVar("_all") == "1")
				nest <<= ALL;
			else
				nest <<= 0;
			if(alist_external != IsExternalMode()) {
				alist_external = IsExternalMode();
				alist.ColumnWidths(alist_external ? "300 0 200": "108 79 317");
			}
			alist.HeaderTab(1).Show(!alist_external);
			SyncFilter();
		}
		Vector<String> upp = GetUppDirsRaw();
		packages.Clear();
		list.AddFrame(lists_status);
		loading = true;
		data.Clear();
		Index<String> dir_exists;
		String cache_path = CachePath(GetVarsName());
		if(IsExternalMode()) {
			upp << "";
			for(FindFile ff(ConfigFile("external") + "/*.*"); ff; ff.Next())
				if(ff.IsFile()) {
					String dir = DecodePathFromFileName(ff.GetName());
					if(DirectoryExists(dir)) {
						PkData& d = data.At(0).GetAdd(dir);
						d.package = dir;
						d.ispackage = true;
					}
				}
		}
		else {
			LoadFromFile(data, cache_path);
			data.SetCount(upp.GetCount());
			for(int i = 0; i < upp.GetCount(); i++) // Scan nest folders for subfolders (additional package candidates)
				ScanFolder(upp[i], data[i], upp[i], dir_exists, Null);
		}
		int update = msecs();
		for(int i = 0; i < data.GetCount() && loading; i++) { // Now investigate individual sub folders
			ArrayMap<String, PkData>& nest = data[i];
			String nest_dir = NormalizePath(upp[i]);
			for(int i = 0; i < nest.GetCount() && loading; i++) {
				if(msecs(update) >= 100) { // each 100 ms update the list (and open select dialog after splash screen is closed)
					if(!IsSplashOpen() && !IsOpen())
						Open();
					progress++;
					SyncList(find);
					update = msecs();
				}
				ProcessEvents(); // keep GUI running

				PkData& d = nest[i];
				String path = nest.GetKey(i);
				if((IsExternalMode() || NormalizePath(path).StartsWith(nest_dir)) && DirectoryExists(path)) {
					String upp_path = PackageFile(d.package);
					LSLOW(); // this is used for testing only, normally it is NOP
					Time tm = FileGetTime(upp_path);
					if(IsNull(tm)) // .upp file does not exist - not a package
						d.ispackage = false;
					else
					if(tm != d.tm) { // cached info is outdated
						Package p;
						if(p.Load(upp_path)) {
							d.description = p.description;
							d.main = p.config.GetCount();
							d.tm = tm;
							d.ispackage = true;
						}
						else
							d.ispackage = false;
					}
					else
						d.ispackage = true;

					if(d.ispackage) {
						String icon_path;
						if(IsUHDMode())
							icon_path = AppendFileName(path, "icon32x32.png");
						if(IsNull(icon_path) || !FileExists(icon_path))
							icon_path = AppendFileName(path, "icon16x16.png");
						tm = FileGetTime(icon_path);
						if(IsNull(tm)) // package icon does not exist
							d.icon = Null;
						else
						if(tm != d.itm || d.icon.GetSize().cx != DPI(16)) { // chached package icon outdated
							d.icon = StreamRaster::LoadFileAny(icon_path);
							d.itm = tm;
						}
					}

					ScanFolder(path, nest, d.nest, dir_exists, d.package + '/');
				}
				else
					nest.Unlink(i); // cached folder was deleted or is not in nest dir
			}
			nest.Sweep();
		}

		if(!IsExternalMode())
			StoreToFile(data, cache_path);
		list.RemoveFrame(lists_status);
		while(IsSplashOpen())
			ProcessEvents();
		if(!IsOpen())
			Open();
		if(loading) {
			loading = false;
			SyncList(find);
		}
	}
}

void SelectPackageDlg::SyncBase(String initvars)
{
	Vector<String> varlist;
	for(FindFile ff(ConfigFile("*.var")); ff; ff.Next())
		if(ff.IsFile()) {
			String n = ff.GetName();
			varlist.Add(GetFileTitle(n));
		}

	Sort(varlist, &PackageLess);
	base.Clear();
	for(String s : varlist)
		base.Add(s, s);
	base.HeaderTab(0).SetText("Assembly (" + AsString(base.GetCount()) + ")");
	if(!base.FindSetCursor(initvars)) {
		if(base.GetCount() > 0)
			base.SetCursor(0);
		else
			OnBase();
	}
}

bool SelectPackageDlg::Pless(const SelectPackageDlg::PkInfo& a, const SelectPackageDlg::PkInfo& b)
{
	return PackageLess(a.package, b.package);
}

INITBLOCK
{
	RegisterGlobalConfig("SelectPkgMain");
	RegisterGlobalConfig("SelectPkg");
}

String SelectPackage(String& nest, const char *title, const char *startwith, bool selectvars, bool main)
{
	SelectPackageDlg dlg(title, selectvars, main);
	const char *c = main ? "SelectPkgMain" : "SelectPkg";
	LoadFromGlobal(dlg, c);
	dlg.SyncBrief();
	dlg.SyncFilter();
	dlg.CenterScreen();
	String b = dlg.Run(nest, startwith);
	StoreToGlobal(dlg, c);
	if(main && selectvars && b.GetCount())
		dlg.StoreLRU(b);

	return b;
}

String SelectPackage(const char *title, const char *startwith, bool selectvars, bool main)
{
	String dummy;
	return SelectPackage(dummy, title, startwith, selectvars, main);
}

