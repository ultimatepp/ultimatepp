#include "x86disas.h"
#include <plugin/ndisasm/ndisasm.h>

CONSOLE_APP_MAIN
{
	byte *q = (byte *)main;
	dword ip = (dword) q;

	for(int i = 0; i < 50000; i++) {
		char h[256];
		int size = NDisassemble(h, q, ip);
		LOG(FormatIntHex(ip) << "  " << h);
		ip += size;
		q += size;
	}
}