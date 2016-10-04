#include <Core/Core.h>

using namespace Upp;

#define DLLFILENAME "Kernel32.dll"
#define DLIMODULE   Win32Kernel32W
#define DLIHEADER   <CharsetW/Kernel32W.dli>
#include <Core/dli.h>

#define DLLFILENAME "Mpr.dll"
#define DLIMODULE   Win32MprW
#define DLIHEADER   <CharsetW/MprW.dli>
#include <Core/dli.h>

CONSOLE_APP_MAIN
{
	if(Win32Kernel32W())
		Cout() << "Kernel OK\n";
	else
		Cout() << "Kernel32W not available\n";
	if(Win32MprW())
		Cout() << "Mpr OK\n";
	else
		Cout() << "MprW not available\n";
}
