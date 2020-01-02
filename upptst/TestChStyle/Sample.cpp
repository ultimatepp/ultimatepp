#include "TestChStyle.h"

void Sample::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White());
	
//	w.DrawImage(10, 10, CtrlImg::spinup());
//	ChPaint(w, 10, 10, 100, 20, CtrlsImg::EFE());
//	ChPaint(w, 101, 0, 17, 23, DropList::StyleDefault().rmiddle[0]);
	w.DrawImage(10, 10, (Image)GetChameleonSample());
	w.DrawImage(50, 10, Magnify((Image)GetChameleonSample(), 4, 4));
//	ChPaint(w, 10, 10, 32, 32, GetChameleonSample());

//	EditFieldFrame().FramePaint(w, RectC(10, 50, 100, 20));
}
