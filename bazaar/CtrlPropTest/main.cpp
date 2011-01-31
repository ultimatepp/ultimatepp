#include "CtrlPropTest.h"

void CtrlPropTest::OnEdit()
{
	if(edit)
	{
		edit = false;
		mbi->Check(edit);
		hk.Disable();
	}
	else
	{
		edit = true;
		mbi->Check(edit);
		hk.Enable();
		SetFocus();
	}
}

CtrlPropTest::CtrlPropTest()
{
	CtrlLayout(vis);
	SetRect(vis.GetRect());
	Add(vis.SizePos());
	
	edit = false;
	mbi = &mb.Add(!IsReadOnly(), t_("Edit"), CtrlImg::selection(),
	        THISBACK(OnEdit));
	AddFrame(mb);

	hk.WhenRightDown = THISBACK(OnCtrlRight);
	hk.Visit(vis);
	hk.Disable();

	InitDummies();
}
	
GUI_APP_MAIN
{
	CtrlPropTest().Run();
}
