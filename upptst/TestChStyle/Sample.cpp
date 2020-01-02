#include "TestChStyle.h"

void Sample::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White());
	
	w.DrawImage(10, 10, CtrlImg::spinup());
//	ChPaint(w, 10, 10, 100, 20, CtrlsImg::EFE());
//	ChPaint(w, 101, 0, 17, 23, DropList::StyleDefault().rmiddle[0]);
	ChPaint(w, 101, 0, 17, 23, DropList::StyleDefault().rmiddle[0]);

//	EditFieldFrame().FramePaint(w, RectC(10, 50, 100, 20));
}
