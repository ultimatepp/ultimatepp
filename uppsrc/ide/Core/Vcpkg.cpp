#include "Core.h"

String VcpkgExe()
{
#ifdef PLATFORM_WIN32
	return GetExeFolder() + "/vcpkg/vcpkg.exe";
#else
	return GetHomeDirFile("vcpkg/vcpkg");
#endif
}

bool IsVcpkgInstalled()
{
	return FileExists(VcpkgExe());
}

bool InstallVcpkg(Function<int(const String&, const String& chdir)> sys)
{
#ifdef PLATFORM_WIN32
	String exedir = GetExeFolder();
	return sys("git clone https://github.com/microsoft/vcpkg.git", exedir) == 0 &&
	       sys("cmd /c \"" + exedir + "/vcpkg/bootstrap-vcpkg.bat\"", Null) == 0;
#else
	String homedir = GetHomeDirectory();
	return sys("git clone https://github.com/microsoft/vcpkg.git", homedir) == 0 &&
	       sys("/bin/sh -e \"" + homedir + "/vcpkg/bootstrap-vcpkg.sh\"", Null) == 0;
#endif
}

bool no_vcpkg_install;

bool IsVcpkgAvailable(Function<int(const String&, const String& chdir)> sys)
{
	if(!IsVcpkgInstalled() && !no_vcpkg_install)
		InstallVcpkg(sys);
	return IsVcpkgInstalled();
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

String VcpkgTriplet(const String& builder, const String& compiler, bool so)
{
#ifdef PLATFORM_WIN32
	if(builder == "CLANG")
		return compiler.Find("i686") >= 0 ? so ? "x86-mingw-dynamic-release" : "x86-mingw-static-release"
		                                  : so ? "x64-mingw-dynamic-release" : "x64-mingw-static-release";
	if(builder.StartsWith("MSC"))
		return builder.Find("64") >= 0 ? so ? "x64-windows" : "x64-windows-static"
		                               : so ? "x86-windows" : "x86-windows-static";
	return "x64-mingw-static-release";
#else
#ifdef CPU_ARM
	String cpu = "arm64";
#else
	String cpu = "x64";
#endif
	return cpu + (so ? "-linux-dynamic" : "-linux");
#endif
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
	return sys(VcpkgExe() + " install --recurse " + name + ":" + triplet, Null) == 0;
}

Index<String> VcpkgInstalledExternalDependencies(const String& triplet)
{
	Index<String> r;
	Vector<VcpkgInstalled> installed = VcpkgList();
	for(const VcpkgInstalled& m : installed)
		if(m.triplets.Find(triplet) >= 0)
			r.FindAdd(m.name);
	return r;
}

bool VcpkInstallMissingExternalDependencies(Function<int(const String&, const String& chdir)> sys, const String& triplet)
{
	bool ok = true;
	Vector<String> missing = MissingExternalDependencies(triplet);
	if(missing.GetCount()) {
		if(!IsVcpkgInstalled())
			InstallVcpkg(sys);
		for(String name : missing)
			if(!VcpkgInstall(sys, name, triplet))\
				ok = false;
	}
	return ok;
}
