#include "Builders.h"

struct Component {
    String name;
    String bom_ref;
    String supplier;

    String version;                      // For shipped components (regardless of linking)
	String purl;                         // PURL from package manager, if available
	Vector<String> sourceDistributions;  // Upstream source archive URLs, if available

    bool isExternal = false;             // true for external dynamically linked platform/distro dependencies (not shipped)

    Vector<String> licenses;             // SPDX license IDs or expressions

    String homepage;
    String originUrl;
};

String Format8601(Time t)
{
	return Format("%04.4d%02.2d%02.2d`T%02.2d`:%02.2d`:%02.2d",
		          t.year, t.month, t.day, t.hour, t.minute, t.second);
}

String MakeBuild::CreateSBOM(const String& triplet)
{
	Array<Component> cs;
	JsonArray dependencies;

#ifdef PLATFORM_WIN32
	Vector<String> required = RequiredExternalDependencies("VCPKG");
	DUMP(required);
	auto ReadComponent = [&](Value p) {
		Component& m = cs.Add();
		m.name = p["name"];
		m.version = p["versionInfo"];
		m.licenses << p["licenseConcluded"];
		m.homepage = p["homepage"];
		m.originUrl = p["downloadLocation"];
	
		for(Value r : p["externalRefs"])
			if(r["referenceType"] == "purl")
				m.purl = r["referenceLocator"];
	};

	for(int i = 0; i < required.GetCount(); i++) {
		String name = required[i];
		Value spdx = ParseJSON(LoadFile(
			GetExeDirFile("vcpkg") + "/installed/" + triplet + "/share/" + name + "/vcpkg.spdx.json"
		));
	
		for(Value p : spdx["packages"]) {
			if(p["SPDXID"] == "SPDXRef-port") {
				ReadComponent(p);
				Component& component = cs.Top();
				JsonArray deps;
				for(String depends : Split(Split(Split(Sys(VcpkgExe() + " depend-info " + component.name),
				                                       CharFilterCrLf).Top(), ':').Top(), ',')) {
					depends = TrimBoth(depends);
					if(!depends.StartsWith("vcpkg-")) {
						if(FindIndex(required, depends) < 0)
							required << depends;
						deps << depends;
					}
				}
				if(deps)
					dependencies << Json("ref", name)("dependsOn", deps);
				for(Value p : spdx["packages"]) {
					String id = p["SPDXID"];
					if(id.StartsWith("SPDXRef-resource-")) {
						String url = p["downloadLocation"];
						if(!IsNull(url) && url != "NONE")
							component.sourceDistributions << url;
					}
				}
				break;
			}
		}
	}
	
	const Workspace& wspc = GetIdeWorkspace();
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
#endif

	Json sbom;
	sbom("bomFormat", "CycloneDX")
	    ("specVersion", "1.4")
	    ("version", 1)
	    ("serialNumber", "urn:uuid:" + Uuid::Create().ToString())
	    ("metadata", Upp::Json("timestamp", Format8601(GetSysTime()))
	                          ("tools", JsonArray() << Json("vendor", "Ultimate++")
	                                                       ("name", "TheIDE"))); // todo: umk when run from umk?

	JsonArray components;
	for(const Component& c : cs) {
		JsonArray licenses;
		for(const String& s : c.licenses)
			if(!IsNull(s))
				licenses << Json("license", Json("id", s));
	
		JsonArray extRefs;
		if(!IsNull(c.homepage))
			extRefs << Json("type", "website")
			               ("url", c.homepage);
		if(!IsNull(c.originUrl))
			extRefs << Json("type", "distribution")
			               ("url", c.originUrl);
		
		for(const String& url : c.sourceDistributions)
			if(!IsNull(url))
				extRefs << Json("type", "source-distribution")
				               ("url", url);

		Json component;
		component("type", "library")
		         ("name", c.name)
		         ("bom-ref", c.bom_ref)
		         ("version", c.version);

		if(!IsNull(c.purl))
			component("purl", c.purl);

		if(licenses)
			component("licenses", licenses);
		if(extRefs)
			component("externalReferences", extRefs);

		components << component;
	}
	
	if(components)
		sbom("components", components);

	if(dependencies)
		sbom("dependencies", dependencies);

	return sbom;
}
