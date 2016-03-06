#include <Core/Core.h>

#include "ndisasm.h"

extern "C" {
#include "lib/disasm.h"
}

NAMESPACE_UPP

int NDisassemble(char *output, const byte *data, uint64 offset, bool x64)// returns instruction length
{
	int len = disasm(const_cast<unsigned char *>(data), output, 256, x64 ? 64 : 32, offset, false, 0);
	if(len <= 0) {
		eatbyte(const_cast<unsigned char *>(data), output, 256, x64 ? 64 : 32);
		len = 1;
	}
	return len;
}

END_UPP_NAMESPACE
