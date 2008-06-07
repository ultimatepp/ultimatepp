#include <Core/Core.h>

using namespace Upp;

struct Foo : Pte<Foo> {
	String text;
};

CONSOLE_APP_MAIN
{
	Ptr<Foo> ptr;
	{
		Foo foo;
		foo.text = "Text";
		ptr = &foo;
		Cout() << (void*)~ptr << " -> " << ptr->text << "\n";
	}
	Cout() << "-------------\n";
	Cout() << (void*)~ptr << "\n";
}
