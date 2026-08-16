#include "Core.h"

#ifdef PLATFORM_WIN32

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

	Vector<String> keys;
	keys << manager;
#ifdef PLATFORM_WIN32
	keys << "WIN32";
#endif
#ifdef PLATFORM_POSIX
	keys << "POSIX";
#endif
#ifdef PLATFORM_LINUX
	keys << "LINUX";
#endif
#ifdef PLATFORM_MACOS
	keys << "MACOS";
#endif
#ifdef PLATFORM_BSD
	keys << "BSD";
#endif

	int maxlen = -1;
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pkg = wspc.GetPackage(i);
		for(const OptItem& m : pkg.external_dependency) {
			if(MatchWhen(m.when, keys) && m.when.GetCount() > maxlen) {
				maxlen = m.when.GetCount();
				required.Add(m.text);
			}
		}
	}
	Sort(required);
	return required;
}

String VcpkgTriplet(const String& builder, const String& compiler, bool so)
{
	if(builder == "CLANG")
		return compiler.Find("i686") >= 0 ? so ? "x86-mingw-dynamic-release" : "x86-mingw-static-release"
		                                  : so ? "x64-mingw-dynamic-release" : "x64-mingw-static-release";
	if(builder.StartsWith("MSC"))
		return builder.Find("64") >= 0 ? so ? "x64-windows" : "x64-windows-static"
		                               : so ? "x86-windows" : "x86-windows-static";
	return "x64-mingw-static-release";
}

String VcpkgTriplet(const VectorMap<String, String>& vars, bool so)
{
	return VcpkgTriplet(vars.Get("BUILDER", Null), vars.Get("COMPILER", Null), so);
}

Vector<String> VcpkgTriplets()
{
	Index<String> ts;
	for(FindFile ff(ConfigFile("*.bm")); ff; ff.Next()) {
		VectorMap<String, String> vars;
		String fn = ConfigFile(ff.GetName());
		if(LoadVarFile(fn, vars)) {
			ts.FindAdd(VcpkgTriplet(vars, false));
			ts.FindAdd(VcpkgTriplet(vars, true));
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

bool VcpkgInstall(Function<int(const String&, const String& chdir)> sys, const String& name, const String& triplet)
{
	return sys(VcpkgExe() + " install " + name + ":" + triplet, Null) == 0;
}

#endif