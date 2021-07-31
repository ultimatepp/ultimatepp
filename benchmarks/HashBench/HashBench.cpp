#include <Core/Core.h>

using namespace Upp;

#define LDUMP(x) x

CONSOLE_APP_MAIN
{
	String s;
	for(int i = 0; i < 1000000; i++)
		s.Cat(Random());
	
	for(int i = 0; i < 100; i++) {
		{
			RTIMING("SHA1");
			SHA1String(s);
		}
		{
			RTIMING("SHA256");
			SHA256String(s);
		}
		{
			RTIMING("MD5");
			MD5String(s);
		}
		{
			RTIMING("xxHash");
			xxHash(s);
		}
		{
			RTIMING("Save");
			SaveFile("c:/xxx/delete.bin", s);
		}
		{
			RTIMING("Load");
			LoadFile("c:/xxx/delete.bin");
		}
	}
}
