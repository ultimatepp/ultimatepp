#include "ide.h"

struct SBOMDlg : WithSBOMLayout<TopWindow> {
	Array<SBOMComponent> cs;
	
	SBOMDlg();

	void Sync();
	void Perform();
};

SBOMDlg::SBOMDlg()
{
	CtrlLayoutExit(*this, "Software Bill Of Materials");
	
	list.AddColumn("Name", 200);
	list.AddColumn("Type", 80);
	list.AddColumn("Version", 120);
	list.AddColumn("External", 50);
	list.AddColumn("Scanned", 50);
	list.AddColumn("License", 300);
	list.AddColumn("Depends", 300);
	list.AddColumn("PURL", 300);
	list.AddColumn("Homepage", 300);
	list.AddColumn("Origin", 300);
	list.AddColumn("Source distributions", 300);
	list.HeaderObject().Absolute();
	list.NoCursor();
}

void SBOMDlg::Sync()
{
	int mod = ~mode;
	String s = ~search;
	int sc = list.GetScroll();
	list.Clear();
	for(SBOMComponent& m : cs) {
		String lic = Join(m.licenses, ", ");
		String dep = Join(m.depends, ", ");
		if(ToUpper(m.name + m.version + lic + dep + m.homepage + m.originUrl).Find(s) >= 0 &&
		   get_i(mod, !m.external, true, true, m.external <= 1, m.external <= 1))
			list.Add(m.name,
			         m.type,
			         m.version,
			         m.external ? "Y" : "",
			         get_i(mod, true, true, !m.external, true, !m.external) ? "Y" : "",
			         lic,
			         dep,
			         m.purl,
			         m.homepage,
			         m.originUrl,
			         Join(m.sourceDistributions, ", "));
	}
	list.ScrollTo(sc);
}


void SBOMDlg::Perform()
{
	mode.Add(SBOM_BASE, "0 Do not include external dependecies");
	mode.Add(SBOM_FULL, "1 Include everything (win32-vcpkg / docker / flatpak)");
	mode.Add(SBOM_EXTERNAL_FULL, "2 Include everything, exclude external dependencies from CVE scanning (linux binary)");
	mode.Add(SBOM_DIRECT, "3 Include only direct external dependencies");
	mode.Add(SBOM_EXTERNAL_DIRECT, "4 Include only direct external dependencies, exclude external dependencies from CVE scanning");  // 4

	mode <<= 1;
#ifdef PLATFORM_LINUX
	mode <<= 2;
#endif

	mode << [this] { Sync(); };
	
	search.NullText("Search");
	search.SetFilter([](int c) { return ToUpper(c); });
	search << [this] { Sync(); };
	
	Sync();
	
	Execute();
}

void Ide::CreateSBOM()
{
	SyncExternalDependencies(true);
	SBOMDlg dlg;
	dlg.save << [&] {
		SelectSaveFile("*.json\t*.*", MakeBuild::CreateSBOM(dlg.cs, ~dlg.mode));
	};

	Progress pi("Scanning");
	dlg.cs = CreateSBOMComponents(GetTargetTriplet(), pi);
	if(dlg.cs.GetCount() == 0)
		return;
	dlg.Perform();
}
