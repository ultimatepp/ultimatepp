#ifndef _plugin_ndisasm_ndisasm_h_
#define _plugin_ndisasm_ndisasm_h_

NAMESPACE_UPP

static const int NDISASM_INSN_MAX = 32;     // one instruction can't be longer than this
static const int NDISASM_OUTBUF_SIZE = 256; // maximum length of disassembly output

#ifdef CPU_32
int NDisassemble(char *output, const byte *data, long offset); // returns instruction length
#else
int NDisassemble(char *output, const byte *data, int64 offset); // returns instruction length
#endif
END_UPP_NAMESPACE

#endif
