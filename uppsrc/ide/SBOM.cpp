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
	
	Execute();
}

void Ide::CreateSBOM()
{
	SyncExternalDependencies(true);
	SBOMDlg dlg;
	dlg.save << [&] {
		SelectSaveFile("*.json\t*.*", MakeBuild::CreateSBOM(dlg.cs));
	};

	Progress pi("Scanning");
	dlg.cs = CreateSBOMComponents(GetTargetTriplet(), pi);
	if(dlg.cs.GetCount() == 0)
		return;
	dlg.Perform();
}
