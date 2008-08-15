#include "sstrm.h"

CONSOLE_APP_MAIN
{
	StringStream ss;
	ss.Put('*');
	ss.Put("ahoj kamarade jak se mas");
	ss.Seek(0);
	DUMP(ss.GetLine());
	LOGHEXDUMP(ss.GetResult(), ss.GetResult().GetCount());
	ss.Seek(0);
	DUMP(ss.GetLine());
	ss.Put("Another one");
	LOGHEXDUMP(ss.GetResult(), ss.GetResult().GetCount());
}