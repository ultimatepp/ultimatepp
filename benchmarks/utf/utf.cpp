#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Vector<dword> data;
	for(int i = 0; i < 20000; i++)
		data.Add(Random(100));
	
	String utf8 = ToUtf8(data.begin(), 200000);
	WString utf16 = utf8.ToWString();

	int n = 0;
	const int N = 2000;
	for(int q = 0; q < N; q++) {
		RTIMING("utf8 -> utf32");
		n += ToUtf32(utf8).GetCount();
	}
	for(int q = 0; q < N; q++) {
		RTIMING("utf8 -> utf16");
		n += ToUtf16(utf8).GetCount();
	}
	for(int q = 0; q < N; q++) {
		RTIMING("utf32 -> utf8");
		n += ToUtf8(data.begin(), 200000).GetCount();
	}
	for(int q = 0; q < N; q++) {
		RTIMING("utf16 -> utf8");
		n += utf16.ToString().GetCount();
	}
	RDUMP(n);
}
