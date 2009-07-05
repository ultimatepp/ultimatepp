#include "Draw.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

#define LLOG(x)

HFONT GetWin32Font(Font fnt, int angle);

void SystemDraw::SetFont(Font font, int angle) {
	DrawLock __;
	LLOG("Set font: " << font << " face: " << font.GetFaceName());
	if(lastFont == font && lastAngle == angle)
		return;
	lastFont = font;
	lastAngle = angle;
	HFONT h = (HFONT) SelectObject(handle, GetWin32Font(font, angle));
	if(!orgFont) orgFont = h;
}

void SystemDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink,
                      int n, const int *dx) {
	Std(font);
	while(n > 30000) {
		DrawTextOp(x, y, angle, text, font, ink, 30000, dx);
		if(dx) {
			for(int i = 0; i < 30000; i++)
				x += *dx++;
		}
		else
			x += GetTextSize(text, font, 30000).cx;
		n -= 30000;
		text += 30000;
	}
	DrawLock __;
	COLORREF cr = GetColor(ink);
	if(cr != lastTextColor) {
		LLOG("Setting text color: " << ink);
		::SetTextColor(handle, lastTextColor = cr);
	}
	SetFont(font, angle);
	int ascent = font.Info().GetAscent();
	if(angle) {
		double sina, cosa;
		Draw::SinCos(angle, sina, cosa);
		Size offset;
		::ExtTextOutW(handle, x + fround(ascent * sina), y + fround(ascent * cosa), 0, NULL, (const WCHAR *)text, n, dx);
	}
	else
		::ExtTextOutW(handle, x, y + ascent, 0, NULL, (const WCHAR *)text,
		              n, dx);
}

#endif

END_UPP_NAMESPACE
