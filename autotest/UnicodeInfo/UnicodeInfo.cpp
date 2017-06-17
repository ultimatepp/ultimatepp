#include <Core/Core.h>

using namespace Upp;

dword ScanHex(const char *s)
{
	return Nvl(ScanInt(s, NULL, 16));
}

CONSOLE_APP_MAIN
{
	FileIn in(GetDataFile("../../uppbox/Unicode/UnicodeData.txt"));

	while(!in.IsEof()) {
		DUMP(in.GetLine());
	}
	
	
	for(int i = 0; i < 0x10ffff; i++)
		if(IsLetter(i))
			DDUMPHEX(i);
}
