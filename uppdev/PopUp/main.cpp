#include "PopUp.h"

popup::popup()
{
	CtrlLayout(*this);
	CtrlLayout(dlg);
	Title("Popups");
	button.WhenPush = THISBACK(PopDlg);
	dlg.button <<= THISBACK(DlgClose);
	button.Tip("Tip!");
	dlg.button.Tip("Tip!");
	dlg.SetFrame(BlackFrame()); //comment this to stop changing size of dlg
}

void popup::PopDlg()
{
	if (dlg.IsOpen())
		return;
	Size sz = dlg.GetSize();
	Rect rc = button.GetScreenRect();
	dlg.SetRect(rc.right, rc.top, sz.cx, sz.cy);
	dlg.PopUp();
}

void popup::DlgClose()
{
	dlg.Close();
}

GUI_APP_MAIN
{
	popup().Run();
}
