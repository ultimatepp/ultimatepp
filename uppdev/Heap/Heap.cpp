#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	int oq = -1;
	for(int i = 300; i > 0; i--) {
		int sz = 4096 - 48;
		int q = (sz / i) & ~15;
		if(q != oq) {
			oq = q;
			LOG(sz / i << ' ' << q << ", wasted: " << sz - i * q);
		}
	}
	static int x[] = { 224, 288, /*336,*/ 368, 448, 576, 672, 1012, 1349, 2016, 4048 };
	for(int i = 1; i < __countof(x); i++) {
		DUMP(x[i]);
		DUMP((double)x[i] / x[i - 1]);
		DUMP(FormatIntHex(x[i]));
	}
	
//	288, 368, 448, 576
	
	static int bsz[] = { 288, 368, 448, 576, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999 };
//	for(int i = 225; i < 576; i++) {
//		LOG(i << ' ' << bsz[i >> 8]);
//	}
	
	/*
	Cout() << "Allocator test:\r\n";
//	HeapTest(256, 100000, true);
	Cout() << "Checked allocator test Passed\r\n";
	HeapTest(8192, 1000000, false);
	Cout() << "Quick allocator test Passed\r\n";
	HeapTest(8192, 100 * 1000000, false);
	Cout() << "Allocator test Passed\r\n";
	*/
}
