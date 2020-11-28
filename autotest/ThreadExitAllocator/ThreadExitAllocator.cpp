#include <Core/Core.h>

using namespace Upp;

struct Foo {
	const char *text;
	Foo(const char *text) : text(text) { LOG("Foo: " << text); }
	~Foo() { LOG("~Foo: " << text); }
};

CONSOLE_APP_MAIN
{
    Thread t;
    t.Run([] {
        thread_local Foo foo("local in Thread");
        LOG("In the thread");
    });
    t.Wait();
}
