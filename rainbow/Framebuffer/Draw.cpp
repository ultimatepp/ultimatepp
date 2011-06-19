#include <CtrlCore/CtrlCore.h>

#ifdef GUI_FB

NAMESPACE_UPP

#define LLOG(x) // LOG(x)
#define LTIMING(x) // RTIMING(x)

/*Rect SystemDraw::GetVirtualScreenArea()
{
	GuiLock __;
}*/

void BackDraw::Destroy()
{
	GuiLock __;
}

void BackDraw::Create(SystemDraw& w, int cx, int cy) {
	GuiLock __;
}

void BackDraw::Put(SystemDraw& w, int x, int y) {
	GuiLock __;
}

END_UPP_NAMESPACE

#endif
