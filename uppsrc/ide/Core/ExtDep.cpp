#include "Core.h"

ExternalDependencyInfo GetExternalDependencyInfo(const String& txt)
{
	ExternalDependencyInfo f;
	f.name = txt;
	int q = txt.Find(' ');
	if(q >= 0) {
		f.name = txt.Mid(0, q);
		String h = TrimBoth(txt.Mid(q));
		if(h.TrimStart("license(")) {
			try {
				CParser p(h);
				f.license = p.ReadString();
			}
			catch(CParser::Error) {}
		}
	}
	return f;
}

Vector<ExternalDependencyInfo> RequiredExternalDependenciesInfo(const Package& pkg)
{
	Vector<String> keys;
	keys << ExternalDependenciesManagerId();
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
	String ed;
	Vector<ExternalDependencyInfo> required;
	for(const OptItem& m : pkg.external_dependency) {
		if(MatchWhen(m.when, keys)) {
			ExternalDependencyInfo f = GetExternalDependencyInfo(m.text);
			if(f.name.GetCount())
				required << f;
		}
	}
	Sort(required, [](const ExternalDependencyInfo& a, const ExternalDependencyInfo& b) { return a.name < b.name; });
	return required;
}

Vector<ExternalDependencyInfo> RequiredExternalDependenciesInfo()
{
	Vector<ExternalDependencyInfo> required;
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pkg = wspc.GetPackage(i);
		required.Append(RequiredExternalDependenciesInfo(pkg));
	}
	Sort(required, [](const ExternalDependencyInfo& a, const ExternalDependencyInfo& b) { return a.name < b.name; });
	return required;
}

Vector<String> RequiredExternalDependencies(const Package& pkg)
{
	Vector<String> required;
	for(const auto& h :  RequiredExternalDependenciesInfo(pkg))
		required << h.name;
	return required;
}

Vector<String> RequiredExternalDependencies()
{
	Vector<String> required;
	for(const auto& h :  RequiredExternalDependenciesInfo())
		required << h.name;
	return required;
}

Vector<String> MissingExternalDependencies(const String& triplet)
{
	Vector<String> missing;
	Index<String> installed = InstalledExternalDependencies(triplet);
	for(String s : RequiredExternalDependencies())
		if(installed.Find(s) < 0)
			missing << s;
	Sort(missing);
	return missing;
}

bool HasDPKG()
{
	static bool dpkg
#ifdef PLATFORM_LINUX
		 = Sys("which dpkg").GetCount()
#endif
	;
	return dpkg;
}

bool HasRPM()
{
	static bool rpm
#ifdef PLATFORM_LINUX
		 = Sys("which rpm").GetCount()
#endif
	;
	return rpm;
}

String ExternalDependenciesManagerId()
{
#ifdef PLATFORM_WIN32
//	if(GetVar("VCPKG") == "1")
		return "VCPKG";
#endif
	if(HasDPKG())
		return "DPKG";
	if(HasRPM())
		return "RPM";
	return Null;
}

Index<String> InstalledExternalDependencies(const String& triplet)
{
	if(IsVCPKG())
		return VcpkgInstalledExternalDependencies(triplet);
	Index<String> r;
#ifdef PLATFORM_LINUX
	if(IsDPKG())
		for(String l : Split(Sys("dpkg-query -W -f='${Package}\n'"), '\n'))
			r.FindAdd(TrimBoth(l));
	if(IsRPM())
		for(String l : Split(Sys("rpm -qa --qf '%{NAME}\n'"), '\n'))
			r.FindAdd(TrimBoth(l));
#endif
	return r;
}

String InstallMissingExternalDependenciesCommand0(const String& triplet)
{
	String cmd;
#ifdef PLATFORM_LINUX
	if(IsRPM() || IsDPKG()) {
		String cmd = IsRPM() ? "dnf install -y" : "apt-get install -y";
		for(String name : MissingExternalDependencies(triplet))
			cmd << " " << name;
	}
#endif
	return cmd;
}

String InstallMissingExternalDependenciesCommand(const String& triplet)
{
	if(IsVCPKG())
		return Null;
	return "sudo " + InstallMissingExternalDependenciesCommand0(triplet);
}

bool InstallMissingExternalDependencies(Function<int(const String&, const String& chdir)> sys, const String& triplet)
{
	if(IsVCPKG())
		return VcpkInstallMissingExternalDependencies(sys, triplet);
	String cmd = InstallMissingExternalDependenciesCommand0(triplet);
	return cmd.GetCount() ? sys(String(UMK ? "" : "pkexec ") + cmd, Null) == 0 : false;
}

bool CanInstallMissingExternalDependencies()
{
	if(IsVCPKG())
		return true;
	static bool is = Sys("which pkexec").GetCount();
	return is;
}
