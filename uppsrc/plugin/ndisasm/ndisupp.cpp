#include <Core/Core.h>

#include "ndisasm.h"

extern "C" {
#undef INT64_MIN
#undef INT64_MAX
#undef UINT64_MAX

#include "lib/compiler.h"
#include "lib/inttypes.h"
#include "lib/disasm.h"
}

namespace Upp {

int NDisassemble(char *output, const byte *data, uint64 offset, bool x64)// returns instruction length
{
	int len = disasm(const_cast<unsigned char *>(data), output, 256, x64 ? 64 : 32, offset, false, 0);
	if(len <= 0) {
		eatbyte(const_cast<unsigned char *>(data), output, 256, x64 ? 64 : 32);
		len = 1;
	}
	return len;
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

}
