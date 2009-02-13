#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	DUMP(LoadFileBOMW("u:/unicode-le.txt"));
	DUMP(LoadFileBOMW("u:/unicode-be.txt"));
	DUMP(LoadFileBOMW("u:/unicode-utf8.txt"));
	DUMP(LoadFileBOMW("u:/ansi.txt"));
	DUMP(LoadFileBOM("u:/unicode-le.txt"));
	DUMP(LoadFileBOM("u:/unicode-be.txt"));
	DUMP(LoadFileBOM("u:/unicode-utf8.txt"));
	DUMP(LoadFileBOM("u:/ansi.txt"));
	
	SaveFileBOM("u:/bom.txt", WString("ěščřžýáíéúů"));
	SaveFileBOMUtf8("u:/bomutf8.txt", "ěščřžýáíéúů");
}
