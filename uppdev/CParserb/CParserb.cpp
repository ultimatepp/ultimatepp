#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	CParser p("i if while 12345 alfa");
	__BREAK__;
	int q = p.Id("i") + p.Id("if") + p.Id("while");
	Cout() << q;
}
