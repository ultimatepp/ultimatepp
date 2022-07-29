#include <ide/ide.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	DDUMP(CleanupId("Upp::Index<Upp::String>::Find(const Upp::String &k) const"));
	DDUMP(CleanupId("clang_PrintingPolicy_setProperty(CXPrintingPolicy Policy, enum CXPrintingPolicyProperty Property, unsigned int Value) __attribute__((dllimport))"));
	
	CheckLogEtalon();
}
