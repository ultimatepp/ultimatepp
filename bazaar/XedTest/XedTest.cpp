#include "XedTest.h"

CONSOLE_APP_MAIN
{
	uint8_t buf[15] = {
		0xf2, 0x2e, 0x4f, 0x0F, 0x85, 0x99, 0x00, 0x00, 0x00
	};

	Cout() << "Length = " << XED.InstructionLength(buf) << "\n";
}
