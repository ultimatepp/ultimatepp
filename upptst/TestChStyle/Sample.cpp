#include "TestChStyle.h"

void Sample::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White());
	
	ChPaint(w, 10, 10, 100, 20, CtrlsImg::EFE());
	ChPaint(w, 10, 30, 100, 20, GetChameleonSample());
	
	EditFieldFrame().FramePaint(w, RectC(10, 50, 100, 20));
}
