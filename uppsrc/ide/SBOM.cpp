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
	Sizeable();
	MaximizeBox();
	
	list.HeaderObject().Absolute();
	list.AddColumn("Name", 200);
	list.AddColumn("Type", 80);
	list.AddColumn("Version", 120);
	list.AddColumn("3rd party", 50);
	list.AddColumn("Shipped", 50);
	list.AddColumn("License", 300);
	list.AddColumn("Depends", 300);
	list.AddColumn("PURL", 300);
	list.AddColumn("Homepage", 300);
	list.AddColumn("Origin", 300);
	list.AddColumn("Source distributions", 300);
	list.NoCursor();
}

void SBOMDlg::Sync()
{
	int mod = ~mode;
	String s = ~search;
	int sc = list.GetScroll();
	list.Clear();
	auto GetSourceDistributions = [](const Vector<Tuple<String, String, String>>& sd) -> String {
		Index<String> seen;
		String out;
		for(const auto& t : sd)
			if(seen.Find(t.a) < 0) {
				seen.Add(t.a);
				if(!out.IsEmpty()) out << ", ";
				out << t.a;
			}
		return out;
	};
	for(SBOMComponent& m : cs) {
		String lic = Join(m.licenses, ", ");
		String dep = Join(m.depends, ", ");
		
		if(ToUpper(m.name + m.version + lic + dep + m.homepage + m.originUrl).Find(s) >= 0 &&
		  (mod != SBOM_BASE || m.external <= T_DYNAMIC_BUNDLED))
			list.Add(m.name,
			         m.type,
			         m.version,
			         m.external ? "Y" : "", // 3rd party
			         mod == SBOM_CONTAINER || m.external <= T_DYNAMIC_BUNDLED ? "Y" : "", // Shipped
			         lic,
			         dep,
			         m.purl,
			         m.homepage,
			         m.originUrl,
			         GetSourceDistributions(m.sourceDistributions));
	}
	list.ScrollTo(sc);
}


void SBOMDlg::Perform()
{
	mode.Add(SBOM_BASE, "Minimal SBOM (CRA/VEX)");
	mode.Add(SBOM_FULL, "Full SBOM (using platform dependencies)");
	mode.Add(SBOM_CONTAINER, "Full SBOM (shipping all dependencies)");

	mode <<= SBOM_FULL;

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
		SelectSaveFile("*.json", AsJSON(ParseJSON(MakeBuild::CreateSBOM(dlg.cs, ~dlg.mode)), true));
	};

	Progress pi("Scanning");
	dlg.cs = CreateSBOMComponents(GetTargetTriplet(), pi);
	if(dlg.cs.GetCount() == 0)
		return;
	dlg.Perform();
}
