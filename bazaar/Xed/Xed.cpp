#include "Xed.h"

#ifndef XED_ENCODER
	#define XED_ENCODER
#endif

#ifndef XED_DECODER
	#define XED_DECODER
#endif

extern "C" {

#include "xed/include/public/xed/xed-agen.h"
#include "xed/include/public/xed/xed-attributes.h"
#include "xed/include/public/xed/xed-chip-features.h"
#include "xed/include/public/xed/xed-common-defs.h"
#include "xed/include/public/xed/xed-common-hdrs.h"
#include "xed/include/public/xed/xed-cpuid-rec.h"
#include "xed/include/public/xed/xed-decoded-inst-api.h"
#include "xed/include/public/xed/xed-decoded-inst.h"
#include "xed/include/public/xed/xed-decode.h"
#include "xed/include/public/xed/xed-disas.h"
#include "xed/include/public/xed/xed-encode.h"
#include "xed/include/public/xed/xed-encoder-hl.h"
#include "xed/include/public/xed/xed-flags.h"
#include "xed/include/public/xed/xed-format-options.h"
#include "xed/include/public/xed/xed-get-time.h"
#include "xed/include/public/xed/xed-iform-map.h"
#include "xed/include/public/xed/xed-ild.h"
#include "xed/include/public/xed/xed-immdis.h"
#include "xed/include/public/xed/xed-immed.h"
#include "xed/include/public/xed/xed-init.h"
#include "xed/include/public/xed/xed-inst.h"
#include "xed/include/public/xed/xed-interface.h"
#include "xed/include/public/xed/xed-isa-set.h"
#include "xed/include/public/xed/xed-operand-action.h"
#include "xed/include/public/xed/xed-operand-values-interface.h"
#include "xed/include/public/xed/xed-patch.h"
#include "xed/include/public/xed/xed-portability.h"
#include "xed/include/public/xed/xed-print-info.h"
#include "xed/include/public/xed/xed-reg-class.h"
#include "xed/include/public/xed/xed-rep-prefix.h"
#include "xed/include/public/xed/xed-state.h"
#include "xed/include/public/xed/xed-types.h"
#include "xed/include/public/xed/xed-util.h"
#include "xed/include/public/xed/xed-version.h"

};

using namespace Upp;

// constructor
XEDClass::XEDClass()
{
	// one time per process
	xed_tables_init();
	
	// set default mode (32 or 64 bit)
	// depending on this machine
#ifdef CPU_64
	_64bitMode = true;
#else
	_64bitMode = false;
#endif

}

// destructor
XEDClass::~XEDClass()
{
}

// set/get 64 bit mode
void XEDClass::Set64bitMode(void)
{
	_64bitMode = true;
}

void XEDClass::Set32bitMode(void)
{
	_64bitMode = false;
}

bool XEDClass::Get64bitMode(void) const
{
	return 	_64bitMode;
}

bool XEDClass::Get32bitMode(void) const
{
	return 	!_64bitMode;
}

// get instruction length from pointer
int XEDClass::InstructionLength(void const *buf)
{
	xed_decoded_inst_t xedd;

	xed_decoded_inst_zero(&xedd);
	if(_64bitMode)
		xed_decoded_inst_set_mode(&xedd, XED_MACHINE_MODE_LONG_64, XED_ADDRESS_WIDTH_64b);
	else
		xed_decoded_inst_set_mode(&xedd, XED_MACHINE_MODE_LEGACY_32,  XED_ADDRESS_WIDTH_32b);
	xed_ild_decode(&xedd, (uint8_t const *)buf, XED_MAX_INSTRUCTION_BYTES);
	
	return xed_decoded_inst_get_length(&xedd);
}


// disassemble instruction at pointer
String XEDClass::DisassembleInstruction(void const *buf, bool bytes)
{
	char disasmBuf[150];
	memset(disasmBuf, 0, 150);
	
	xed_decoded_inst_t xedd;

	xed_decoded_inst_zero(&xedd);
	if(_64bitMode)
		xed_decoded_inst_set_mode(&xedd, XED_MACHINE_MODE_LONG_64, XED_ADDRESS_WIDTH_64b);
	else
		xed_decoded_inst_set_mode(&xedd, XED_MACHINE_MODE_LEGACY_32,  XED_ADDRESS_WIDTH_32b);
	xed_decode(&xedd, (uint8_t const *)buf, XED_MAX_INSTRUCTION_BYTES);
	
	// add hex dump in front, if requested
	char *bufP = disasmBuf;
	if(bytes)
	{
		int len = xed_decoded_inst_get_length(&xedd);
		int i = 0;
		while(i < len)
		{
			sprintf(bufP, "%02x ", ((uint8_t const *)buf)[i]);
			bufP += 3;
			i++;
		}
		while(i < 15)
		{
			sprintf(bufP, "   ");
			i++;
			bufP += 3;
		}
	}
	
	if(!xed_format_context(XED_SYNTAX_INTEL, &xedd, bufP, 100, 0, 0, 0))
		return "<CAN'T DISASSEMBLE>";
	return disasmBuf;
}

// dump all instruction info
String XEDClass::DumpInstruction(void const *buf)
{
	char disasmBuf[100];

	xed_decoded_inst_t xedd;

	xed_decoded_inst_zero(&xedd);
	if(_64bitMode)
		xed_decoded_inst_set_mode(&xedd, XED_MACHINE_MODE_LONG_64, XED_ADDRESS_WIDTH_64b);
	else
		xed_decoded_inst_set_mode(&xedd, XED_MACHINE_MODE_LEGACY_32,  XED_ADDRESS_WIDTH_32b);
	xed_decode(&xedd, (uint8_t const *)buf, XED_MAX_INSTRUCTION_BYTES);
	
	xed_decoded_inst_dump(&xedd, disasmBuf, 100);
	return disasmBuf;
}


XEDClass &__getXED(void)
{
	static XEDClass x;
	return x;
}
