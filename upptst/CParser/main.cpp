#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	CParser p("i if while 12345 alfa");
	ASSERT(p.Id("i"));
	ASSERT(p.Id("if"));
	ASSERT(p.Id("while"));
	ASSERT(p.IsInt());
	ASSERT(p.ReadInt() == 12345);
	ASSERT(p.IsId());
	ASSERT(p.ReadId() == "alfa");
}
