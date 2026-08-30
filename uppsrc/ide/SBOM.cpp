#include "ide.h"

void Ide::CreateSBOM()
{
	{
		UrepoConsole console;
		int errors = 0;
		
		TheIde()->VcpkgInstallMissing([&](const String& cmd, const String& chdir)
		                             { return console.System(cmd, chdir); });
	}

#ifdef PLATFORM_WIN32
	if(!IsVcpkgAvailable())
		return;
	SelectSaveFile("*.json\t*.*", MakeBuild::CreateSBOM(GetVcpkgTriplet()));
#endif
}
