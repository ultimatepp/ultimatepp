#include "ide.h"

struct FlagsDlg : WithConfLayout<TopWindow> {
	VectorMap<String, Tuple<String, Index<String>>> code_flags;
	Index<String> recognized_flags;
	int           standard_flags;

	enum { CC_SET, CC_FLAG, CC_PACKAGES, CC_COUNT };
	
	void Options();
	void Flags();
	void Reload();

	void Get(const Package::Config& cfg) { flags <<= cfg.param; name <<= cfg.name; Flags(); }
	void Set(Package::Config& cfg)       { cfg.param = ~flags; cfg.name = ~name; }

	FlagsDlg();
	~FlagsDlg();
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

FlagsDlg::~FlagsDlg()
{
	StoreToGlobal([&](Stream& s) { SerializePlacement(s); }, "FlagsDlgPlacement");
}

FlagsDlg::FlagsDlg()
{
	CtrlLayoutOKCancel(*this, "Configuration flags");
	
	Sizeable().Zoomable();

	LoadFromGlobal([&](Stream& s) { SerializePlacement(s); }, "FlagsDlgPlacement");

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

	flags.SetFilter(FlagFilterM);
	flags << [=] { Flags(); };
	gui <<= false;
	debugcode <<= false;
	gui << [=] { Options(); };
	debugcode << [=] { Options(); };
	recognized_flags.FindAdd("GUI");
	recognized_flags.FindAdd("DEBUGCODE");
	standard_flags = recognized_flags.GetCount();

	search.NullText("Search");
	search.SetFilter([](int c) { return ToUpper(c); });
	search << [=] { Reload(); };

	Reload();
}

void FlagsDlg::Reload()
{
	String s = ~search;
	int c = accepts.GetCursor();
	int sc = accepts.GetScroll();
	accepts.Clear();
	recognized_flags.Trim(standard_flags);
	for(int pass = 0; pass < 2; pass++) // second pass for "hidden" flags
		for(const auto& f : ~code_flags) {
			String packages = Join(f.value.b.GetKeys(), ", ");
			if(IsNull(f.value.a) == !!pass && ToUpper(f.key + f.value.a + packages).Find(s) >= 0) {
				accepts.Add(false, f.key, AttrText(f.value.a).Italic(pass), packages);
				recognized_flags.FindAdd(f.key);
			}
		}
	accepts.ScrollTo(sc);
	accepts.SetCursor(c);
}

struct MainConfigDlg : public WithConfigLayout<TopWindow> {
	const Workspace& wspc;
	
	Array<Package::Config> config;
	
	bool Perform(const String& startwith);
	
	bool CanMove() const                     { return list.GetCount() == config.GetCount(); }
	void Sync();
	void LoadList(int setkey = -1, bool fix_search = true);

	typedef MainConfigDlg CLASSNAME;

	MainConfigDlg(const Workspace& wspc);
};

void MainConfigDlg::Sync()
{
	bool b = list.IsCursor();
	insert.Enable(b);
	remove.Enable(b);
	duplicate.Enable(b);
	edit.Enable(b);
	b = b && CanMove();
	up.Enable(b);
	down.Enable(b);
}

void MainConfigDlg::LoadList(int setkey, bool fix_search)
{
	for(int pass = 0; pass < 2; pass++) {
		String s = ~search;
		int sc = list.GetScroll();
		list.Clear();
		for(int i = 0; i < config.GetCount(); i++)
			if(ToUpper(config[i].param).Find(s) >= 0 || ToUpper(config[i].name).Find(s) >= 0)
				list.Add(i, config[i].param, config[i].name);
		list.ScrollTo(sc);
		if(setkey < 0 || list.FindSetCursor(setkey))
			break;
		if(!fix_search) {
			list.GoBegin();
			break;
		}
		search <<= Null;
	}
	Sync();
}

MainConfigDlg::MainConfigDlg(const Workspace& wspc_) : wspc(wspc_) {
	CtrlLayoutOKCancel(*this, "Main package configuration(s)");
	Sizeable().Zoomable();

	search.NullText("Search");
	search.SetFilter(CharFilterToUpper);
	search << [=] {
		LoadList(list.GetKey(), false);
	};

	list.AddKey(); // index in config
	list.AddColumn("Flags", 3);
	list.AddColumn("Optional name", 2);
	list.WhenSel = [=] {
		Sync();
	};
	list.WhenDrag = [=] {
		if(CanMove())
			list.DoDragAndDrop(InternalClip(list, "main_config-item"), list.GetDragSample(), DND_MOVE);
	};

	list.WhenDropInsert = [=](int q, PasteClip& d) {
		if(GetInternalPtr<ArrayCtrl>(d, "main_config-item") == &list && list.IsCursor() && d.Accept()) {
			if(q >= 0 && q <= list.GetCount() && CanMove()) {
				int from = list.GetKey();
				if(from >= 0 && from < config.GetCount() && q != from) {
					Package::Config h = config[from];
					config.Remove(from);
					if(from < q)
						q--;
					config.Insert(q) = h;
					LoadList(q);
				}
			}
		}
	};

	append.SetImage(IdeImg::add()) << [=] {
		FlagsDlg cfg;
		if(cfg.Run() == IDOK) {
			cfg.Set(config.Add());
			LoadList(config.GetCount() - 1);
		}
	};

	insert.SetImage(IdeImg::insert()) << [=] {
		int q = list.GetKey();
		if(q >= 0 && q < config.GetCount()) {
			FlagsDlg cfg;
			if(cfg.Run() != IDOK)
				return;
			cfg.Set(config.Insert(q));
			LoadList(q);
		}
	};

	duplicate.SetImage(IdeImg::duplicate()) << [=] {
		int q = list.GetKey();
		if(q >= 0 && q < config.GetCount()) {
			FlagsDlg cfg;
			cfg.Get(config[q]);
			if(cfg.Run() != IDOK)
				return;
			q++;
			cfg.Set(config.Insert(q));
			LoadList(q);
		}
	};

	list.WhenLeftDouble = edit.SetImage(IdeImg::pencil()) ^= [=] {
		int q = list.GetKey();
		if(q >= 0 && q < config.GetCount()) {
			FlagsDlg cfg;
			cfg.Get(config[q]);
			if(cfg.Run() != IDOK)
				return;
			cfg.Set(config[q]);
			LoadList(q);
		}
	};

	remove.SetImage(IdeImg::remove()) << [=] {
		int q = list.GetKey();
		if(q >= 0 && q < config.GetCount() && PromptYesNo("Remove configuration?")) {
			config.Remove(q);
			if(q >= list.GetCount())
				q--;
			LoadList(q);
		}
	};
	
	up.SetImage(IdeImg::arrow_up()) << [=] {
		int q = list.GetKey();
		if(q > 0 && q < config.GetCount())
			config.Swap(q - 1, q);
		LoadList(q - 1);
	};

	down.SetImage(IdeImg::arrow_down()) << [=] {
		int q = list.GetKey();
		if(q >= 0 && q + 1 < config.GetCount())
			config.Swap(q + 1, q);
		LoadList(q + 1);
	};

	list.WhenBar = [=](Bar& bar) {
		bar.Add("Append", IdeImg::add(), [=] { append.WhenAction(); });
		bar.Add("Insert", IdeImg::insert(), [=] { insert.WhenAction(); });
		bar.Add("Duplicate", IdeImg::duplicate(), [=] { duplicate.WhenAction(); });
		bar.Add("Edit", IdeImg::pencil(), [=] { edit.WhenAction(); });
		bar.Add("Remove", IdeImg::remove(), [=] { remove.WhenAction(); });
		bool canmove = list.GetCount() == config.GetCount();
		bar.Add(canmove, "Move up", IdeImg::arrow_up(), [=] { up.WhenAction(); });
		bar.Add(canmove, "Move down", IdeImg::arrow_down(), [=] { down.WhenAction(); });
	};
}

bool MainConfigDlg::Perform(const String& startwith) {
	list.SetCursor(0);
	list.FindSetCursor(startwith, 1);
	Sync();
	return Run() == IDOK;
}

void Ide::MainConfig() {
	package.SetCursor(0);
	if(package.GetCursor() != 0) return;
	MainConfigDlg dlg(IdeWorkspace());
	dlg.config = clone(actual.config);
	dlg.LoadList();
	if(!dlg.Perform(mainconfigparam)) return;
	actual.config.Clear();
	actual.config = clone(dlg.config);
	SavePackage();
	if(dlg.list.IsCursor()) {
		mainconfigparam = dlg.list.Get(1);
		mainconfigname = Nvl((String)dlg.list.Get(2), mainconfigparam);
		MakeTitle();
	}
	SyncMainConfigList();
	SetHdependDirs();
	MakeTitle();
	TriggerIndexer();
	editor.TriggerSyncFile(0);
	SyncExternalDependencies();
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
	SyncExternalDependencies();
}
