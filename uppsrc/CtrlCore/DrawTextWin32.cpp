#include "CtrlCore.h"

#ifdef GUI_WIN

namespace Upp {

#define LLOG(x)

HFONT  GetWin32Font(Font fnt, int angle);
extern StaticMutex sFontLock;

void SystemDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink,
                      int n, const int *dx) {
	Std(font);
	GuiLock __;
	COLORREF cr = GetColor(ink);
	if(cr != lastTextColor) {
		LLOG("Setting text color: " << ink);
		::SetTextColor(handle, lastTextColor = cr);
	}
	Mutex::Lock ___(sFontLock); // need this because of GetWin32Font
	HGDIOBJ orgfont = ::SelectObject(handle, GetWin32Font(font, angle));
	int ascent = font.Info().GetAscent();
	if(angle) {
		double sina, cosa;
		Draw::SinCos(angle, sina, cosa);
		Size offset;
		::ExtTextOutW(handle, x + fround(ascent * sina), y + fround(ascent * cosa), 0, NULL, (const WCHAR *)text, n, dx);
	}
	else
		::ExtTextOutW(handle, x, y + ascent, 0, NULL, (const WCHAR *)text, n, dx);
	::SelectObject(handle, orgfont);
}

}

#endif
