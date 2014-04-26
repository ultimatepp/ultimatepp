#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	{
		CParser p("  someid     $$      and");
		LOG("GetPtr:      " << AsCString(p.GetPtr()));
		LOG("GetSpacePtr: " << AsCString(p.GetSpacePtr()));
		ASSERT(p.Id("someid"));
		LOG("GetPtr:      " << AsCString(p.GetPtr()));
		LOG("GetSpacePtr: " << AsCString(p.GetSpacePtr()));
		ASSERT(p.Char('$'));
		LOG("GetPtr:      " << AsCString(p.GetPtr()));
		LOG("GetSpacePtr: " << AsCString(p.GetSpacePtr()));
		ASSERT(p.Char('$'));
		LOG("GetPtr:      " << AsCString(p.GetPtr()));
		LOG("GetSpacePtr: " << AsCString(p.GetSpacePtr()));
	}
	
	{
		CParser p("/* TEST */");
		p.NoSkipComments();
		ASSERT(p.Char2('/','*'));
		ASSERT(p.Id("TEST"));
		ASSERT(p.Char2('*','/'));
	}

	LOG("========== OK");
}
