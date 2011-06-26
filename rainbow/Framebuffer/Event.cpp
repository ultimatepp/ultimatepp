#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

#define LLOG(x) LOG(x)

static Point fbmousepos;

Point GetMousePos() {
	return fbmousepos;
}

void Ctrl::DoMouseFB(int event, Point p, int zdelta)
{
	fbmousepos = p;
	int a = event & Ctrl::ACTION;
//	if(a == DOWN)
//		ClickActivateWnd();
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
//	if(a == Ctrl::MOUSEMOVE)
//		DoCursorShape();
}

bool Ctrl::DoKeyFB(dword key, int cnt)
{
	bool b = DispatchKey(key, cnt);
	SyncCaret();
	Ctrl *desktop = GetDesktop();
	if(desktop)
		desktop->PostInput();
	return b;
}

END_UPP_NAMESPACE
