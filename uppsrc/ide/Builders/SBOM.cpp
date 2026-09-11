#include "Builders.h"

String Format8601Z(Time t)
{
	return Format("%04.4d-%02.2d-%02.2d`T%02.2d`:%02.2d`:%02.2d`Z",
		          t.year, t.month, t.day, t.hour, t.minute, t.second);
}

Array<SBOMComponent> MakeBuild::CreateSBOMComponents(const String& triplet, Gate<int, int> progress)
{
	Array<SBOMComponent> cs;

	Index<String> deps_done;

	auto AddDependency = [&](SBOMComponent& m, const String& name) {
		for(String h : { m.bom_ref + "\v" + name, name + "\v" + m.bom_ref }) {
			if(deps_done.Find(h) >= 0)
				return;
			deps_done.Add(h);
		}
		m.depends << name;
	};

	auto ReadComponent = [&](Value p) {
		SBOMComponent& m = cs.Add();
		m.name = p["name"];
		m.version = p["versionInfo"];
		m.licenses << ~p["licenseConcluded"];
		
		m.homepage = p["homepage"];
		m.originUrl = p["downloadLocation"];
	
		for(Value r : p["externalRefs"])
			if(r["referenceType"] == "purl")
				m.purl = r["referenceLocator"];
	};
	
	Index<String> required;
	VectorMap<String, String> override_licenses;

	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		if(progress(i, wspc.GetCount())) {
			cs.Clear();
			return cs;
		}
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

		SBOMComponent& m = cs.Add();
		if(i == 0)
			m.type = "application";
		auto PkgName = [=](const String& s) {
			 return "u++pkg:" + Filter(s, [](int c) { return c == '\\' ? '/' : c; });
		};
		m.bom_ref = m.name = PkgName(n);

#ifdef PLATFORM_WIN32
		String git = GetExeDirFile("bin/mingit/cmd/git");
#else
		String git = "git";
#endif

		git << " -C " << PackageDirectory(n) << " ";
		
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

		m.licenses << Nvl(pk.license_id, "BSD-2-Clause");
		
		for(const OptItem& u : pk.uses)
            AddDependency(m, PkgName(u.text));

		for(auto s : RequiredExternalDependenciesInfo(pk)) {
			AddDependency(m, s.name);
			required.FindAdd(s.name);
			if(s.license.GetCount())
				override_licenses.GetAdd(s.name) = s.license;
		}
	}

#ifdef PLATFORM_POSIX
	for(int i = 0; i < required.GetCount(); i++) {
		if(progress(i, required.GetCount())) {
			cs.Clear();
			return cs;
		}
		String name = required[i];
		SBOMComponent& m = cs.Add();
		m.bom_ref = m.name = name;
		String depends, archAndSource;
		SplitTo(Sys("dpkg-query -W -f='${Depends}\n${Version}\n${Homepage}\n${Architecture} ${Source}' " + name), '\n', false,
		        depends, m.version, m.homepage, archAndSource);
		
		int sp = archAndSource.Find(' ');
		String arch    = archAndSource.Left(sp);                    // sp<0 → whole string is arch
		String source  = sp >= 0 ? archAndSource.Mid(sp + 1) : "";
		
		String distro = "debian";
		String osr = LoadFile("/etc/os-release");
		for(String l : Split(osr, '\n'))
			if(l.TrimStart("ID="))
				distro = TrimBoth(l);
		
		m.purl = "pkg:deb/" + distro + "/" + name + "@" + m.version + "?arch=" + arch;
		if(source.GetCount() && source != name)
			m.purl << "&source=" + source;
		
		for(String dep : Split(depends, ',')) {
			int q = dep.Find('(');
			if(q >= 0)
				dep.Trim(q);
			dep = TrimBoth(dep);
			required.FindAdd(dep);
			AddDependency(m, dep);
		}

		FileIn in("/usr/share/doc/" + name + "/copyright");
		while(!in.IsEof()) {
			String l = in.GetLine();
			if(l.TrimStart("License: ")) {
				m.licenses << l;
				break;
			}
		}
	}
#endif

#ifdef PLATFORM_WIN32
	for(int i = 0; i < required.GetCount(); i++) {
		if(progress(i, required.GetCount())) {
			cs.Clear();
			return cs;
		}

		String name = required[i];
		Value spdx = ParseJSON(LoadFile(
			GetExeDirFile("vcpkg") + "/installed/" + triplet + "/share/" + name + "/vcpkg.spdx.json"
		));
	
		for(Value p : spdx["packages"]) {
			if(p["SPDXID"] == "SPDXRef-port") {
				ReadComponent(p);
				SBOMComponent& component = cs.Top();
				component.bom_ref = component.name;
				for(String depends : Split(Split(Split(Sys(VcpkgExe() + " depend-info " + component.name),
				                                       CharFilterCrLf).Top(), ':').Top(), ',')) {
					depends = TrimBoth(depends);
					if(!depends.StartsWith("vcpkg-")) {
						required.FindAdd(depends);
						AddDependency(component, depends);
					}
				}
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

	for(SBOMComponent& c : cs) {
		String ol = override_licenses.Get(c.name, Null);
		if(ol.GetCount()) {
			c.licenses.Clear();
			c.licenses << ol;
		}
	}

	return cs;
}

String MakeBuild::CreateSBOM(const String& triplet)
{
	return CreateSBOM(CreateSBOMComponents(triplet));
}

String MakeBuild::CreateSBOM(const Array<SBOMComponent>& cs)
{
	auto EmitLicense = [&](JsonArray& arr, const String& s) {
		if(s.IsEmpty() || s == "NOASSERTION")
			return;                                        // unknown -> omit, don't assert
		
		static Index<String> ops;
		static Index<String> lcspdx;
		static Vector<String> canon;                       // parallel: lowercase → canonical
		
		if(canon.IsEmpty()){
			for(int i = 0; i < SPDXLicenses().GetCount(); i++) {
				canon << SPDXLicenses()[i];
				lcspdx.Add(ToLower(SPDXLicenses()[i]));
			}
			ops.Add("(");
			ops.Add(")");
			ops.Add("or");
			ops.Add("and");
			ops.Add("with");
		}

		String low = ToLower(s);
		low.TrimStart("(");
		low.TrimEnd(")");
		Vector<String> vec = Split(low,' ');
		if(vec.GetCount()>=3){
			for(int i = 0; i < vec.GetCount() ; i++){
				if(lcspdx.Find(vec[i])<0 && ops.Find(vec[i])<0) break; // Not valid expression
				if(i == (vec.GetCount() - 1)){
					String lic;
					for(int i = 0; i < vec.GetCount() ; i++){
						int o = ops.Find(vec[i]);
						if(o>=0) lic << ToUpper(ops[o]) << " ";
						else{
							int l = lcspdx.Find(vec[i]);
							if(l>=0) lic << canon[l] << " ";
						}
					}
					arr << Json("expression", TrimRight(lic)); // Case-corrected validated SPDX expression
					return;
				}
			}
		}
		
		int f = lcspdx.Find(low);
		if(f >= 0) {
			arr << Json("license", Json("id", canon[f]));   // Case-corrected validated SPDX ID
			return;
		}
		
		arr << Json("license", Json("name", s));            // everything else: free text
	};

	JsonArray dependencies;
	JsonArray components;
	Json      main_component;

	for(const SBOMComponent& c : cs) {

		JsonArray licenses;

		for(const String& s : c.licenses)
			if(!IsNull(s))
				EmitLicense(licenses, s);
	
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

		JsonArray deps;
		for(String dep : c.depends)
			deps << dep;
		if(deps)
			dependencies << Json("ref", c.bom_ref)("dependsOn", deps);
	}

	Json sbom;
	sbom("bomFormat", "CycloneDX")
	    ("specVersion", "1.6")
	    ("version", 1)
	    ("serialNumber", "urn:uuid:" + Uuid::CreateV4().ToStringWithDashes())
	    ("metadata", Upp::Json("timestamp", Format8601Z(GetUtcTime()))
	                          ("tools", JsonArray() << Json("vendor", "U++")
	                                                       ("name", UMK ? "umk" : "TheIDE"))
	                          ("component", main_component));
	
	if(components)
		sbom("components", components);

	if(dependencies)
		sbom("dependencies", dependencies);

	return sbom;
}

