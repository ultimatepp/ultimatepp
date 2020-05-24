#include <Core/Core.h>

using namespace Upp;

// U++ Ptr class is sort of 'weak pointer' whose main purpose it to become NULL after pointee
// is destructed

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
