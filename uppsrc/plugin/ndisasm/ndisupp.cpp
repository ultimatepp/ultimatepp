#include <Core/Core.h>

#ifdef P_WAIT
#undef P_WAIT
#endif

#include "ndisasm.h"

#include <stdio.h>
#include <string.h>

extern "C" {
#undef INT64_MIN
#undef INT64_MAX
#undef UINT64_MAX

#include "lib/compiler.h"
#include "lib/disasm.h"
}

namespace Upp {

// NASM 3.x needs its ctype tables initialized once (nasm_tolower etc.);
// they are only ever read afterwards, so a single lazy init is enough.
static bool sNdisasmInit;

static void NDisasmInit()
{
	if(!sNdisasmInit) {
		nasm_ctype_init();
		reset_global_defaults(0);
		sNdisasmInit = true;
	}
}

int NDisassemble(char *output, const byte *data, uint64 offset, bool x64) // returns instruction length
{
	NDisasmInit();

	iflag_t prefer;
	iflag_clear_all(&prefer);

	int bits = x64 ? 64 : 32;
	// API contract (unchanged from 2.08): 'data' must point to at least
	// NDISASM_INSN_MAX (32) readable bytes.
	int len = disasm(data, NDISASM_INSN_MAX, output, NDISASM_OUTBUF_SIZE,
	                 bits, (int64_t)offset, false, &prefer);
	if(len <= 0) {
		eatbyte(data[0], output, NDISASM_OUTBUF_SIZE, bits);
		len = 1;
	}
	return len;
}

#ifdef flagMAIN
CONSOLE_APP_MAIN
{
	static const byte x64[] = {
		0x55,                               // push rbp
		0x48, 0x89, 0xE5,                   // mov rbp,rsp
		0x48, 0x83, 0xEC, 0x20,             // sub rsp,0x20
		0x62, 0xF1, 0x7C, 0x58, 0xA8, 0xEF, // EVEX-encoded instruction
		0xF3, 0x0F, 0x6F, 0xC1,             // movdqu xmm0,xmm1
	};
	static const byte x32[] = {
		0x55,                               // push ebp
		0x89, 0xE5,                         // mov ebp,esp
		0xB8, 0x01, 0x00, 0x00, 0x00,       // mov eax,0x1
		0x0F, 0xA2,                         // cpuid
	};

	byte blob[2 * NDISASM_INSN_MAX] = { 0 }; // satisfy the 32-readable-bytes contract
	extern const char nasm_version[];       // from ver.c - proves the new engine is linked
	printf("ndisasm engine: NASM %s\n", nasm_version);

	for(int m64 : { 0, 1 }) {
		size_t n = m64 ? sizeof(x64) : sizeof(x32);
		memcpy(blob, m64 ? x64 : x32, n);
		printf("--- %s ---\n", m64 ? "64-bit" : "32-bit");
		int64_t pc = 0x401000;
		for(;;) {
			char line[NDISASM_OUTBUF_SIZE];
			int len = NDisassemble(line, blob + (pc - 0x401000), pc, m64 != 0);
			printf("%08llX  %s\n", (unsigned long long)pc, line);
			pc += len;
			if(size_t(pc - 0x401000) >= n)
				break;
		}
	}
}
#endif

}