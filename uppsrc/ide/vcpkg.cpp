#include "ide.h"

#ifdef PLATFORM_WIN32

struct VcpkgDlg : WithVcpkgLayout<TopWindow> {
	Vector<VcpkgInstalled> items;

	void SyncIde();
	void SyncList();
	void Perform();

	VcpkgDlg();
};

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
	
	for(String r : required)
		for(String t : triplets)
			if(!VcpkgHasInstalled(items, r, t))
				missing_list.Add(r, t);
	install_missing.Enable(missing_list.GetCount());
}

void VcpkgDlg::SyncList()
{
	items = VcpkgList();
	
	list.Clear();
	for(const VcpkgInstalled& m : items)
		list.Add(m.name, Join(m.triplets.GetKeys(), " "), m.version, m.desc);
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
		for(int i = 0; i < missing_list.GetCount(); i++) {
			String name = missing_list.Get(i, 0);
			String triplet = missing_list.Get(i, 1);
			console.System(VcpkgExe() + " install " + name + ":" + triplet);
		}
		console.Log("Installation completed.", SGreen());
		console.Perform();
		SyncList();
		SyncIde();
	};
}

void Ide::Vcpkg()
{
	VcpkgDlg dlg;
	dlg.Perform();
}

#endif
