#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	const Vector<String>& cl = CommandLine();
	for(int i = 0; i < cl.GetCount(); i++)
		Cout() << cl[i];
}
