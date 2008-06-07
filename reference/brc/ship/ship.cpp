#include <Core/Core.h>
#include <plugin/bz2/bz2.h>

using namespace Upp;

#include "ship.brc"

CONSOLE_APP_MAIN
{
	puts("Unpacking BRC.exe...");
	if(!SaveFile("BRC.exe", BZ2Decompress(MemReadStream(BRC, BRC_length))))
		puts("Error unpacking file!");
}
