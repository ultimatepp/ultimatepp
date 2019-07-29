#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
//	StdLogSetup(LOG_COUT|LOG_FILE);
	const LanguageInfo& f = GetLanguageInfo();
	DUMP(AsString(f));
}
