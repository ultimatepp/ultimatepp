#include "CtrlCore.h"

namespace Upp {

void Ctrl::VirtualPopUp(Ctrl *owner)
{
	ASSERT(!IsOpen());
	TopWindow *win = owner->GetTopWindow();
	Top *top = win->GetTop();
	if(!win || !top) {
//		Popup(owner); // Fix this for splash screens
		return;
	}
	
	top->virtual_popups.Add(this);
	Refresh();
	_DBG_ win->Refresh();


	Top *vtop = new Top;
	SetTop(vtop);
	vtop->virtual_popup = true;
	vtop->owner = owner;
}

bool Ctrl::IsVirtualPopUp() const
{
	const Top *top = GetTop();
	return top && top->virtual_popup;
}

void Ctrl::CloseVirtualPopUp()
{
	ASSERT(IsVirtualPopUp());
	Ctrl *owner = GetOwner();
	ASSERT(owner);
	TopWindow *win = owner->GetTopWindow();
	_DBG_ win->Refresh();
	Refresh();
	Top *top = win->GetTop();
	if(top) {
		int q = FindIndex(top->virtual_popups, this);
		ASSERT(q >= 0);
		top->virtual_popups.Remove(q);
	}
	DeleteTop();
}

};