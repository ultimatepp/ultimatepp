#include <CtrlLib/CtrlLib.h>

using namespace Upp;

String FileName() { return GetExeDirFile("test.txt"); }

GUI_APP_MAIN
{
	SetDefaultCharset(CHARSET_WIN1250);
	TopWindow w;
	w.Sizeable().Zoomable().SetRect(0, 0, 100, 100);
	DocEdit edit;
	edit <<= LoadFile(FileName());
	w.Add(edit.SizePos());
	w.Run();
	SaveFile(FileName(), ~edit);
}
