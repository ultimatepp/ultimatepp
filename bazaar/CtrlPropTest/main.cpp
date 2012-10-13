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

	hk.Multiselect(false);
	hk.WhenRightSelect = THISBACK(OnCtrlRight);
	hk.SetSource(&vis);
	hk.Disable();
	
	hk <<= THISBACK(OnMoveAction);

	InitDummies();
	OnEdit();
}
	
GUI_APP_MAIN
{
	CtrlPropTest().Run();
}
