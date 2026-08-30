#include "Builders.h"

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

String MakeBuild::CreateSBOM(const String& triplet)
{
	Vector<String> required = RequiredExternalDependencies("VCPKG");
#ifdef PLATFORM_WIN32
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
		Value spdx = ParseJSON(LoadFile(GetExeDirFile("vcpkg") + "/installed/" + triplet + "/share/" + x + "/vcpkg.spdx.json"));
		for(Value p : spdx["packages"]) {
			if(p["SPDXID"] == "SPDXRef-port") {
				ReadComponent(p);
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

				
		Json component;
		component("type", "library")
		         ("name", c.name)
		         ("version", c.version);

		if(licenses)
			component("licenses", licenses);
		if(extRefs)
			component("externalReferences", extRefs);

		components << component;
	}
	
	if(components)
		sbom("components", components);
	
	return sbom;
#endif
}
