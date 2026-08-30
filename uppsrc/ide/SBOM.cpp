#include "ide.h"

void Ide::CreateSBOM()
{
	Vector<String> required = RequiredExternalDependencies("VCPKG");
#ifdef PLATFORM_WIN32
	if(!IsVcpkgAvailable())
		return;
	SelectSaveFile("*.json\t*.*", MakeBuild::CreateSBOM(GetVcpkgTriplet()));
#endif
}
