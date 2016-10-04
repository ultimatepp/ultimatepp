#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
//	LabelBoxDlg().TopMost().Run();
//	Ctrl::SetXPStyle(false);
//	LabelBoxDlg().Run();


	TopWindow win;
	win.Sizeable().Zoomable();
	Label label;
	label.SetAlign(ALIGN_CENTER);
	win.Add(label);
	win.Add(label.SizePos());
	label.SetLabel("\1This is a very long line. hope it works");
	win.Run();
}
