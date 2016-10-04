#include <Core/Core.h>

using namespace Upp;

int QC(const char *a, const char *b);
int QC3(const char *a, const char *b);
int MemCmp(const char *a, const char *b, size_t len);

CONSOLE_APP_MAIN
{
	Index<String> wrd;
#ifdef PLATFORM_POSIX
	String f = LoadFile("/home/mirek/alice30.txt");
#else
	String f = LoadFile("e:/alice30.txt");
#endif
	int line = 1;
	const char *q = f;
	for(;;) {
		int c = *q;
		if(IsAlpha(c)) {
			const char *b = q++;
			while(IsAlNum(*q)) q++;
			if(q - b < 14)
				wrd.FindAdd(String(b, q));
		}
		else {
			if(!c) break;
			if(c == '\n')
				++line;
			q++;
		}
	}

	{
		RTIMING("MemCmp");
		int n = 0;
		for(int i = 0; i < wrd.GetCount(); i++)
			for(int j = 0; j < wrd.GetCount(); j++)
				if(MemCmp(~wrd[i], ~wrd[j], 14) < 0)
					n++;
		RDUMP(n);
	}

	{
		RTIMING("memcmp");
		int n = 0;
		for(int i = 0; i < wrd.GetCount(); i++)
			for(int j = 0; j < wrd.GetCount(); j++)
				if(memcmp(~wrd[i], ~wrd[j], 14) < 0)
					n++;
		RDUMP(n);
	}
}
