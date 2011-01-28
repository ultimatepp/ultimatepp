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

	pe.PopUp(owner, c);
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

