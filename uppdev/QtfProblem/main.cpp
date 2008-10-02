#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow y;
	RichTextView view;
	view.SetQTF(LoadFile(GetDataFile("text.txt")));
	y.Add(view.SizePos());
	y.Run();
}
