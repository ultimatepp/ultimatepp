#include <CtrlCore/CtrlCore.h>

#ifdef GUI_SKELETON

NAMESPACE_UPP

#define LLOG(x)

void SystemDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink,
                      int n, const int *dx) {
	Std(font);
}

END_UPP_NAMESPACE

#endif
