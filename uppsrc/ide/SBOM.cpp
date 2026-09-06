#include "ide.h"

void Ide::CreateSBOM()
{
	String triplet;
#ifdef PLATFORM_WIN32
	{
		UrepoConsole console;
		int errors = 0;
		
		TheIde()->VcpkgInstallMissing([&](const String& cmd, const String& chdir)
		                             { return console.System(cmd, chdir); });
	}

	if(!IsVcpkgAvailable())
		return;
	
	triplet = GetVcpkgTriplet();
#endif

	SelectSaveFile("*.json\t*.*", MakeBuild::CreateSBOM(triplet));
}
