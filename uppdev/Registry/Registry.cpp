#include <Core/Core.h>

CONSOLE_APP_MAIN
{
	Cout() << GetWinRegString("InstallationFolder",
	                          "Software\\Microsoft\\Microsoft SDKs\\Windows\\v6.0") << "\r\n";
}
