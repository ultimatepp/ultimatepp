#include "ide.h"

#ifdef _DEBUG
#define LSLOW()    // Sleep(20) // Simulate HD seeks to test package cache
#else
#define LSLOW()
#endif

void SelectPackageDlg::PackageMenu(Bar& menu)
{
	bool b = GetCurrentName().GetCount();
	menu.Add(b, "Rename package..", THISBACK(RenamePackage));
	menu.Add(b, "Delete package", THISBACK(DeletePackage));
}

bool RenamePackageFs(const String& upp, const String& newname)
{
	if(IsNull(newname)) {
		Exclamation("Wrong name.");
		return false;
	}
	if(FileExists(PackagePath(newname))) {
		Exclamation("Package [* \1" + newname + "\1] already exists!");
		return false;
	}
	String pf = GetFileFolder(upp);
	String npf = GetFileFolder(pf) + "/" + newname;
	if(!FileMove(pf, npf)) {
		Exclamation("Renaming package folder has failed.");
		return false;
	}
	if(!FileMove(npf + "/" + GetFileName(upp), npf + "/" + newname + ".upp")) {
		FileMove(npf, pf);
		Exclamation("Renaming .upp file has failed.");
		return false;
	}
	return true;
}

void SelectPackageDlg::RenamePackage()
{
	String n = GetCurrentName();
	if(IsNull(n))
		return;
	WithRenamePackageLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Rename package");
	dlg.name.SetFilter(FilterPackageName);
	dlg.name <<= n;
	dlg.name.SelectAll();
again:
	if(dlg.Execute() != IDOK)
		return;
	if(!RenamePackageFs(PackagePath(GetCurrentName()), ~dlg.name))
		goto again;
	Load();
}

void SelectPackageDlg::DeletePackage()
{
	String n = GetCurrentName();
	if(IsNull(n))
		return;
	String pp = GetFileFolder(PackagePath(GetCurrentName()));
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
	base.AddColumn("Assembly");
	base.WhenCursor = THISBACK(OnBase);
	base.WhenBar = THISBACK(ToolBase);
	base.WhenLeftDouble = THISBACK(OnBaseEdit);
	ok <<= clist.WhenLeftDouble = alist.WhenLeftDouble = THISBACK(OnOK);
	cancel <<= WhenClose = THISBACK(OnCancel);
	clist.Columns(4);
	clist.WhenEnterItem = clist.WhenKillCursor = THISBACK(ListCursor);
	alist.AddColumn("Package").Add(3);
	alist.AddColumn("Nest");
	alist.AddColumn("Description");
	alist.AddIndex();
	alist.ColumnWidths("108 79 317");
	alist.WhenCursor = THISBACK(ListCursor);
	alist.EvenRowColor();
	alist.SetLineCy(max(Zy(16), Draw::GetStdFontCy()));
	list.Add(clist.SizePos());
	list.Add(alist.SizePos());
	splitter.Horz(base, list);
	splitter.SetPos(2000);
	splitter.Zoom(selectvars ? -1 : 1);
	newu <<= THISBACK(OnNew);
	filter <<= THISBACK(OnFilter);
	filter.Add(MAIN|FIRST, "Main packages of first nest");
	filter.Add(MAIN, "All main packages");
	filter.Add(FIRST, "All packages of first nest");
	filter.Add(0, "All packages");
	filter <<= main ? MAIN|FIRST : 0;
	progress.Hide();
	brief <<= THISBACK(SyncBrief);
	search.NullText("Search (Ctrl+K)", StdFont().Italic(), SColorDisabled());
	search <<= THISBACK(SyncList);
	search.SetFilter(CharFilterDefaultToUpperAscii);
	SyncBrief();
	description.NullText("Package description (Alt+Enter)", StdFont().Italic(), SColorDisabled());
	description <<= THISBACK(ChangeDescription);
	ActiveFocus(brief ? (Ctrl&)clist : (Ctrl&)alist);
	clist.BackPaintHint();
	alist.BackPaintHint();
	base.BackPaintHint();
	loadi = 0;
	loading = false;
	clist.WhenBar = alist.WhenBar = THISBACK(PackageMenu);
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
	if(clist.IsShown())
		return clist.GetCurrentName();
	else
	if(alist.IsCursor())
		return alist.Get(0);
	return Null;
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
		String pp = PackagePath(p.package);
		Package pkg;
		if(!pkg.Load(pp)) {
			Exclamation("Package does not exist.");
			return;
		}
		dlg.text <<= pkg.description;
		if(dlg.Run() != IDOK)
			return;
		pkg.description = ~dlg.text;
		pkg.Save(pp);
		p.description = description <<= ~dlg.text;
		if(alist.IsCursor())
			alist.Set(2, ~dlg.text);
	}
}

void SelectPackageDlg::ListCursor()
{
	int c = GetCurrentIndex();
	if(c >= 0 && c < packages.GetCount()) {
		String pp = PackagePath(GetCurrentName());
		Package pkg;
		pkg.Load(pp);
		description <<= pkg.description;
	}
	else
		description <<= Null;
}

void SelectPackageDlg::SyncBrief()
{
	bool b = brief;
	alist.Show(!b);
	clist.Show(b);
}

String SelectPackageDlg::Run(String startwith)
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
	case IDOK:  return GetCurrentName();
	case IDYES: return selected;
	default:
		LoadVars(bkvar);
		SyncBase(GetVarsName());
		return Null;
	}
}

void SelectPackageDlg::OnOK()
{
	Package pkg;
	int f = ~filter;
	String n = GetCurrentName();
	if(n.GetCount() && pkg.Load(PackagePath(n)) &&
	   (!(f & MAIN) || pkg.config.GetCount())) {
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
	SyncList();
}

void SelectPackageDlg::OnBase()
{
	if(!finished && !canceled)
		Load();
}

void SelectPackageDlg::OnNew() {
	TemplateDlg dlg;
	LoadFromGlobal(dlg, "NewPackage");
	int f = ~filter;
	dlg.Load(GetUppDirs(), f & MAIN);
	while(dlg.Run() == IDOK) {
		String nest = ~dlg.nest;
		String name = NativePath(String(~dlg.package));
		String path = AppendFileName(nest, AppendFileName(name, GetFileName(name) + ".upp"));
		if(FileExists(path) && !PromptYesNo("Package [* \1" + path + "\1] already exists.&"
		                                    "Do you wish to recreate the files?"))
			continue;
		RealizePath(path);
		if(!SaveFile(path, Null)) {
			Exclamation("Error writing the file [* \1" + path + "\1].");
			continue;
		}
		dlg.Create();
		selected = name;
		Break(IDYES);
		break;
	}
	StoreToGlobal(dlg, "NewPackage");
}

Vector<String> SelectPackageDlg::GetSvnDirs()
{
	Vector<String> r;
	Vector<String> dirs = SplitDirs(GetVar("UPP"));
	for(int i = 0; i < dirs.GetCount(); i++) {
		String d = NormalizePath(dirs[i]);
		if(IsSvnDir(d))
			r.Add(d);
	}
	return r;
}

void SelectPackageDlg::SyncSvnDir(const String& dir)
{
	SvnSyncDirs(Vector<String>() << dir);
	Load();
}

void SelectPackageDlg::SyncSvnDirs()
{
	SvnSyncDirs(GetSvnDirs());
	Load();
}

void SelectPackageDlg::ToolBase(Bar& bar)
{
	bar.Add("New assembly..", THISBACK(OnBaseAdd))
		.Key(K_INSERT);
	bar.Add(base.IsCursor(), "Edit assembly..", THISBACK(OnBaseEdit))
		.Key(K_CTRL_ENTER);
	bar.Add(base.IsCursor(), "Remove assembly", THISBACK(OnBaseRemove))
		.Key(K_CTRL_DELETE);
	Vector<String> d = GetSvnDirs();
	if(d.GetCount()) {
		bar.Separator();
		for(int i = 0; i < d.GetCount(); i++)
			bar.Add("Synchronize " + d[i], IdeImg::svn_dir(), THISBACK1(SyncSvnDir, d[i]));
		bar.Add("Synchronize everything..", IdeImg::svn(), THISBACK(SyncSvnDirs));
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
	if(PromptOKCancel(NFormat("Remove base file [* \1%s\1]?", varpath))) {
		if(!FileDelete(varpath))
			Exclamation(NFormat("Error deleting file [* \1%s\1].", varpath));
		else
			SyncBase(next);
	}
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
		sz.cx += Ctrl::Zx(20);
		sz.cy = max(sz.cy, Ctrl::Zy(16));
		return sz;
	}

	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const {
		ValueArray va = q;
		String txt = va[0];
		Image icon = va[1];
		if(IsNull(icon))
			icon = IdeImg::Package();
		icon = DPI(icon);
		w.DrawRect(r, paper);
		w.DrawImage(r.left, r.top + (r.Height() - icon.GetHeight()) / 2, icon);
		w.DrawText(r.left + Ctrl::Zx(20), r.top + (r.Height() - Draw::GetStdFontCy()) / 2, txt, fnt, ink);
	}

	PackageDisplay() { fnt = StdFont(); }
};

void SelectPackageDlg::SyncList()
{
	String n = GetCurrentName();
	int asc = alist.GetScroll();
	int csc = clist.GetSbPos();

	packages.Clear();
	String s = ~search;
	int f = ~filter;
	Index<String> added;
	for(int i = 0; i < min((f & FIRST) ? 1 : data.GetCount(), data.GetCount()); i++) {
		const ArrayMap<String, PkData>& nest = data[i];
		for(int i = 0; i < nest.GetCount(); i++) {
			const PkData& d = nest[i];
			if(!nest.IsUnlinked(i) &&
			   d.ispackage &&
			   (!(f & MAIN) || d.main) &&
			   ToUpper(d.package + d.description + d.nest).Find(s) >= 0 &&
			   added.Find(d.package) < 0) {
				packages.Add() = d;
				added.Add(d.package);
			}
		}
	}
	Sort(packages);
	alist.Clear();
	clist.Clear();
	ListCursor();
	static PackageDisplay pd, bpd;
	bpd.fnt.Bold();
	for(int i = 0; i < packages.GetCount(); i++) {
		const PkInfo& pkg = packages[i];
		Image icon = pkg.icon;
		if(IsNull(icon))
			icon = pkg.main ? IdeImg::MainPackage() : IdeImg::Package();
		clist.Add(pkg.package, icon);
		alist.Add(pkg.package, pkg.nest, pkg.description, icon);
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
			dir_exists.Add(ff.GetPath());
			String p = ff.GetPath();
			bool nw = nd.Find(p) < 0; // Do we have any info loaded about this package?
			PkData& d = nd.GetAdd(ff.GetPath());
			d.package = prefix + ff.GetName();
			d.nest = nest;
			if(nw) { // No cached info available about the folder
				d.ispackage = IsLetter(*d.package) && d.package.Find('.') < 0; // First heuristic guess
				d.main = d.ispackage && prefix.GetCount() == 0; // Expect it is main
			}
		}
}

String SelectPackageDlg::CachePath(const char *vn) const
{
	return AppendFileName(ConfigFile("cfg"), String(vn) + ".pkg_cache");
}

void SelectPackageDlg::Load()
{
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
		}
		Vector<String> upp = GetUppDirs();
		packages.Clear();
		description.Hide();
		progress.Show();
		loading = true;
		data.Clear();
		Index<String> dir_exists;
		String cache_path = CachePath(GetVarsName());
		LoadFromFile(data, cache_path);
		data.SetCount(upp.GetCount());
		for(int i = 0; i < upp.GetCount(); i++) // Scan nest folders for subfolders (package candidates)
			ScanFolder(upp[i], data[i], GetFileName(upp[i]), dir_exists, Null);
		int update = msecs();
		for(int i = 0; i < data.GetCount() && loading; i++) { // Now investigate individual sub folders
			ArrayMap<String, PkData>& nest = data[i];
			for(int i = 0; i < nest.GetCount() && loading; i++) {
				if(msecs(update) >= 100) { // each 100 ms update the list (and open select dialog after splash screen is closed)
					if(!IsSplashOpen() && !IsOpen())
						Open();
					progress++;
					SyncList();
					update = msecs();
				}
				ProcessEvents(); // keep GUI running
	
				PkData& d = nest[i];
				String path = nest.GetKey(i);
				FindFile ff(path);
				if(ff && ff.IsFolder()) {
					String upp_path = AppendFileName(path, GetFileName(d.package) + ".upp");
					LSLOW();
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
						String icon_path = AppendFileName(path, "icon16x16.png");
						tm = FileGetTime(icon_path);
						if(IsNull(tm)) // package icon does not exist
							d.icon = Null;
						else
						if(tm != d.itm) { // chached package icon outdated
							d.icon = StreamRaster::LoadFileAny(icon_path);
							d.itm = tm;
						}
					}
				}
				else 
					nest.Unlink(i); // cached folder was deleted
				ScanFolder(path, nest, d.nest, dir_exists, d.package + '/');
			}
			nest.Sweep();
		}
	
		StoreToFile(data, cache_path);
		progress.Hide();
		while(IsSplashOpen())
			ProcessEvents();
		if(!IsOpen())
			Open();
		description.Show();
		if(loading) {
			loading = false;
			SyncList();
		}
	}
}

void SelectPackageDlg::SyncBase(String initvars)
{
	Vector<String> varlist;
	for(FindFile ff(ConfigFile("*.var")); ff; ff.Next())
		if(ff.IsFile())
			varlist.Add(GetFileTitle(ff.GetName()));
	Sort(varlist, &PackageLess);
	base.Clear();
	Append(base, varlist);
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

String SelectPackage(const char *title, const char *startwith, bool selectvars, bool main)
{
	SelectPackageDlg dlg(title, selectvars, main);
	const char *c = main ? "SelectPkgMain" : "SelectPkg";
	LoadFromGlobal(dlg, c);
	dlg.SyncBrief();
	String b = dlg.Run(startwith);
	StoreToGlobal(dlg, c);
	return b;
}
