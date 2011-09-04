#include "CtrlPropTest.h"

void CtrlPropTest::DoEdit(Ctrl& c)
{
	if(pe.IsOpen()) return;

	Ctrl* owner = Ctrl::GetActiveWindow();
	if(!owner)
	{
		Exclamation("No Active Window");
		return;	
	}

	pe.pec.SetCtrl(&c);
	pe.Backup();
	pe.PopUp(owner);
}

void CtrlPropTest::DoList(Ctrl& c)
{
	if(pl.IsOpen()) return;

	Ctrl* owner = Ctrl::GetActiveWindow();
	if(!owner)
	{
		Exclamation("No Active Window");
		return;	
	}

	pl.PopUp(owner, c);
}

bool CtrlPropTest::CanEdit()
{
	if(pe.IsOpen() || pl.IsOpen()) return false;
	return true;
}

void CtrlPropTest::OnCtrlRight(Ctrl& c, Point p, dword keyflags)
{
	if(&c == &hk) return;
	if(!CanEdit()) return;
	if(keyflags & K_SHIFT)
		DoList(c);
	else
		DoEdit(c);
}

void CtrlPropTest::OnMoveAction()
{
	if(hk.GetCtrl() && hk.GetCtrl() == pe.pec.GetCtrl())
		pe.pec.Update();
}
