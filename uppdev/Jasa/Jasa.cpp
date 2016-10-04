#include <Core/Core.h>

using namespace Upp;

String ReadString(const char *txt)
{
	Cout() << txt;
	return ReadStdIn();
}

int ReadInt(const char *txt = "")
{
	return StrInt(ReadString(txt));
}

double ReadDouble(const char *txt = "")
{
	return StrDbl(ReadString(txt));
}


CONSOLE_APP_MAIN
{
pocitani:
    int l = 1;
	int a = ReadInt("Zadej cislo: ");
    for(int i = 1; i <= a; i++) {
		 l=l*i;
		Cout() << l << '\n';
    }
	Cout() << l << '\n';
	goto pocitani;
}

/*
1*1 = 1
1*2 = 2
2*3 = 6
6*4 = 24
*/