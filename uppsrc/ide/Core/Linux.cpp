#include "Core.h"

#ifdef PLATFORM_POSIX

String ExternalDependenciesManagerId()
{
	return "DPKG"; // TODO add rpm mode...
}

Index<String> InstalledExternalDependencies(const String&)
{
	Index<String> r;
	for(String l : Split(Sys("dpkg-query -W -f='${Package}\n'"), '\n'))
		r.FindAdd(TrimBoth(l));
	return r;
}

String InstallMissingExternalDependenciesCommand0(const String& triplet)
{
	String cmd = "apt-get install -y";
	for(String name : MissingExternalDependencies(triplet))
		cmd << " " << name;
	return cmd;
}

String InstallMissingExternalDependenciesCommand(const String& triplet)
{
	return "sudo " + InstallMissingExternalDependenciesCommand0(triplet);
}

bool InstallMissingExternalDependencies(Function<int(const String&, const String& chdir)> sys, const String& triplet)
{
	String cmd = InstallMissingExternalDependenciesCommand0(triplet);
	return sys(String(UMK ? "" : "pkexec ") + cmd, Null) == 0;
}

bool CanInstallMissingExternalDependencies()
{
	static bool is = Sys("which pkexec").GetCount();
	return is;
}


#endif