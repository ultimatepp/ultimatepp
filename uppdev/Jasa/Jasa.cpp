#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	for(;;) {
		String s = ReadStdIn();
		if(s.GetLength() == 0)
			break;
		int a = atoi(s);
		s = ReadStdIn();
		int b = atoi(s);
		Cout() << a << '+' << b << '=' << a + b << '\n';
		Cout() << a << '-' << b << '=' << a - b << '\n';
		Cout() << a << '*' << b << '=' << a * b << '\n';
	}
}
