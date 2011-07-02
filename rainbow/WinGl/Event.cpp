#include <CtrlCore/CtrlCore.h>

#ifdef GUI_WINGL

NAMESPACE_UPP

#define LLOG(x) LOG(x)

static Point glmousepos;

Point GetMousePos() {
	return glmousepos;
}

void Ctrl::DoMouseGl(int event, Point p, int zdelta)
{
	glmousepos = p;
	int a = event & Ctrl::ACTION;
	if(a == Ctrl::UP && Ctrl::ignoreclick) {
		EndIgnore();
		return;
	}
	else
	if(a == Ctrl::DOWN && ignoreclick)
		return;
	LLOG("Mouse event: " << event << " position " << p << " zdelta " << zdelta);
	Ctrl *desktop = GetDesktop();
	if(desktop) {
		desktop->DispatchMouse(event, p, zdelta);
		desktop->PostInput();
	}
}

bool Ctrl::DoKeyGl(dword key, int cnt)
{
	bool b = DispatchKey(key, cnt);
	SyncCaret();
	Ctrl *desktop = GetDesktop();
	if(desktop)
		desktop->PostInput();
	return b;
}

Point fbCursorPos = Null;
Image fbCursorImage;

Point fbCursorBakPos = Null;
Image fbCursorBak;

Rect  fbCaretRect;
Image fbCaretBak;
int   fbCaretTm;

void Ctrl::RemoveCursor()
{
}

void Ctrl::RemoveCaret()
{
}

void Ctrl::SetCaret(int x, int y, int cx, int cy)
{
	GuiLock __;
	caretx = x;
	carety = y;
	caretcx = cx;
	caretcy = cy;
	fbCaretTm = GetTickCount();
	SyncCaret();
}

void Ctrl::SyncCaret() {
	GuiLock __;
}

void Ctrl::CursorSync()
{
}

void  Ctrl::SetMouseCursor(const Image& image)
{
	GuiLock __;
	if(image.GetSerialId() != fbCursorImage.GetSerialId()) {
		fbCursorImage = image;
		fbCursorPos = Null;
	}
}

END_UPP_NAMESPACE

#endif