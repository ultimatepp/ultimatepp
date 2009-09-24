#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String h;
	for(int i = 128; i < 256; i++)
		h.Cat(i);
//	int chrset = CHARSET_ARMSCII_8;
	int chrset = CHARSET_CP1161;
	WString w = ToUnicode(h, chrset);
	for(int i = 0; i < 128; i++) {
		if(i + 128 != (int)FromUnicode(w[i], chrset))
			LOG(i + 128 << ' ' << (int)w[i] << ' ' << (int)FromUnicode(w[i], chrset) << " <-----");
		else
			LOG(i + 128 << ' ' << (int)w[i] << ' ' << (int)FromUnicode(w[i], chrset));
	}
}

