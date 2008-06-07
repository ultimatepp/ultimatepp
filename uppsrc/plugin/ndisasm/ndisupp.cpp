#include <Core/Core.h>

#include "ndisasm.h"

extern "C" {
#include "lib/disasm.h"
}

NAMESPACE_UPP
#ifdef CPU_32
int NDisassemble(char *output, const byte *data, long offset)// returns instruction length
#else
int NDisassemble(char *output, const byte *data, int64 offset)// returns instruction length
#endif
{
	unsigned preferences = 0; // see lib/insns.h(84)
	long len = (long)disasm(const_cast<unsigned char *>(data), output, 32, offset, false, preferences);
	if(len <= 0) {
		eatbyte(const_cast<unsigned char *>(data), output);
		len = 1;
	}
	return (int)len;
}

#ifdef flagMAIN
CONSOLE_APP_MAIN
{
	long start = 0x4012f8, end = start + 256;
	while(start < end) {
		char line[NDISASM_OUTBUF_SIZE];
		start += NDisassemble(line, (const byte *)start, start);
		puts(line);
	}
}
#endif

END_UPP_NAMESPACE
