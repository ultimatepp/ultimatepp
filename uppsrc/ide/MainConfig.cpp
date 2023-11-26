#include "ide.h"

struct MainConfigDlg : public WithConfigLayout<TopWindow> {
	EditString ce, fe;
	FrameRight<Button> cb;
	const Workspace& wspc;
	
	void FlagDlg();

	bool Perform(const String& startwith);

	typedef MainConfigDlg CLASSNAME;

	MainConfigDlg(const Workspace& wspc);
};

bool SetSw(const String& flag, Ctrl& sw, const char *flg) {
	if(flag == flg) {
		sw <<= 1;
		return true;
	}
	return false;
}

String GetSw(Ctrl& sw, const char *flag) {
	if((int)~sw)
		return String(flag) + ' ';
	return Null;
}

void MainConfigDlg::FlagDlg()
{
	VectorMap<String, Index<String>> flags;

	PPInfo pp;
	pp.SetIncludes(TheIde()->GetCurrentIncludePath() + ";" + GetClangInternalIncludes());
	const Workspace& wspc = GetIdeWorkspace();

	for(int i = 0; i < wspc.GetCount(); i++) { // find package of included file
		const Package& pk = wspc.GetPackage(i);
		String pk_name = wspc[i];
		for(int i = 0; i < pk.file.GetCount(); i++)
			for(const Tuple<String, int>& m : pp.GetFileFlags(SourcePath(pk_name, pk.file[i]))) {
				String f = m.a;
				f.TrimStart("flag");
				flags.GetAdd(f).FindAdd(pk_name);
			}
	}
	
	SortByKey(flags);
	
	WithConfLayout<TopWindow> cfg;
	CtrlLayoutOKCancel(cfg, "Configuration flags");
	cfg.Sizeable().MaximizeBox();
	Vector<String> flg = SplitFlags0(~~fe);
	enum { CC_SET, CC_NAME, CC_PACKAGES, CC_COUNT };
	cfg.accepts.AddColumn("Set").With([](One<Ctrl>& ctrl) {
		ctrl.Create<Option>().NoWantFocus();
	});
	cfg.accepts.AddColumn("Flag");
	cfg.accepts.AddColumn("Packages");
	cfg.accepts.SetLineCy(Zy(20));
	cfg.accepts.ColumnWidths("26 122 204");
	for(const auto& f : ~flags)
		cfg.accepts.Add(false, f.key, Join(f.value.GetKeys(), ", "));
	
	cfg.other.SetFilter(FlagFilterM);
	cfg.gui <<= false;
	cfg.debugcode <<= false;
	String other;
	for(int i = 0; i < flg.GetCount(); i++) {
		String f = flg[i];
		if(!SetSw(f, cfg.gui, "GUI") &&
		   !SetSw(f, cfg.debugcode, "DEBUGCODE")) {
			int x = *f == '.' ? cfg.accepts.Find(f.Mid(1), CC_NAME) : cfg.accepts.Find(f, CC_NAME);
			if(x >= 0)
				cfg.accepts.Set(x, CC_SET, true);
			else {
				if(!other.IsEmpty())
					other << ' ';
				other << f;
			}
		}
	}
	cfg.other <<= other;
	if(cfg.Run() == IDOK) {
		String flags;
		flags
		    << GetSw(cfg.gui, "GUI")
		    << GetSw(cfg.debugcode, "DEBUGCODE");
		for(int i = 0; i < cfg.accepts.GetCount(); i++)
			if(cfg.accepts.Get(i, CC_SET))
				flags << cfg.accepts.Get(i, CC_NAME) << ' ';
		flags << cfg.other.GetText().ToString();
		fe = Join(SplitFlags0(flags), " ").ToWString();
	}
}

MainConfigDlg::MainConfigDlg(const Workspace& wspc_) : wspc(wspc_) {
	CtrlLayoutOKCancel(*this, "Main package configuration(s)");
	fe.AddFrame(cb);
	fe.SetFilter(FlagFilterM);
	cb.SetImage(CtrlImg::smallright()).NoWantFocus();
	cb <<= THISBACK(FlagDlg);
	list.AddColumn("Flags", 3).Edit(fe);
	list.AddColumn("Optional name", 2).Edit(ce);
	list.Appending().Removing().Moving().Duplicating();
	
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

}

bool MainConfigDlg::Perform(const String& startwith) {
	list.SetCursor(0);
	list.FindSetCursor(startwith);
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
