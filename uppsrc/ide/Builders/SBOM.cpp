#include "Builders.h"

struct Component {
    String name;
    String type = "library";
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

String Format8601Z(Time t)
{
	return Format("%04.4d%02.2d%02.2d`T%02.2d`:%02.2d`:%02.2d`Z",
		          t.year, t.month, t.day, t.hour, t.minute, t.second);
}

String MakeBuild::CreateSBOM(const String& triplet)
{
	Array<Component> cs;
	JsonArray dependencies;

#ifdef PLATFORM_WIN32
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

	Index<String> required;

	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pk = wspc.GetPackage(i);
		String n = wspc[i];
		for(String fn : pk.file) {
			String file = SourcePath(n, fn);
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

		Component& m = cs.Add();
		if(i == 0)
			m.type = "application";
		auto PkgName = [=](const String& s) {
			 return "u++pkg:" + Filter(s, [](int c) { return c == '\\' ? '/' : c; });
		};
		m.bom_ref = m.name = PkgName(n);
	
		String git = GetExeDirFile("bin/mingit/cmd/git") + " -C " + PackageDirectory(n) + " ";
		
		String origin = TrimBoth(Sys(git + "config --get remote.origin.url"));
		if(origin.GetCount()) {
			m.originUrl = "git+" + origin;
			origin.TrimEnd(".git");
			m.homepage = origin;
		}

		String ts, hash;
		if(SplitTo(TrimBoth(Sys(git + "log -1 --date=unix --format=\"%h %cd\"")), " ", hash, ts)) {
			m.version = Format8601Z(Atoi64(ts) + Time(1970, 1, 1)) + "#" + hash;
			if(m.originUrl.GetCount())
				m.sourceDistributions << m.originUrl + "@" + hash;
		}
		m.licenses << "BSD-2-Clause"; // todo
		
		JsonArray deps;
		for(const OptItem& u : pk.uses)
			deps << PkgName(u.text);

		for(String s : RequiredExternalDependencies(pk, "VCPKG")) {
			deps << s;
			required.FindAdd(s);
		}
		dependencies << Json("ref", m.name)("dependsOn", deps);
	}
	
	for(int i = 0; i < required.GetCount(); i++) {
		String name = required[i];
		Value spdx = ParseJSON(LoadFile(
			GetExeDirFile("vcpkg") + "/installed/" + triplet + "/share/" + name + "/vcpkg.spdx.json"
		));
	
		for(Value p : spdx["packages"]) {
			if(p["SPDXID"] == "SPDXRef-port") {
				ReadComponent(p);
				Component& component = cs.Top();
				component.bom_ref = component.name;
				JsonArray deps;
				for(String depends : Split(Split(Split(Sys(VcpkgExe() + " depend-info " + component.name),
				                                       CharFilterCrLf).Top(), ':').Top(), ',')) {
					depends = TrimBoth(depends);
					if(!depends.StartsWith("vcpkg-")) {
						required.FindAdd(depends);
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
#endif

	JsonArray components;
	Json main_component;
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
		component("type", c.type)
		         ("name", c.name)
		         ("bom-ref", c.bom_ref)
		         ("version", c.version);

		if(!IsNull(c.purl))
			component("purl", c.purl);

		if(licenses)
			component("licenses", licenses);
		if(extRefs)
			component("externalReferences", extRefs);
		
		if(!main_component)
			main_component = component;
		else
			components << component;
	}

	Json sbom;
	sbom("bomFormat", "CycloneDX")
	    ("specVersion", "1.4")
	    ("version", 1)
	    ("serialNumber", "urn:uuid:" + Uuid::CreateV4().ToString())
	    ("metadata", Upp::Json("timestamp", Format8601Z(GetUtcTime()))
	                          ("tools", JsonArray() << Json("vendor", "U++")
	                                                       ("name", "TheIDE")) // todo: umk when run from umk?
	                          ("component", main_component));
	
	if(components)
		sbom("components", components);

	if(dependencies)
		sbom("dependencies", dependencies);

	return sbom;
}
