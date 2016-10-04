#include <CtrlLib/CtrlLib.h>

#include <uxtheme.h>
#include <tmschema.h>

using namespace Upp;

#define DEF(x)    x, #x,

struct {
	int color;
	const char *text;
}
syscolor[] = {
	DEF(COLOR_3DDKSHADOW)
	DEF(COLOR_3DFACE)
	DEF(COLOR_BTNFACE)
	DEF(COLOR_3DHILIGHT)
	DEF(COLOR_3DHIGHLIGHT)
	DEF(COLOR_BTNHILIGHT)
	DEF(COLOR_BTNHIGHLIGHT)
	DEF(COLOR_3DLIGHT)
	DEF(COLOR_3DSHADOW)
	DEF(COLOR_BTNSHADOW)
	DEF(COLOR_ACTIVEBORDER)
	DEF(COLOR_ACTIVECAPTION)
	DEF(COLOR_APPWORKSPACE)
	DEF(COLOR_BACKGROUND)
	DEF(COLOR_BTNTEXT)
	DEF(COLOR_CAPTIONTEXT)
	DEF(COLOR_GRADIENTACTIVECAPTION)
	DEF(COLOR_ACTIVECAPTION)
	DEF(COLOR_GRADIENTINACTIVECAPTION)
	DEF(COLOR_INACTIVECAPTION)
	DEF(COLOR_GRAYTEXT)
	DEF(COLOR_HIGHLIGHT)
	DEF(COLOR_HIGHLIGHTTEXT)
	DEF(COLOR_HOTLIGHT)
	DEF(COLOR_INACTIVEBORDER)
	DEF(COLOR_INACTIVECAPTION)
	DEF(COLOR_INACTIVECAPTIONTEXT)
	DEF(COLOR_INFOBK)
	DEF(COLOR_INFOTEXT)
	DEF(COLOR_MENU)
//	DEF(COLOR_MENUHILIGHT)
//	DEF(COLOR_MENUBAR)
	DEF(COLOR_MENUTEXT)
	DEF(COLOR_SCROLLBAR)
	DEF(COLOR_WINDOW)
	DEF(COLOR_WINDOWFRAME)
	DEF(COLOR_WINDOWTEXT)
	0, NULL
};

struct Visual : public TopWindow {
	virtual void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(sz, LtGray);
//		w.DrawRect(sz, SWhite);
	/*
		int tcy = Draw::GetStdFontCy();
		int yc = sz.cy / tcy;
		for(int i = 0; syscolor[i].text; i++) {
			int x = i / yc * 128;
			int y = i % yc * tcy;
			w.DrawRect(x, y, 128, tcy, Color::FromCR(GetSysColor(syscolor[i].color)));
			w.DrawText(x, y, syscolor[i].text, StdFont(), SWhite);
			w.DrawText(x + 1, y - 1, syscolor[i].text, StdFont(), SBlack);
		}
	*/
	//	HTHEME theme = OpenThemeData(GetHWND(), WString("COMBOBOX"));
		HTHEME theme = OpenThemeData(GetHWND(), WString("MENU"));
		wchar h[5000];
		GetThemeSysString(theme, TMT_XMLNAME, h, 1000);
		DUMP(WString(h));
	//	HTHEME theme = OpenThemeData(GetHWND(), WString("PROGRESS"));
	//	HTHEME theme = OpenThemeData(GetHWND(), WString("EDIT"));
		for(int x = 0; x < 20; x++)
			for(int y = 0; y < 5; y++) {
				LOGFONTW f;
//				GetThemeFont(theme, NULL, x, y, 809 /*TMT_BODYFONT*/, (LOGFONT *)&f);
//				DUMP(f.lfHeight);
//				DUMP(f.lfFaceName);
				Rect r = RectC(x * 45, y * 52, 40, 50);
				HDC hdc = w.BeginGdi();
				DrawThemeBackground(theme, hdc, x, y, r, NULL);
				DrawThemeBackground(theme, hdc, x, y, r.OffsetedVert(6 * 52), NULL);
				w.EndGdi();
				Rect rr;
				GetThemeBackgroundContentRect(theme, NULL, x, y, r.OffsetedVert(6 * 52), rr);
				DUMP(rr);
				DrawFrame(w, rr, LtRed);
			}
/*		Size csz;
		GetThemePartSize(theme, hdc, BP_CHECKBOX, CBS_CHECKEDHOT,
		                 NULL, TS_MIN, csz);
		DUMP(csz);
		GetThemePartSize(theme, hdc, BP_CHECKBOX, CBS_CHECKEDHOT,
		                 NULL, TS_TRUE, csz);
		DUMP(csz);
		GetThemePartSize(theme, hdc, BP_CHECKBOX, CBS_CHECKEDHOT,
		                 NULL, TS_DRAW, csz);
		DUMP(csz);*/
		CloseThemeData(theme);
	}
};

String GetThemeInfo(LPCWSTR pszPropertyName)
{
	wchar theme[512], colors[512], size[512];
	GetCurrentThemeName(theme, 512, colors, 512, size, 512);
	wchar h[1024];
	GetThemeDocumentationProperty(theme, pszPropertyName, h, 1000);
	return FromUnicode(h);
}

GUI_APP_MAIN
{
	wchar theme[512], colors[512], size[512];
	GetCurrentThemeName(theme, 512, colors, 512, size, 512);
	DUMP(FromUnicode(theme));
	DUMP(FromUnicode(colors));
	DUMP(FromUnicode(size));

	DUMP(GetThemeInfo(SZ_THDOCPROP_DISPLAYNAME));
	DUMP(GetThemeInfo(SZ_THDOCPROP_TOOLTIP));
	DUMP(GetThemeInfo(SZ_THDOCPROP_AUTHOR));
	DUMP(GetThemeInfo(SZ_THDOCPROP_CANONICALNAME));

	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(ncm);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
	DUMP(ncm.lfMenuFont.lfHeight);
	DUMP(ncm.lfMenuFont.lfFaceName);
	DUMP(ncm.lfStatusFont.lfHeight);
	DUMP(ncm.lfStatusFont.lfFaceName);
	DUMP(ncm.lfMessageFont.lfHeight);
	DUMP(ncm.lfMessageFont.lfFaceName);

	HFONT hfont = ::CreateFontIndirect(&ncm.lfMenuFont);
	HDC hdc = CreateIC("DISPLAY", NULL, NULL, NULL);
	TEXTMETRIC tm;
	hfont = (HFONT) ::SelectObject(hdc, hfont);
	::GetTextMetrics(hdc, &tm);
	int h = tm.tmAscent + tm.tmDescent;
	::DeleteObject(SelectObject(hdc, hfont));
	::DeleteDC(hdc);

	DUMP(h);

	Draw::SetStdFont(Font(Font::FindFaceNameIndex(ncm.lfMenuFont.lfFaceName), abs(ncm.lfMenuFont.lfHeight)));

/*
	HGDIOBJ hfont = GetStockObject(DEFAULT_GUI_FONT);
	HDC hdc = CreateIC("DISPLAY", NULL, NULL, NULL);
	hfont = SelectObject(hdc, hfont);
	char h[513];
	GetTextFace(hdc, 512, h);
	DUMP(h);
	SelectObject(hdc, hfont);
	DeleteDC(hdc);
*/

	Visual().Zoomable().Run();
}
