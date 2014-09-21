#include "coff.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

#define MACHINE(mach) { ASSTRING(mach), COMBINE(COFF_IMAGE_FILE_MACHINE_, mach) },

static const MachineInfo _COFFMachineList[] =
{
	MACHINE(I386)
	MACHINE(R3000)
	MACHINE(R4000)
	MACHINE(R10000)
	MACHINE(WCEMIPSV2)
	MACHINE(ALPHA)
	MACHINE(POWERPC)
	MACHINE(SH3)
	MACHINE(SH3E)
	MACHINE(SH4)
	MACHINE(ARM)
	MACHINE(THUMB)
	MACHINE(IA64)
	MACHINE(MIPS16)
	MACHINE(MIPSFPU)
	MACHINE(MIPSFPU16)
	MACHINE(ALPHA64)
	MACHINE(AXP64)
	{ NULL, -1 }
};

const MachineInfo *COFFMachineList() { return _COFFMachineList; }

String COFFMachineNames()
{
	String out;
	for(const MachineInfo *mlist = COFFMachineList(), *mi = mlist; mi->name; mi++)
		out << (mi == mlist ? "" : ", ") << mi->name;
	return out;
}

String COFFMachineName(int code)
{
	for(const MachineInfo *mi = COFFMachineList(); mi->name; mi++)
		if(mi->code == code)
			return mi->name;
	return NFormat("UNKNOWN(%04x)", code);
}

int COFFMachineCode(String name)
{
	for(const MachineInfo *mi = COFFMachineList(); mi->name; mi++)
		if(mi->name == name)
			return mi->code;
	return COFF_IMAGE_FILE_MACHINE_UNKNOWN;
}

String COFFSymbolName(const COFF_IMAGE_SYMBOL& sym, const char *strtbl)
{
	String name;
	if(sym.N.Name.Short)
		name = MaxLenString(sym.N.ShortName, 8);
	else
		name = strtbl + sym.N.Name.Long;
	return name;
}

#endif

END_UPP_NAMESPACE
