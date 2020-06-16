#include <Core/Core.h>

using namespace Upp;

String RandomString(int len)
{
	String h;
	while(len-- > 0)
		h.Cat(Random(96) + 32);
	return h;
}

CONSOLE_APP_MAIN
{
	Buffer<String> s(10000), t(10000);
	
	for(int i = 0; i < 10000; i++)
		s[i] = RandomString(Random(20));

	RTIMING("Set0");
	for(int j = 0; j < 10000; j++) {
		for(int i = 0; i < 10000; i++)
			t[i] = ~s[i];
	}
}
