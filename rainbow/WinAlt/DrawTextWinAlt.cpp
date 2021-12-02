#include <CtrlCore/CtrlCore.h>

#ifdef GUI_WINALT

NAMESPACE_UPP

#define LLOG(x)

HFONT GetWin32Font(Font fnt, int angle);

void SystemDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink,
                      int n, const int *dx) {
	Std(font);
	font.Bold();
	while(n > 30000) {
		DrawTextOp(x, y, angle, text, font, ink, 30000, dx);
		if(dx)
			for(int i = 0; i < 30000; i++)
				x += *dx++;
		else
			x += GetTextSize(text, font, 30000).cx;
		n -= 30000;
		text += 30000;
	}
	GuiLock __;
	COLORREF cr = GetColor(ink);
	if(cr != lastTextColor) {
		LLOG("Setting text color: " << ink);
		::SetTextColor(handle, lastTextColor = cr);
	}
	HGDIOBJ orgfont = ::SelectObject(handle, GetWin32Font(font, angle));
	int ascent = font.Info().GetAscent();
	if(angle) {
		double sina, cosa;
		Draw::SinCos(angle, sina, cosa);
		Size offset;
		::ExtTextOutW(handle, x + fround(ascent * sina), y + fround(ascent * cosa), 0, NULL,
		              ToSystemCharsetW(text), n, dx);
	}
	else
		::ExtTextOutW(handle, x, y + ascent, 0, NULL, ToSystemCharsetW(text),
		              n, dx);
	::SelectObject(handle, orgfont);
}

END_UPP_NAMESPACE

#endif
