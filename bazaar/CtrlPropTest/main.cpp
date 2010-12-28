#include "CtrlPropTest.h"

void CtrlPropTest::EditCB()
{
	if(hk.IsEnabled())
	{
		hk.Disable();
		mbi->Check(false);
	}
	else
	{
		SetFocus(); //kill foucus from all others
		hk.Enable();
		rc.Remove();
		mbi->Check(true);
	}
}

CtrlPropTest::CtrlPropTest()
{
	CtrlLayout(*this, "Window title");

	mbi = &mb.Add(!IsReadOnly(), t_("Edit"), CtrlImg::selection(),
	        THISBACK(EditCB));
	AddFrame(mb);

	InstallMouseHook(&MyMouseHook);

	hk.WhenLeftDown = THISBACK(OnCtrlLeft);
	hk.WhenRightDown = THISBACK(OnCtrlRight);
	hk.Disable();
	Add(hk.SizePos());

	rcst = RectCtrl::StyleDefault();	
	rcst.rectcol = Null;
	rc.SetStyle(rcst);
	rc <<= THISBACK(OnRectChange);
	rc.WhenMissed = THISBACK(OnMissed);

	InitDummies();
}
	
GUI_APP_MAIN
{
	CtrlPropTest().Run();
}
