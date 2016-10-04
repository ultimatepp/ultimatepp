#include <Core/Core.h>

using namespace Upp;

class StringBufferOf : public StringBuffer {
	String& tgt;

public:
	StringBufferOf(String& s) : tgt(s), StringBuffer(s) {}
	~StringBufferOf() { tgt = *this; }
};

CONSOLE_APP_MAIN
{
	String a = "Hello!";
	String b = a;
	{
		StringBufferOf x(a);
		x[2] = 'A';
	}
	DUMP(a);
	DUMP(b);
}
