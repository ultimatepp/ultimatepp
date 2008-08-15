#include <Core/Core.h>
#include <plugin/bz2/bz2.h>

using namespace Upp;

#include "ship.brc"

CONSOLE_APP_MAIN
{
	puts("Unpacking BRC.exe...");
	MemReadStream in(BRC, BRC_length);
	if(!SaveFile("BRC.exe", BZ2Decompress(in)))
		puts("Error unpacking file!");
}
