#ifndef _plugin_ndisasm_ndisasm_h_
#define _plugin_ndisasm_ndisasm_h_

namespace Upp {

static const int NDISASM_INSN_MAX = 32;     // one instruction can't be longer than this
static const int NDISASM_OUTBUF_SIZE = 256; // maximum length of disassembly output

int NDisassemble(char *output, const byte *data, uint64 offset, bool x64 = false); // returns instruction length

}

#endif
