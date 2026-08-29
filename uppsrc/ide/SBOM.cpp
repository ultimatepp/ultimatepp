#include "ide.h"

struct Component {
    String name;
    String supplier;

    String version;                // For shipped components (regardless of linking)

    bool isExternal = false;            // true for external dynamically linked platform/distro dependencies (not shipped)

    Vector<String> licenses;    // SPDX license IDs or expressions

    String homepage;
    String originUrl;
};

String Format8601(Time t)
{
	return Format("%04.4d%02.2d%02.2d`T%02.2d`:%02.2d`:%02.2d",
		          t.year, t.month, t.day, t.hour, t.minute, t.second);
}

void Ide::CreateSBOM()
{
	Vector<String> required = RequiredExternalDependencies("VCPKG");
#ifdef PLATFORM_WIN32
	if(IsVcpkgAvailable()) {
		Array<Component> cs;
		auto ReadComponent = [&](Value p) {
			Component& m = cs.Add();
			m.name = p["name"];
			m.version = p["versionInfo"];
			m.licenses << p["licenseConcluded"];
			m.homepage = p["homepage"];
			m.originUrl = p["downloadLocation"];
		};
		for(String x : required) {
			Value spdx = ParseJSON(LoadFile(GetExeDirFile("vcpkg") + "/installed/" + GetVcpkgTriplet() + "/share/" + x + "/vcpkg.spdx.json"));
			for(Value p : spdx["packages"]) {
				if(p["SPDXID"] == "SPDXRef-port") {
					ReadComponent(p);
					break;
				}
			}
		}
		
		const Workspace& wspc = IdeWorkspace();
		for(int i = 0; i < wspc.GetCount(); i++) {
			const Package& pk = wspc.GetPackage(i);
			String n = wspc[i];
			for(int i = 0; i < pk.file.GetCount(); i++) {
				String file = SourcePath(n, pk.file[i]);
				if(ToLower(GetFileName(file)) == "sbom.json") {
					Value sbom = ParseJSON(LoadFile(file));
					if(sbom.Is<ValueArray>()) {
						for(Value p : sbom)
							ReadComponent(p);
					}
					else
						ReadComponent(sbom);
				}
			}
		}
		

// --- CycloneDX Export ---
		ValueMap sbom;
		sbom.Add("bomFormat", "CycloneDX");
		sbom.Add("specVersion", "1.4");
		sbom.Add("version", 1);
		sbom.Add("serialNumber", "urn:uuid:" + Uuid::Create().ToString());

		// Metadata block
		ValueMap metadata;
		metadata.Add("timestamp", Format8601(GetSysTime()));
		ValueMap tool;
		tool.Add("vendor", "Ultimate++");
		tool.Add("name", "TheIDE"); // todo: umk when run from umk?
		ValueArray tools;
		tools.Add(tool);
		metadata.Add("tools", tools);
		sbom.Add("metadata", metadata);

		// Components block
		ValueArray components;
		for(const Component& c : cs) {
			ValueMap comp;
			comp.Add("type", "library");
			comp.Add("name", c.name);
			comp.Add("version", c.version);

			// Licenses
			if(!c.licenses.IsEmpty() && !IsNull(c.licenses[0])) {
				ValueMap lic;
				ValueMap licDetail;
				licDetail.Add("id", c.licenses[0]);
				lic.Add("license", licDetail);
				ValueArray licArray;
				licArray.Add(lic);
				comp.Add("licenses", licArray);
			}

			// External References (Homepage & Download Location)
			ValueArray extRefs;
			if(!IsNull(c.homepage) && !String(c.homepage).IsEmpty()) {
				ValueMap ref;
				ref.Add("type", "website");
				ref.Add("url", c.homepage);
				extRefs.Add(ref);
			}
			if(!IsNull(c.originUrl) && !String(c.originUrl).IsEmpty()) {
				ValueMap ref;
				ref.Add("type", "distribution");
				ref.Add("url", c.originUrl);
				extRefs.Add(ref);
			}
			if(extRefs.GetCount() > 0)
				comp.Add("externalReferences", extRefs);

			components.Add(comp);
		}
		sbom.Add("components", components);

		// Save to file
		SelectSaveFile("*.json\t*.*", AsJSON(Value(sbom), true));
    }
#endif
}
