#include <Core/Core.h>

#include "simplebin.brc"

CONSOLE_APP_MAIN
{
	puts(NFormat("source (%d) = %s...%s", source_length, String(source, 20), String(source + source_length - 20, 20)));
	puts(NFormat("binsrc (%d) = %s...%s", binsrc_length, String(binsrc, 20), String(binsrc + binsrc_length - 20, 20)));
	puts(NFormat("array_count = %d", array_count));
	for(int i = 0; i < array_count; i++)
		puts(NFormat("array[%d] (%d) = %s...%s", i, array_length[i], String(array[i], 20), String(array[i] + array_length[i] - 20, 20)));
}
