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
		hk.Enable();
		mbi->Check(true);
		SetFocus(); //kill foucus from all others
	}
}

CtrlPropTest::CtrlPropTest()
{
	CtrlLayout(*this, "Window title");

	mbi = &mb.Add(!IsReadOnly(), t_("Edit"), CtrlImg::selection(),
	        THISBACK(EditCB));
	AddFrame(mb);

	hk.WhenRightDown = THISBACK(OnCtrlRight);
	hk.Disable();
	Add(hk.SizePos());

	InitDummies();
}
	
GUI_APP_MAIN
{
	CtrlPropTest().Run();
}
