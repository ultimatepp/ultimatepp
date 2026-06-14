#include "CtrlCore.h"

#define LLOG(x)
#define LDUMP(x)

namespace Upp {

void Ctrl::VirtualPopUp(Ctrl *owner, bool activate)
{
	ASSERT(!IsOpen());
	TopWindow *win = owner->GetTopWindow();
	ASSERT(owner && win);
	Top *top = win->GetTop();
	ASSERT(top);
	
	top->virtual_popups.Add(this);

	Top *vtop = new Top;
	SetTop(vtop);
	vtop->virtual_popup = true;
	vtop->owner = owner;
	
	if(activate)
		SetFocus();

	DLOG("=================");
	RefreshFrame();
}

bool Ctrl::IsVirtualPopUp() const
{
	const Top *top = GetTop();
	return top && top->virtual_popup;
}

Rect Ctrl::GetVirtualPopUpRect(const Rect& vp_frame_rect) const
{ // converts frame rect to owner frame rect
	ASSERT(IsVirtualPopUp());
	Rect sr = GetOwner()->GetScreenRect();
	Rect r = vp_frame_rect.Offseted(GetScreenRect().TopLeft()) & GetScreenRect() & sr;
//	DLOG("+++ GetVirtualPopUpRect");
//	DDUMP(GetOwner()->GetScreenRect());
//	DDUMP(GetRect());
//	DDUMP(GetScreenRect());
	return Rect(r.TopLeft() - sr.TopLeft(), r.GetSize());
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