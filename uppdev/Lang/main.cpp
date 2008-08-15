#include "CtrlLib/CtrlLib.h"

using namespace Upp;

#define   TFILE <Lang/test.t>
#include  <Core/t.h>

GUI_APP_MAIN
{
	DUMP(LNGFromText(String()));
	DUMP(LNGAsText(GetSystemLNG()));
}
