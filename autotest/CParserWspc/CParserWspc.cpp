#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	CParser p("  someid     $$      and");
	LOG("GetPtr:      " << AsCString(p.GetPtr()));
	LOG("GetSpacePtr: " << AsCString(p.GetSpacePtr()));
	p.Id("someid");
	LOG("GetPtr:      " << AsCString(p.GetPtr()));
	LOG("GetSpacePtr: " << AsCString(p.GetSpacePtr()));
	p.Char('$');
	LOG("GetPtr:      " << AsCString(p.GetPtr()));
	LOG("GetSpacePtr: " << AsCString(p.GetSpacePtr()));
	p.Char('$');
	LOG("GetPtr:      " << AsCString(p.GetPtr()));
	LOG("GetSpacePtr: " << AsCString(p.GetSpacePtr()));
	LOG("========== OK");
}
