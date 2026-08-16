#include "ide.h"

#ifdef PLATFORM_WIN32

void Finish(UrepoConsole& console, int errors)
{
	if(errors)
		console.Log(String() << "There were errors (" << errors << ")", SLtRed());
	else
		console.Log("OK", SGreen());
}

struct VcpkgDlg : WithVcpkgLayout<TopWindow> {
	Vector<VcpkgInstalled> items;

	void Sync();
	void SyncIde();
	void SyncList();
	void Install();
	void Perform();

	VcpkgDlg();
};

VcpkgDlg::VcpkgDlg()
{
	CtrlLayoutExit(*this, "vcpkg");
	
	list.AddColumn("Name");
	list.AddColumn("Triplets");
	list.AddColumn("Version");
	list.AddColumn("Description");
	list.ColumnWidths("140 396 79 281");
	
	required_list.AddColumn("Required");

	triplets_list.AddColumn("Triplets");

	missing_list.AddColumn("Missing");
	missing_list.AddColumn("Triplet");
	
	install_missing << [=] {
		UrepoConsole console;
		int errors = 0;
		for(int i = 0; i < missing_list.GetCount(); i++) {
			String name = missing_list.Get(i, 0);
			String triplet = missing_list.Get(i, 1);
			if(!VcpkgInstall([&](const String& cmd, const String& chdir) { return console.System(cmd, chdir); }, name, triplet)) {
				console.Log("Failed", SLtRed());
				errors++;
			}
		}
		
		Finish(console, errors);
		console.Perform();
		SyncList();
		SyncIde();
	};
	
	install << [=] { Install(); };
	
	remove << [=] {
		if(!list.IsCursor())
			return;
		String name = ~list.GetKey();
		if(PromptYesNo("Remove [* \1" + name + "\1]?")) {
			UrepoConsole console;
			int errors = 0;
			for(String triplet : Split(~list.Get(list.GetCursor(), 1), ' ')) {
				if(console.System(VcpkgExe() + " remove " + name + ":" + triplet)) {
					console.Log("Failed", SLtRed());
					errors++;
				}
			}
			Finish(console, errors);
			console.Perform();
			SyncList();
			SyncIde();
		}
	};
	
	update << [=] { // TODO!
		UrepoConsole console;
		console.System("git pull --no-rebase", GetFileFolder(VcpkgExe()));
		console.System(VcpkgExe() + " update");
		console.Perform();
	};
	
	list.WhenSel << [this] { Sync(); };
}

void VcpkgDlg::Sync()
{
	remove.Enable(list.IsCursor());
}

void VcpkgDlg::SyncIde()
{
	Vector<String> triplets = VcpkgTriplets();

	triplets_list.Clear();
	for(String s : triplets)
		triplets_list.Add(s);

	Vector<String> required = RequiredExternalDependencies("VCPKG");
	
	required_list.Clear();
	for(String s : required)
		required_list.Add(s);
	
	missing_list.Clear();
	for(String r : required)
		for(String t : triplets)
			if(!VcpkgHasInstalled(items, r, t))
				missing_list.Add(r, t);
	install_missing.Enable(missing_list.GetCount());
	Sync();
}

void VcpkgDlg::SyncList()
{
	items = VcpkgList();
	
	list.Clear();
	for(const VcpkgInstalled& m : items)
		list.Add(m.name, Join(m.triplets.GetKeys(), " "), m.version, m.desc);
	Sync();
}

void VcpkgDlg::Perform()
{
	if(!IsVcpkgInstalled()) {
		if(!PromptYesNo("Install vcpkg?"))
			return;
		UrepoConsole console;
		if(!InstallVcpkg([&](const String& cmd, const String& chdir) { return console.System(cmd, chdir); })) {
			Exclamation("Installation failed");
			return;
		}
		console.Log("Installation completed.", SGreen());
		console.Perform();
	}
	SyncList();
	SyncIde();
	Execute();
}

struct VcpkgInstallDlg : WithVcpkgInstallLayout<TopWindow> {
	bool Key(dword key, int count) override;

	VcpkgInstallDlg();
	
	struct Item : Moveable<Item> {
		String name;
		String version;
		String desc;
	};
	
	Vector<Item>  items;
	Index<String> ts;
	
	void Perform();
	void Sync();
};

void VcpkgDlg::Install()
{
	VcpkgInstallDlg dlg;
	for(const String& s : VcpkgTriplets())
		dlg.ts.FindAdd(s);
	for(const VcpkgInstalled& m : items) {
		for(const String& s : m.triplets)
			dlg.ts.FindAdd(s);
	}
	SortIndex(dlg.ts);
	dlg.Perform();
	SyncIde();
}

VcpkgInstallDlg::VcpkgInstallDlg()
{
	CtrlLayoutOKCancel(*this, "Install");
	list.AddColumn("Name");
	list.AddColumn("Version");
	list.AddColumn("Description");
	list.ColumnWidths("176 154 551");
	
	triplets.AddColumn("Triplets");
	triplets.NoCursor().NoGrid();
	
	search.NullText("Search (Ctrl+K)");
	search.SetFilter([](int c) { return (int)ToUpper(ToAscii(c)); });
	search << [this] { Sync(); };
}

bool VcpkgInstallDlg::Key(dword key, int count)
{
	if(key == K_CTRL_K) {
		search.SetFocus();
		return true;
	}
	return TopWindow::Key(key, count);
}

void VcpkgInstallDlg::Sync()
{
	String s = ~search;
	Value key = list.GetKey();
	int sc = list.GetScroll();
	list.Clear();
	for(const Item& m : items)
		if(ToUpper(m.name + m.version + m.desc).Find(s) >= 0)
			list.Add(m.name, m.version, m.desc);
	list.ScrollTo(sc);
	if(!IsNull(key))
		list.FindSetCursor(key);
}

void VcpkgInstallDlg::Perform()
{
	for(Value l : ParseJSON(Sys(VcpkgExe() + " search --x-json"))) {
		Item& m = items.Add();
		
		m.name = ~l["package_name"];
		m.version = ~l["version"];
		String p = ~l["port_version"];
		if(p != "0")
			m.version << '#' << p;
		if(l["description"].GetCount())
			m.desc = l["description"][0];
	}
	
	for(String s : Split(Sys(VcpkgExe() + " help triplets"), '\n')) {
		s.TrimEnd("\r");
		if(s.TrimStart("  "))
			ts.FindAdd(s);
	}
	
	for(String s : ts) {
		int ii = triplets.GetCount();
		triplets.Add();
		auto& o = triplets.CreateCtrl<Option>(ii, 0);
		o.SetLabel(s);
	}
	
	Sync();
	
	if(ExecuteOK()) {
		String name = ~list.GetKey();
		if(name.GetCount() == 0)
			return;

		int errors = 0;
		UrepoConsole console;
		for(int i = 0; i < triplets.GetCount(); i++) {
			if((bool)~*triplets.GetCtrl(i, 0)) {
				if(console.System(VcpkgExe() + " install " + name + ":" + ts[i])) {
					console.Log("Failed", SLtRed());
					errors++;
				}
			}
		}
		
		Finish(console, errors);
		console.Perform();
	}
}

void Ide::Vcpkg()
{
	VcpkgDlg dlg;
	dlg.Perform();
}

#endif
