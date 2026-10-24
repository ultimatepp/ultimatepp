#include "Core.h"

Index<String> AllHostPackagesList()
{
	Index<String> list;
	if(IsRPM() || IsDPKG())
		for(String s : Split(Sys(IsRPM() ? "dnf repoquery --queryformat '%{name} - %{summary}' '*'"
		                                 : "apt-cache search ."), '\n')) {
			int q = s.Find(" - ");
			if(q >= 0)
				s.Trim(q);
			list.FindAdd(s);
		}
	if(IsBREW())
		for(const auto& m : GetBrewPackages())
			list.FindAdd(m.name);
	if(IsVCPKG())
		for(Value l : ParseJSON(Sys(VcpkgExe() + " search --x-json")))
			list.FindAdd(~l["package_name"]);
	return list;
}


void ParseExternalDependencyInfo(Vector<ExternalDependencyInfo>& r, const String& txt)
{
	int i0 = r.GetCount();
	String license;
	for(String s : Split(txt, ' '))
		if(s.TrimStart("license(") && s.TrimEnd(")"))
			license = s;
		else {
			if(r.GetCount() && (*s == '|' || *r.Top().name.Last() == '|'))
				r.Top().name << s;
			else
				r.Add().name = s;
		}
	for(int i = i0; i < r.GetCount(); i++)
		r[i].license = license;
}

Vector<ExternalDependencyInfo> RequiredExternalDependenciesInfo(const Package& pkg, const Vector<String>& keys)
{
	Vector<ExternalDependencyInfo> required;
	for(const OptItem& m : pkg.external_dependency) {
		if(MatchWhen(m.when, keys)) {
			ParseExternalDependencyInfo(required, m.text);
		}
	}
	Sort(required, [](const ExternalDependencyInfo& a, const ExternalDependencyInfo& b) { return a.name < b.name; });
	return required;
}

Vector<String> GetExternalDependenciesKeys()
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
	return keys;
}

Vector<ExternalDependencyInfo> RequiredExternalDependenciesInfo(const Package& pkg)
{
	return RequiredExternalDependenciesInfo(pkg, GetExternalDependenciesKeys());
}

Vector<ExternalDependencyInfo> RequiredExternalDependenciesInfo(const Vector<String>& keys)
{
	Vector<ExternalDependencyInfo> required;
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pkg = wspc.GetPackage(i);
		required.Append(RequiredExternalDependenciesInfo(pkg, keys));
	}
	Sort(required, [](const ExternalDependencyInfo& a, const ExternalDependencyInfo& b) { return a.name < b.name; });
	return required;
}

Vector<ExternalDependencyInfo> RequiredExternalDependenciesInfo()
{
	return RequiredExternalDependenciesInfo(GetExternalDependenciesKeys());
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
	Index<String> missi;
	Index<String> available = AllHostPackagesList();
	Index<String> installed = InstalledExternalDependencies(triplet);
	for(String s : RequiredExternalDependencies()) {
		Vector<String> ss = Split(s, '|');
		if(ss.GetCount() > 1)
			for(String h : ss)
				if(available.Find(h) >= 0) {
					s = h;
					break;
				}
		if(installed.Find(s) < 0)
			missi.FindAdd(s);
	}
	Vector<String> missing = missi.PickKeys();
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
	return "VCPKG";
#endif
#ifdef PLATFORM_OSX
	return "BREW";
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
#ifdef PLATFORM_OSX
	if(IsBREW())
		for(String l : Split(Sys("/opt/homebrew/bin/brew list"), '\n'))
			r.FindAdd(TrimBoth(l));
#endif
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
#ifdef PLATFORM_OSX
	if(IsBREW()) {
		cmd = "/opt/homebrew/bin/brew install -y ";
		for(String name : MissingExternalDependencies(triplet))
			cmd << " " << name;
	}
#endif
#ifdef PLATFORM_LINUX
	if(IsRPM() || IsDPKG()) {
		cmd = IsRPM() ? "dnf install -y" : "apt-get install -y";
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
	if(IsBREW())
		return InstallMissingExternalDependenciesCommand0(triplet);
	return "sudo " + InstallMissingExternalDependenciesCommand0(triplet);
}

bool InstallMissingExternalDependencies(Function<int(const String&, const String& chdir)> sys, const String& triplet)
{
	if(IsVCPKG())
		return VcpkInstallMissingExternalDependencies(sys, triplet);
	String cmd = InstallMissingExternalDependenciesCommand0(triplet);
	return cmd.GetCount() ? sys(String(UMK || IsBREW() ? "" : "pkexec ") + cmd, Null) == 0 : false;
}

bool CanInstallMissingExternalDependencies()
{
	if(IsVCPKG() || IsBREW())
		return true;

	static bool is = Sys("which pkexec").GetCount();
	return is;
}
