#include "PopUpText.h"

NAMESPACE_UPP

void PopUpInfo::Paint(Draw& w)
{
	Size sz = GetSize();
	if(!IsTransparent())
		w.DrawRect(0, 0, sz.cx, sz.cy, color);
	PaintLabel(w, 0, 0, sz.cx, sz.cy, !IsShowEnabled(), false, false, VisibleAccessKeys());
}

PopUpInfo::PopUpInfo(): color(SColorInfo())
{
	Transparent(false);
	NoWantFocus();
	IgnoreMouse();
	SetAlign(ALIGN_CENTER);
	SetFrame(BlackFrame());
	opened = false;
}

END_UPP_NAMESPACE