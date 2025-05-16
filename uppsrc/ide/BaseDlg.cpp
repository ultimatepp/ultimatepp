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
	String path = PackageFile(name);
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

	CtrlLayoutOKCancel(*this, "Nests editor");
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

struct BoldDisplayClass : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
					   Color ink, Color paper, dword style) const {
		w.DrawRect(r, paper);
		DrawSmartText(w, r.left, r.top, r.Width(), (String)q, StdFont().Bold(), ink);
	}
};

Display& BoldDisplay()
{
	return Single<BoldDisplayClass>();
}

void NestEditorDlg::Sync()
{
	bool b = nests.GetCount();
	edit.Enable(b);
	remove.Enable(b);
	up.Enable(b);
	down.Enable(b);
	if(firstbold)
		for(int i = 0; i < nests.GetCount(); i++)
			nests.SetDisplay(i, 0, i == 0 ? BoldDisplay() : StdDisplay());
}

bool BaseSetup(String& vars) { return BaseSetupDlg().Run(vars); }

BaseSetupDlg::BaseSetupDlg()
{
	CtrlLayoutOKCancel(*this, "Assembly setup");

	setup_nest.Tip("Open nest editor..");
	setup_nest << [=] {
		NestEditorDlg ndlg;
		ndlg.Set(~upp);
		if(!ndlg.ExecuteOK())
			return;
		upp <<= ndlg.Get();
		OnUpp();
	};

	upp << [=] { OnUpp(); };
	
	setup_includes << [=] {
		NestEditorDlg ndlg;
		ndlg.firstbold = false;
		ndlg.Title("Includes");
		ndlg.Set(~include);
		if(!ndlg.ExecuteOK())
			return;
		include <<= ndlg.Get();
	};
	
	output_sel.Tip("Select output directory...");
	upv_sel.Tip("Select UppHub directory...");
	DirSelect(output, output_sel);
	DirSelect(upv, upv_sel);
	upv.NullText(GetHubDir());
	output.NullText(GetDefaultUppOut());
}

bool BaseSetupDlg::Run(String& vars)
{
	upp      <<= GetVar("UPP");
	output   <<= GetUppOut();
	include  <<= GetVar("INCLUDE");
	upv      <<= GetVar("UPPHUB");
	all      <<= GetVar("_all") == "1";
	base     <<= vars;
	new_base = IsNull(vars);

	Sync();
	
	while(TopWindow::Run() == IDOK)
	{
		String varname = ~base;
		String varfile = VarFilePath(varname);
		if(varname != vars)
		{
			if(FileExists(varfile) && !PromptOKCancel(Format("Overwrite existing assembly [* \1%s\1]?", varfile)))
				continue;
			if(!SaveVars(varname))
			{
				Exclamation(Format("Error writing assmbly [* \1%s\1].", VarFilePath(varname)));
				continue;
			}
		}
		SetVar("UPP", ~upp);
		SetVar("OUTPUT", ~output);
		SetVar("INCLUDE", ~include);
		SetVar("UPPHUB", ~upv);
		SetVar("_all", all ? "1" : "0");
		Vector<String> paths = SplitDirs(upp.GetText().ToString());
		for(int i = 0; i < paths.GetCount(); i++)
			RealizeDirectory(paths[i]);
		RealizeDirectory(~output);
		vars = varname;
		return true;
	}
	return false;
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
