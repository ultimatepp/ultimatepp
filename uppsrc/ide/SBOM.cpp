#include "ide.h"

struct SBOMDlg : WithSBOMLayout<TopWindow> {
	Array<SBOMComponent> cs;
	
	SBOMDlg();

	void Perform();
};

SBOMDlg::SBOMDlg()
{
	CtrlLayoutExit(*this, "Software Bill Of Materials");
	
	list.AddColumn("Name", 200);
	list.AddColumn("Type", 80);
	list.AddColumn("Version", 120);
	list.AddColumn("License", 300);
	list.AddColumn("Depends", 300);
	list.AddColumn("PURL", 300);
	list.AddColumn("Homepage", 300);
	list.AddColumn("Origin", 300);
	list.AddColumn("Source distributions", 300);
	list.HeaderObject().Absolute();
	list.NoCursor();
}

void SBOMDlg::Perform()
{
	for(SBOMComponent& m : cs)
		list.Add(m.name,
		         m.type,
		         m.version,
		         Join(m.licenses, ", "),
		         Join(m.depends, ", "),
		         m.purl,
		         m.homepage,
		         m.originUrl,
		         Join(m.sourceDistributions, ", "));
	
	mode.Add(SBOM_BASE, "0 Do not include external dependecies");
	mode.Add(SBOM_FULL, "1 Include everything (win32-vcpkg / docker / flatpak)");
	mode.Add(SBOM_EXTERNAL_FULL, "2 Include everything, exclude external dependencies from CVE scanning (linux binary)");
	mode.Add(SBOM_DIRECT, "3 Include only direct external dependencies");
	mode.Add(SBOM_EXTERNAL_DIRECT, "4 Include only direct external dependencies, exclude external dependencies from CVE scanning");  // 4

	mode <<= 1;
#ifdef PLATFORM_LINUX
	mode <<= 2;
#endif
	
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
