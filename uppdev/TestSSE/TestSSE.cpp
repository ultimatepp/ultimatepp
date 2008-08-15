#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	unsigned int cpeinfo;
	unsigned int cpsse3;
	__asm {
		mov eax, 01h       ;01h is the parameter for the CPUID command below
		cpuid
		mov cpeinfo, edx   ;Get the info stored by CPUID
		mov cpsse3, ecx    ;info about SSE3 is stored in ECX
	}

	Cout() << "1 - Instruction set is supported by CPU\n";
	Cout() << "0 - Instruction set not supported\n"; 
	Cout() << "--------------------------------\n";
	Cout() << "MMX:  " << ((cpeinfo >> 23) & 0x1 ) << "\tSSE:  " << ((cpeinfo >> 25) & 0x1 ) << "\tSSE2: " << ((cpeinfo >> 26) & 0x1 ) << "\n"; 
	Cout() << "SSE3: " << ((cpsse3       ) & 0x1 ); 
}

