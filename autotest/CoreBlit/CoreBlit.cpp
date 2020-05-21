#include <Core/Core.h>

using namespace Upp;

template <class T, class FN>
void CheckCopy(FN copy)
{
	LOG("CheckCopy " << sizeof(T));
	Buffer<T> b0(20000, 0), b1(20000, 0), b2(20000, 0);
	for(int pass = 0; pass < 4; pass++) {
		LOG(pass);
		for(int i = 0; i < 500000; i++) {
			int pos = Random(200);
			int len = Random(decode(pass, 0, 20, 1, 200, 2, 2000, 9999));
			memset(~b0 + pos, i, len * sizeof(T));
			pos = Random(200);
			len = Random(200);
			copy(~b1 + pos, ~b0 + pos, len);
			memcpy(~b2 + pos, ~b0 + pos, len * sizeof(T));
			if(memcmp(b1, b2, 20000)) {
				RDUMP(i);
				RDUMP(pos);
				RDUMP(len);
				Panic("Failed");
			}
		}
	}
}

template <class T, class FN>
void CheckFill(FN fill)
{
	LOG("CheckFill " << sizeof(T));
	Buffer<T> b1(20000, 0), b2(20000, 0);
	for(int pass = 0; pass < 4; pass++) {
		LOG(pass);
		for(int i = 0; i < 500000; i++) {
			int pos = Random(200);
			int len = Random(decode(pass, 0, 20, 1, 200, 2, 2000, 9999));
			pos = Random(200);
			len = Random(200);
			fill(~b1 + pos, i, len);
			for(int j = 0; j < len; j++)
				b2[pos + j] = i;
			if(memcmp(b1, b2, 20000)) {
				RDUMP(i);
				RDUMP(pos);
				RDUMP(len);
				Panic("Failed");
			}
		}
	}
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	CheckFill<byte>(svo_memset);
	CheckFill<dword>(memsetd);
	
	CheckCopy<byte>(svo_memcpy);
	CheckCopy<dword>(memcpyd);
	
	LOG("============= OK");
}
