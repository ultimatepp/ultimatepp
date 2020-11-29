#include <Core/Core.h>

using namespace Upp;

String text;

CONSOLE_APP_MAIN
{
    Thread t;
    t.Run([] {
		text = "This is just a test of thread shutdown";
    });
    t.Wait();
    DDUMP(text);
}
