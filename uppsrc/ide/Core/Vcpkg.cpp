#include "Core.h"

String VcpkgExe()
{
	return GetExeFolder() + "/vcpkg/vcpkg.exe";
}

bool IsVcpkgInstalled()
{
	return FileExists(VcpkgExe());
}

bool InstallVcpkg(Function<int(const String&, const String& chdir)> sys)
{
	String exedir = GetExeFolder();
	return sys("git clone https://github.com/microsoft/vcpkg.git", exedir) == 0 &&
	       sys("cmd /c \"" + exedir + "/vcpkg/bootstrap-vcpkg.bat\"", Null) == 0;
}

Vector<VcpkgInstalled> VcpkgList()
{
	VectorMap<String, VcpkgInstalled> ms;
	for(Value l : ParseJSON(Sys(VcpkgExe() + " list --x-json"))) {
		String name = ~l["package_name"];
		VcpkgInstalled& m = ms.GetAdd(name);
		m.name = name;
		m.triplets.FindAdd(~l["triplet"]);
		m.version = ~l["version"];
		String p = ~l["port_version"];
		if(p != "0")
			m.version << '#' << p;
		if(l["desc"].GetCount())
			m.desc = l["desc"][0];
	}
	return ms.PickValues();
}

Vector<String> RequiredExternalDependencies(const String& manager)
{
	Vector<String> required;
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pkg = wspc.GetPackage(i);
		for(const OptItem& m : pkg.external_dependency) {
			if(IsNull(m.when) || m.when == manager)
				required.Add(m.text);
		}
	}
	Sort(required);
	return required;
}

Vector<String> VcpkgTriplets()
{
	Index<String> ts;
	for(FindFile ff(ConfigFile("*.bm")); ff; ff.Next()) {
		VectorMap<String, String> vars;
		String fn = ConfigFile(ff.GetName());
		if(LoadVarFile(fn, vars)) {
			String builder = vars.Get("BUILDER", Null);
			String compiler = vars.Get("COMPILER", Null);
			
			if(builder == "CLANG")
				ts.FindAdd(compiler.Find("i686") >= 0 ? "x86-mingw-static-release" : "x64-mingw-static-release");
			if(builder.StartsWith("MSC"))
				ts.FindAdd(builder.Find("64") >= 0 ? "x64-windows-static-md" : "x86-windows-static-md");
		}
	}
	
	Vector<String> triplets = ts.PickKeys();
	Sort(triplets);
	return triplets;
}

bool VcpkgHasInstalled(Vector<VcpkgInstalled>& items, const String& name, const String& triplet)
{
	for(const VcpkgInstalled& m : items)
		if(m.name == name && m.triplets.Find(triplet) >= 0)
			return true;
	return false;
}
