#include <CtrlCore/CtrlCore.h>

#ifdef GUI_SKELETON

NAMESPACE_UPP

#define LLOG(x) // LOG(x)
#define LTIMING(x) // RTIMING(x)

/*Rect SystemDraw::GetVirtualScreenArea()
{
	GuiLock __;
}*/

dword SystemDraw::GetInfo() const
{
	return NATIVE;
}

Size SystemDraw::GetPageSize() const
{
	return Size(0, 0);
}

Size SystemDraw::GetNativeDpi() const
{
	return Size(96, 96);
}

void SystemDraw::BeginNative()
{
}

void SystemDraw::EndNative()
{
}

int SystemDraw::GetCloffLevel() const
{
	return 0;
}

SystemDraw::~SystemDraw() {
	GuiLock __;
}

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
