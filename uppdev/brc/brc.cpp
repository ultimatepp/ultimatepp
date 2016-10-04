#include <Core/Core.h>

#include "brc.brc"

CONSOLE_APP_MAIN
{
	String s = (char *)rodina_macro;
	DUMP(s.GetLength());
	DUMP(rodina_macro_length);
	DUMP(s);
}
