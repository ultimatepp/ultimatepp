#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String out;
	Sys("c:/xxx/pdftoppm -r 100 \"C:/xxx/smlouva.pdf\" \"C:/xxx/out.ppm\"");
	DDUMP(out);
}
