#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	RichText txt = ParseQTF(LoadDataFile("Sample.qtf"));
	String out = GetHomeDirFile("upp_test.rtf");
	SaveFile(out, EncodeRTF(txt));
	LaunchWebBrowser(out);
}
