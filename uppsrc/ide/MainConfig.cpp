#include "ide.h"

struct MainConfigDlg : public WithConfigLayout<TopWindow> {
	const Workspace& wspc;
	
	bool Perform(const String& startwith);
	
	void Sync();

	typedef MainConfigDlg CLASSNAME;

	MainConfigDlg(const Workspace& wspc);
};

struct FlagsDlg : WithConfLayout<TopWindow> {
	Index<String> recognized_flags;

	enum { CC_SET, CC_FLAG, CC_PACKAGES, CC_COUNT };
	
	void Options();
	void Flags();

	void Get(ArrayCtrl& a) { flags <<= a.Get(0); name <<= a.Get(1); Flags(); }
	void Set(ArrayCtrl& a) { a.Set(0, ~flags); a.Set(1, ~name); Flags(); }

	FlagsDlg();
};

void FlagsDlg::Options()
{
	Vector<String> flg = SplitFlags0(~~flags);

	String f;

	if(gui)
		f << "GUI ";
	if(debugcode)
		f << "DEBUGCODE ";
	for(int i = 0; i < accepts.GetCount(); i++)
		if(accepts.Get(i, CC_SET))
			f << accepts.Get(i, CC_FLAG) << ' ';
	
	for(String s : flg)
		if(recognized_flags.Find(s) < 0)
			f << s << ' ';
	
	f.TrimEnd(" ");
	
	flags <<= f;
	flags.SetSelection(flags.GetLength());
}

void FlagsDlg::Flags()
{
	Index<String> flg(pick(SplitFlags0(~~flags)));
	gui <<= flg.Find("GUI") >= 0;
	debugcode <<= flg.Find("DEBUGCODE") >= 0;
	
	for(int i = 0; i < accepts.GetCount(); i++)
		accepts.Set(i, CC_SET, flg.Find(~~accepts.Get(i, CC_FLAG)) >= 0);
}

FlagsDlg::FlagsDlg()
{
	VectorMap<String, Tuple<String, Index<String>>> code_flags;

	CtrlLayoutOKCancel(*this, "Configuration flags");

	PPInfo pp;
	pp.SetIncludes(TheIde()->GetCurrentIncludePath() + ";" + GetClangInternalIncludes());
	const Workspace& wspc = GetIdeWorkspace();
	
	static Index<String> ignore_flags = {
		"DEBUG", "WIN32", "GUI", "DEBUGCODE", "GCC", "GCC32", "CLANG",
		"ANDROID", "WIN32", "POSIX", "OSX", "SO",
	};

	for(int i = 0; i < wspc.GetCount(); i++) { // find package of included file
		const Package& pk = wspc.GetPackage(i);
		String pk_name = wspc[i];
		for(int i = 0; i < pk.file.GetCount(); i++)
			if(!pk.file[i].separator)
				for(auto m : ~pp.GetFileFlags(SourcePath(pk_name, pk.file[i]))) {
					String f = m.key;
					f.TrimStart("flag");
					if(ignore_flags.Find(f) < 0) {
						auto& fl = code_flags.GetAdd(f);
						fl.b.FindAdd(pk_name);
						String comment = m.value;
						if(comment.GetCount() > fl.a.GetCount())
							fl.a = comment;
					}
				}
	}

	SortByKey(code_flags);

	Sizeable().MaximizeBox();

	enum { CC_SET, CC_NAME, CC_PACKAGES, CC_COUNT };
	accepts.AddColumn("Set").With([=](One<Ctrl>& ctrl) {
		ctrl.Create<Option>().NoWantFocus() ^= [=] { Options(); };
	});
	accepts.AddColumn("Flag");
	accepts.AddColumn("Comment");
	accepts.AddColumn("Packages");
	accepts.SetLineCy(Zy(20));
	accepts.ColumnWidths("29 140 458 117");
	accepts.EvenRowColor();
	accepts.NoCursor();
	for(int pass = 0; pass < 2; pass++) // second pass for "hidden" flags
		for(const auto& f : ~code_flags)
			if(IsNull(f.value.a) == pass) {
				accepts.Add(false, f.key, AttrText(f.value.a).Italic(pass), Join(f.value.b.GetKeys(), ", "));
				recognized_flags.FindAdd(f.key);
			}
	
	flags.SetFilter(FlagFilterM);
	flags << [=] { Flags(); };
	gui <<= false;
	debugcode <<= false;
	gui << [=] { Options(); };
	debugcode << [=] { Flags(); };
	recognized_flags.FindAdd("GUI");
	recognized_flags.FindAdd("DEBUGCODE");
}

void MainConfigDlg::Sync()
{
	bool b = list.IsCursor();
	insert.Enable(b);
	remove.Enable(b);
	duplicate.Enable(b);
	edit.Enable(b);
	up.Enable(b);
	down.Enable(b);
}

MainConfigDlg::MainConfigDlg(const Workspace& wspc_) : wspc(wspc_) {
	CtrlLayoutOKCancel(*this, "Main package configuration(s)");
	Sizeable().Zoomable();
	list.AddColumn("Flags", 3);
	list.AddColumn("Optional name", 2);
	list.WhenSel = [=] {
		Sync();
	};
	list.WhenDrag = [=] {
		list.DoDragAndDrop(InternalClip(list, "main_config-item"), list.GetDragSample(), DND_MOVE);
	};
	list.WhenDropInsert = [=](int line, PasteClip& d) {
		if(GetInternalPtr<ArrayCtrl>(d, "main_config-item") == &list && list.IsCursor() && d.Accept()) {
			int q = list.GetCursor();
			if(q == line)
				return;
			Vector<Value> h = list.ReadRow(q);
			list.Remove(q);
			if(q < line)
				line--;
			if(line >= 0 && line <= list.GetCount()) {
				list.Insert(line);
				list.Set(line, h);
				list.SetCursor(line);
			}
		}
	};

	add.SetImage(IdeImg::add()) << [=] {
		FlagsDlg cfg;
		if(cfg.Run() == IDOK) {
			list.Add(~cfg.flags, ~cfg.name);
			list.GoEnd();
		}
	};

	insert.SetImage(IdeImg::insert()) << [=] {
		if(list.IsCursor()) {
			FlagsDlg cfg;
			if(cfg.Run() != IDOK)
				return;
			int q = list.GetCursor();
			list.Insert(q);
			list.SetCursor(q);
			cfg.Set(list);
		}
	};

	duplicate.SetImage(IdeImg::duplicate()) << [=] {
		if(list.IsCursor()) {
			FlagsDlg cfg;
			cfg.Get(list);
			if(cfg.Run() != IDOK)
				return;
			int q = list.GetCursor() + 1;
			list.Insert(q);
			list.SetCursor(q);
			cfg.Set(list);
		}
	};

	list.WhenLeftDouble = edit.SetImage(IdeImg::pencil()) ^= [=] {
		if(list.IsCursor()) {
			FlagsDlg cfg;
			cfg.Get(list);
			if(cfg.Run() != IDOK)
				return;
			cfg.Set(list);
		}
	};

	remove.SetImage(IdeImg::remove()) << [=] {
		int q = list.GetCursor();
		if(q >= 0) {
			list.Remove(q);
			if(q >= list.GetCount())
				q--;
			if(q >= 0)
				list.SetCursor(q);
		}
	};
	
	up.SetImage(IdeImg::arrow_up()) << [=] {
		list.SwapUp();
	};

	down.SetImage(IdeImg::arrow_down()) << [=] {
		list.SwapDown();
	};
}

bool MainConfigDlg::Perform(const String& startwith) {
	list.SetCursor(0);
	list.FindSetCursor(startwith);
	Sync();
	return Run() == IDOK;
}

void Ide::MainConfig() {
	package.SetCursor(0);
	if(package.GetCursor() != 0) return;
	MainConfigDlg dlg(IdeWorkspace());
	for(int i = 0; i < actual.config.GetCount(); i++) {
		const Package::Config& f = actual.config[i];
		dlg.list.Add(f.param, f.name);
	}
	if(!dlg.Perform(mainconfigname)) return;
	actual.config.Clear();
	for(int i = 0; i < dlg.list.GetCount(); i++) {
		Package::Config& f = actual.config.Add();
		f.param = dlg.list.Get(i, 0);
		f.name = dlg.list.Get(i, 1);
	}
	SavePackage();
	if(dlg.list.IsCursor()) {
		mainconfigparam = dlg.list.Get(0);
		mainconfigname = Nvl((String)dlg.list.Get(1), mainconfigparam);
		MakeTitle();
	}
	SyncMainConfigList();
	SetHdependDirs();
	MakeTitle();
	TriggerIndexer();
	editor.TriggerSyncFile(0);
}

void Ide::SyncMainConfigList()
{
	mainconfiglist.Clear();
	const Workspace& wspc = IdeWorkspace();
	if(wspc.GetCount() <= 0) return;
	const Array<Package::Config>& f = wspc.GetPackage(0).config;
	for(int i = 0; i < f.GetCount(); i++)
		mainconfiglist.Add(f[i].param, Nvl(f[i].name, f[i].param));
	SetMainConfigList();
}

void Ide::SetMainConfigList()
{
	mainconfiglist <<= mainconfigparam;
	mainconfigname = mainconfiglist.GetValue();
	mainconfiglist.Tip("Main configuration: " + mainconfigparam);
}

void Ide::OnMainConfigList()
{
	mainconfigparam = ~mainconfiglist;
	SetMainConfigList();
	MakeTitle();
	TriggerIndexer();
	editor.TriggerSyncFile(0);
	editor.SetFocus();
}
