#include "CtrlPropTest.h"

void DoEdit(Ctrl& c)
{
	PropEdit& pe = Single<PropEdit>();
	if(pe.IsOpen()) return;

	Ctrl* owner = Ctrl::GetActiveWindow();
	if(!owner)
	{
		Exclamation("No Active Window");
		return;	
	}

	pe.Rejector(); pe.PopUp(owner, c);
}

void DoList(Ctrl& c)
{
	PropList& pe = Single<PropList>();
	if(pe.IsOpen()) return;

	Ctrl* owner = Ctrl::GetActiveWindow();
	if(!owner)
	{
		Exclamation("No Active Window");
		return;	
	}

	pe.Rejector(); pe.PopUp(owner, c);
}

bool CanEdit()
{
	PropEdit& pe = Single<PropEdit>();
	if(pe.IsOpen()) return false;
	PropList& pl = Single<PropList>();
	if(pl.IsOpen()) return false;
	return true;
}
