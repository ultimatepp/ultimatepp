#include <Core/Core.h>



CONSOLE_APP_MAIN
{
	int x = 5555;
	int *y = new (&x) int();
	DUMP(*y);
}
