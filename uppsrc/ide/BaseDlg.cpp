#include "ide.h"

static VectorMap<String, PackageInfo> sPi;

void InvalidatePackageInfo(const String& name)
{
	int q = sPi.Find(name);
	if(q >= 0)
		sPi[q].path.Clear();
}

PackageInfo GetPackageInfo(const String& name)
{
	String path = PackagePath(name);
	Time tm = FileGetTime(path);
	int q = sPi.Find(name);
	if(q >= 0) {
		if(path == sPi[q].path && tm == sPi[q].stamp)
			return sPi[q];
	}
	else {
		q = sPi.GetCount();
		sPi.Add(name);
	}
	PackageInfo& pi = sPi[q];
	pi.path = path;
	pi.stamp = tm;
	Package p;
	p.Load(path);
	pi.ink = p.ink;
	pi.italic = p.italic;
	pi.bold = p.bold;
	return pi;
}

void AddAssemblyPaths(FileSel& dir)
{
	dir.ClearPlaces();
	dir.AddStandardPlaces();
	VectorMap<String, int> paths;
	for(FindFile ff(ConfigFile("*.var")); ff; ff.Next()) {
		if(ff.IsFile()) {
			Nest n;
			if(n.Load(ff.GetPath())) {
				for(String p : Split(n.Get("UPP"), ';')) {
					FindFile ff(p);
					if(ff) {
						String h = NormalizePath(GetFileFolder(p));
					#ifdef PLATFORM_WIN32
						h = ToLower(h);
					#endif
						paths.GetAdd(AppendFileName(h, ff.GetName()), 0)++;
					}
				}
			}
		}
	}
	SortByKey(paths);
	StableSortByValue(paths, std::greater<int>());
	Vector<String> p = paths.PickKeys();
	Index<String> was;
	if(p.GetCount()) {
		dir.AddPlaceSeparator();
		for(String h : p) {
			String name = GetFileName(h);
			if(was.Find(name) >= 0)
				name << " (" << GetFileName(GetFileFolder(h)) << ")";
			else
				was.Add(name);
			dir.AddPlace(h, IdeImg::Icon(), name);
		}
	}
}

void NestEditorDlg::Set(const String& s)
{
	Vector<String> l = Split(s, ';');
	nests.Clear();
	for(int i = 0; i < l.GetCount(); i++)
		nests.Add(l[i]);
}

String NestEditorDlg::Get() const
{
	String s;
	for(int i = 0; i < nests.GetCount(); i++)
		MergeWith(s, ";", ~nests.Get(i, 0));
	return s;
}

NestEditorDlg::NestEditorDlg()
{
	static FileSel dir;

	CtrlLayoutOKCancel(*this, "Nests");
	Sizeable().Zoomable();

	add.SetImage(IdeImg::add()) << [=] {
		if(dir.ExecuteSelectDir("Insert nest directory")) {
			int q = max(nests.GetCursor(), 0);
			nests.Insert(q);
			nests.Set(q, 0, ~dir);
			nests.SetCursor(q);
			Sync();
		}
	};

	nests.WhenLeftDouble = edit.SetImage(IdeImg::pencil()) ^= [=] {
		if(nests.IsCursor()) {
			String h = nests.Get(0);
			if(EditText(h, "Nest directory", "Directory"))
				nests.Set(0, h);
		}
	};

	remove.SetImage(IdeImg::remove()) << [=] {
		nests.DoRemove();
		Sync();
	};
	
	up.SetImage(IdeImg::arrow_up()) << [=] {
		nests.SwapUp();
	};

	down.SetImage(IdeImg::arrow_down()) << [=] {
		nests.SwapDown();
	};

	nests.AutoHideSb().Moving()
		 .NoGrid().EvenRowColor().SetLineCy(nests.GetLineCy() + DPI(2))
		 .NoHeader().AddColumn();
	nests.WhenSel = nests.WhenStartEdit = [=] { Sync(); };

	nests.WhenDrag = [=] {
		nests.DoDragAndDrop(InternalClip(nests, "nest-item"), nests.GetDragSample(), DND_MOVE);
	};
	nests.WhenDropInsert = [=](int line, PasteClip& d) {
		if(GetInternalPtr<ArrayCtrl>(d, "nest-item") == &nests && nests.IsCursor() && d.Accept()) {
			int q = nests.GetCursor();
			if(q == line)
				return;
			String h = nests.Get(0);
			nests.Remove(q);
			if(q < line)
				line--;
			if(line >= 0 && line <= nests.GetCount()) {
				nests.Insert(line);
				nests.Set(line, 0, h);
				nests.SetCursor(line);
			}
		}
	};

	AddAssemblyPaths(dir);

	Sync();
}

void NestEditorDlg::Sync()
{
	bool b = nests.GetCount();
	edit.Enable(b);
	remove.Enable(b);
	up.Enable(b);
	down.Enable(b);
}

bool BaseSetup(String& vars) { return BaseSetupDlg().Run(vars); }

BaseSetupDlg::BaseSetupDlg()
: browse_out("Output & intermediate files")
{
	CtrlLayoutOKCancel(*this, "Assembly setup");
	browse_upp.SetImage(CtrlImg::right_arrow()).Tip("Select path");
	browse_upp <<= THISBACK(OnBrowseUpp);
	upp.AddFrame(browse_upp);

	setup_nest.Tip("Open nest editor");
	setup_nest << [=] {
		NestEditorDlg ndlg;
		ndlg.Set(~upp);
		if(!ndlg.ExecuteOK())
			return;
		upp <<= ndlg.Get();
		OnUpp();
	};

	upp <<= THISBACK(OnUpp);
	browse_out.Attach(output);
}

bool BaseSetupDlg::Run(String& vars)
{
	upp     <<= GetVar("UPP");
	output  <<= GetVar("OUTPUT");
	base    <<= vars;
	new_base = IsNull(vars);
	
	while(TopWindow::Run() == IDOK)
	{
		String varname = ~base;
		String varfile = VarFilePath(varname);
		if(varname != vars)
		{
			if(FileExists(varfile) && !PromptOKCancel(NFormat("Overwrite existing assembly [* \1%s\1]?", varfile)))
				continue;
			if(!SaveVars(varname))
			{
				Exclamation(NFormat("Error writing assmbly [* \1%s\1].", VarFilePath(varname)));
				continue;
			}
		}
		SetVar("UPP", ~upp);
		SetVar("OUTPUT", ~output);
		Vector<String> paths = SplitDirs(upp.GetText().ToString());
		for(int i = 0; i < paths.GetCount(); i++)
			RealizeDirectory(paths[i]);
		RealizeDirectory(~output);
		vars = varname;
		return true;
	}
	return false;
}

void BaseSetupDlg::OnBrowseUpp()
{
	String s = ~upp;
	int b, e;
	if(upp.HasFocus())
		upp.GetSelection(b, e);
	else
		e = s.GetLength();
	b = e;
	while(b > 0 && s[b - 1] != ';')
		b--;
	while(e < s.GetLength() && s[e] != ';')
		e++;
	SelectDirButton dirsel;
	String pre = s.Left(b), post = s.Mid(e);
	dirsel.ActiveDir(s.Mid(b, e - b));
	
	AddAssemblyPaths((FileSel&) dirsel);

	if(dirsel.ExecuteSelectDir()) {
		String newdir = ~dirsel;
		upp <<= pre + newdir + Nvl(post, ";");
		upp.SetWantFocus();
		OnUpp();
	}
}

void BaseSetupDlg::OnUpp()
{
	if(new_base) {
		String s = ~upp;
		int f = s.Find(';');
		if(f >= 0) s.Trim(f);
		base <<= GetFileTitle(s);
	}
}
