#include "CtrlCore.h"

#define LLOG(x)
#define LDUMP(x)

namespace Upp {

Vector<Ptr<Ctrl>> Ctrl::virtual_popups;

void Ctrl::VirtualPopUp(Ctrl *owner, bool activate)
{
	ASSERT(!IsOpen());
	TopWindow *win = owner->GetTopWindow();
	ASSERT(owner && win);

	Top *vtop = new Top;
	SetTop(vtop);
	vtop->owner = owner;
	
	virtual_popups << this;
	
	if(activate)
		SetFocus();

	DLOG("=================");
	RefreshFrame();
}

bool Ctrl::IsVirtualPopUp() const
{
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
	return GetVirtualPopUpRect(GetRect().GetSize());
}

void Ctrl::CloseVirtualPopUp()
{
	ASSERT(IsVirtualPopUp());
	Ctrl *owner = GetOwner();
	ASSERT(owner);
	TopWindow *win = owner->GetTopWindow();
	Refresh();
	virtual_popups.RemoveIf([&](int i) {
		return virtual_popups[i] == this || !virtual_popups[i];
	});
	DeleteTop();
}

};