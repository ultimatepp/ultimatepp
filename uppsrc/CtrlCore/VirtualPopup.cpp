#include "CtrlCore.h"

#define LLOG(x)
#define LDUMP(x)

namespace Upp {

Vector<Ptr<Ctrl>> Ctrl::virtual_popups;

Rect Ctrl::GetVirtualPopUpOverRect()
{
	Rect r = GetVirtualPopUpRect();
	const Top *top = GetTop();
	return top && top->virtual_dropshadow ? r.Inflated(DPI(20)) : r;
}

void Ctrl::RefreshVirtualPopUp()
{
	if(IsVirtualPopUp())
		GetOwner()->RefreshFrame(GetVirtualPopUpOverRect());
}

void Ctrl::VirtualPopUp(Ctrl *owner, bool activate, bool dropshadow)
{
	ASSERT(!IsOpen());
	TopWindow *win = owner->GetTopWindow();
	ASSERT(owner && win);

	Top *vtop = new Top;
	SetTop(vtop);
	vtop->owner = owner;
	vtop->virtual_dropshadow = dropshadow;
	
	virtual_popups << this;
	
	if(activate)
		SetFocus();
	
	popup = true;

	RefreshVirtualPopUp();
	
}

bool Ctrl::IsVirtualPopUp() const
{
	const Top *top = GetTop();
	if(top && top->owner)
		for(Ctrl *q : virtual_popups)
			if(q == this)
				return true;
	return false;
}

Rect Ctrl::GetVirtualPopUpRect(const Rect& vp_frame_rect) const
{ // converts frame rect to owner frame rect
	ASSERT(IsVirtualPopUp());
	Rect sr = GetTopWindow()->GetScreenRect();
	Rect r = vp_frame_rect.Offseted(GetScreenRect().TopLeft()) & GetScreenRect() & sr;
//	DLOG("+++ GetVirtualPopUpRect");
//	DDUMP(GetOwner()->GetScreenRect());
//	DDUMP(GetRect());
//	DDUMP(GetScreenRect());
	return Rect(r.TopLeft() - sr.TopLeft(), r.GetSize());
}

Rect Ctrl::GetVirtualPopUpRect() const
{
	return GetVirtualPopUpRect(GetScreenRect().GetSize());
}

void Ctrl::CloseVirtualPopUp()
{
	ASSERT(IsVirtualPopUp());
	RefreshVirtualPopUp();
	popup = false;
	virtual_popups.RemoveIf([&](int i) {
		return virtual_popups[i] == this || !virtual_popups[i];
	});
	DeleteTop();
}

bool Ctrl::use_virtual_popups;

};