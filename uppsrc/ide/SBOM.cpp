#include "ide.h"

void Ide::CreateSBOM()
{
#ifdef PLATFORM_WIN32
	{
		UrepoConsole console;
		int errors = 0;
		
		TheIde()->VcpkgInstallMissing([&](const String& cmd, const String& chdir)
		                             { return console.System(cmd, chdir); });
	}

	if(!IsVcpkgAvailable())
		return;

	SelectSaveFile("*.json\t*.*", MakeBuild::CreateSBOM(GetVcpkgTriplet()));
#endif
}
