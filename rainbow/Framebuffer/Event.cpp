#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

void Ctrl::DoMouseFB(int event, Point p, int zdelta)
{
	int a = event & Ctrl::ACTION;
//	if(a == DOWN)
//		ClickActivateWnd();
	if(a == Ctrl::UP && Ctrl::ignoreclick)
		EndIgnore();
	else
	if(a != Ctrl::DOWN || !ignoreclick)
		LOG("Mouse " << p << ' ' << zdelta);
//	if(_this) PostInput();
//	if(a == Ctrl::MOUSEMOVE)
//		DoCursorShape();
}

END_UPP_NAMESPACE
