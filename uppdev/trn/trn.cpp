#include <Core/Core.h>

using namespace Upp;

int Trn(int x)
{
	if(x % 2)
		return x + 1;
	else
		return x;
}


CONSOLE_APP_MAIN
{
	for(;;) {
		Cout() << "Zadej cislo x: ";
		int cislo = atoi(ReadStdIn());
		Cout() << "Trn(" << cislo << ") = " << Trn(cislo) << "\n";
	}
}
