#include "vfkstrm.h"

void TryMain()
{
	VectorMap<int, String> fgroups;
	fgroups.Add(0, "L:\\vfk\\Pardubice8bloku\\Export.vfk");
	fgroups.Add(0, "L:\\vfk\\Pardubicembpej\\Export.vfk");
	fgroups.Add(1, "L:\\vfk\\UstinadOrlici.vfk");
	VfkStream vfk;
	vfk.Open(fgroups);
	vfk.Dump();
}

CONSOLE_APP_MAIN
{
	try {
		TryMain();
	}
	catch(Exc e) {
		puts(e);
		SetExitCode(1);
	}
}
