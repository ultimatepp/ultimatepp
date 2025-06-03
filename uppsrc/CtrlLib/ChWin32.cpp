#include "CtrlLib.h"

// #define STATIC_IMAGES // avoids calling theming API while app is running - probably wrong path

#ifdef GUI_WIN

#define LLOG(x)   // RLOG(x)
#define LTIMING(x) // RTIMING(x)

#include <uxtheme.h>
#if defined(_MSC_VER) && _MSC_VER > 1400	// Visual C > 2005
#include <vssym32.h>
#else
#include <tmschema.h>
#endif

namespace Upp {

#define DLLFILENAME "uxtheme.dll"
#define DLIMODULE   XpTheme
#define DLIHEADER   <CtrlLib/XPTheme.dli>
#define DLLCALL     STDAPICALLTYPE
#define WIDGET(x)
#include <Core/dli.h>
#undef  WIDGET

#define FN(a, b, c)
#define WIDGET(x)    XP_##x,
enum {
#include "XPTheme.dli"
	XP_COUNT
};

#undef  WIDGET
#define WIDGET(x)   #x,

static const char *xp_widget_name[] = {
#include "XPTheme.dli"
	NULL
};

#undef  WIDGET
#undef  FN

struct XpElement : Moveable<XpElement> {
	int16 widget;
	int8  part;
	int8  state;

	bool  whista = false;
	bool  contentm = false;

	bool  operator==(const XpElement& e) const {
		return e.widget == widget && e.part == part && e.state == state;
	}

	hash_t GetHashValue() const { return widget ^ part ^ state; }
};

static HANDLE xp_widget_handle[XP_COUNT];
static VectorMap<XpElement, int> xp_margin;
static VectorMap<XpElement, int> xp_opaque;

void XpClear()
{
	for(int i = 0; i < XP_COUNT; i++)
		if(xp_widget_handle[i]) {
			XpTheme().CloseThemeData(xp_widget_handle[i]);
			xp_widget_handle[i] = NULL;
		}
	xp_margin.Clear();
	xp_opaque.Clear();
}

HANDLE XpWidget(int widget)
{
	if(xp_widget_handle[widget] == NULL && XpTheme())
		xp_widget_handle[widget] = XpTheme().OpenThemeData(NULL,
		                            ToSystemCharsetW(xp_widget_name[widget]));
	return xp_widget_handle[widget];
}

Image XpImage0(int widget, int part, int state, Color color, Size sz, int margin = 0)
{
	HANDLE theme = XpWidget(widget);
	if(!theme)
		return Null;
	if(IsNull(sz))
		XpTheme().GetThemePartSize(theme, ScreenHDC(), part, state, NULL,
		                           1 /*TS_TRUE*/, sz);
	Color c = White;
	Image m[2];
	for(int q = 0; q < 2; q++) {
		ImageDraw iw(sz + 2 * margin);
		iw.DrawRect(sz + 2 * margin, Nvl(color, c));
		Rect r(sz);
		r.Offset(margin, margin);
		XpTheme().DrawThemeBackground(theme, iw.BeginGdi(), part, state, r, NULL);
		iw.EndGdi();
		m[q] = iw;
		if(!IsNull(color))
			return m[q];
		c = Black;
	}
	return RecreateAlpha(m[0], m[1]);
}

bool sEmulateDarkTheme;

Color sAdjust(Color c)
{
	return sEmulateDarkTheme ? DarkTheme(c) : c;
}

Image XpImage(int widget, int part, int state, Color color = Null, Size sz = Null, int margin = 0)
{
	Image m = XpImage0(widget, part, state, color, sz, margin);
	return sEmulateDarkTheme ? DarkTheme(m) : m;
}

bool XpIsAvailable(int widget, int part, int state)
{
	HANDLE theme = XpWidget(widget);
	if(!theme)
		return false;
	return XpTheme().IsThemePartDefined(theme, part, state);
}

int XpMargin(const XpElement& e)
{
	int q = xp_margin.Find(e);
	if(q >= 0)
		return xp_margin[q];

	q = ImageMargin(XpImage(e.widget, e.part, e.state, White, Size(30, 30)), 4, 10);
	xp_margin.Add(e, q);
	return q;
}

Color XpColor(int widget, int part, int state, int type)
{
	COLORREF cref = 0;
	HANDLE theme = XpWidget(widget);
	if(!theme)
		return Null;
	int r = XpTheme().GetThemeColor(theme, part, state, type, &cref);
	return r == S_OK ? sAdjust(Color::FromCR(cref)) : Null;
}

int XpInt(int widget, int part, int state, int type)
{
	HANDLE theme = XpWidget(widget);
	if(!theme)
		return Null;
	int n = Null;
	int r = XpTheme().GetThemeInt(theme, part, state, type, &n);
	return r == S_OK ? n : Null;
}

int XpBool(int widget, int part, int state, int type)
{
	HANDLE theme = XpWidget(widget);
	if(!theme)
		return Null;
	BOOL flag = false;
	int r = XpTheme().GetThemeBool(theme, part, state, type, &flag);
	return r == S_OK ? flag : Null;
}

void SetXpImages(int uii, int n, int widget, int part, int state)
{
	for(int i = 0; i < n; i++) {
		Image m = XpImage(widget, part, state++);
		if(!IsNull(m))
			CtrlsImg::Set(uii++, m);
	}
}

void SetXpColors(Color *color, int n, int widget, int part, int state, int type)
{
	int ii = 0;
	for(int i = 0; i < n; i++) {
		Color c = XpColor(widget, part, state++, type);
		if(!IsNull(c))
			color[ii++] = c;
	}
}

struct Win32ImageMaker : ImageMaker {
	int        widget;
	int        part;
	int        state;
	Size       sz;

	virtual String Key() const {
		StringBuffer key;
		RawCat(key, widget);
		RawCat(key, part);
		RawCat(key, state);
		RawCat(key, sz);
		return String(key);
	}
	virtual Image Make() const {
		Rect rr(sz);
		Image m[2];
		for(int i = 0; i < 2; i++) {
			ImageDraw iw(sz);
			iw.DrawRect(sz, i ? Black() : White());
			XpTheme().DrawThemeBackground(XpWidget(widget), iw.BeginGdi(), part, state, rr, NULL);
			iw.EndGdi();
			m[i] = iw;
		}
		return DarkTheme(RecreateAlpha(m[0], m[1]));
	}
};

Value XpLookFn(Draw& w, const Rect& rect, const Value& v, int op, Color)
{
	op = op & ~LOOK_NOCACHE;
	if(IsTypeRaw<XpElement>(v)) {
		const XpElement& e = ValueTo<XpElement>(v);
		HANDLE htheme = XpWidget(e.widget);
		int q;
		switch(op) {
		case LOOK_MARGINS:
			if(e.contentm && htheme) {
				Rect r(0, 0, 100, 100);
				Rect cr;
				XpTheme().GetThemeBackgroundContentRect(htheme, NULL, e.part, e.state, r, cr);
				cr.bottom = 100 - cr.bottom;
				cr.right = 100 - cr.right;
				return cr;
			}
			q = XpMargin(e);
			return Rect(q, q, q, q);
		}
		if(op == LOOK_PAINT || op == LOOK_PAINTEDGE) {
			LTIMING("XpPaint");
			Rect r = rect;
			if(op == LOOK_PAINTEDGE) {
				q = XpMargin(e);
				w.Begin();
				w.ExcludeClip(r.left + q, r.top + q,
				              r.Width() - 2 * q, r.Height() - 2 * q);
			}
			if(e.whista) {
				w.Clip(r);
				r.right++;
				r.top--;
				r.bottom++;
			}
			if(htheme) {
				if(sEmulateDarkTheme) {
					Win32ImageMaker m;
					m.widget = e.widget;
					m.part = e.part;
					m.state = e.state;
					m.sz = r.GetSize();
					w.DrawImage(r.left, r.top, e.widget == XP_PROGRESS && findarg(e.part, PP_CHUNK, PP_CHUNKVERT) >= 0 ? m.Make() : MakeImage(m));
				}
				else {
					SystemDraw *sw = dynamic_cast<SystemDraw *>(&w);
					if(sw) {
						HDC hdc = sw->BeginGdi();
						XpTheme().DrawThemeBackground(htheme, hdc, e.part, e.state, r, NULL);
						sw->EndGdi();
					}
				}
			}
			if(e.whista)
				w.End();
			if(op == LOOK_PAINTEDGE)
				w.End();
			return 1;
		}
	}
	return Null;
}

struct chCtrlsImg {
	int id;
	int count;
	int widget;
	int part;
	int state;
};

static chCtrlsImg sImgs[] = {
	{ CtrlsImg::I_SizeGrip, 1, XP_STATUS, SP_GRIPPER, 0 },
	{ CtrlsImg::I_S0, 4, XP_BUTTON, BP_RADIOBUTTON, RBS_UNCHECKEDNORMAL },
	{ CtrlsImg::I_S1, 4, XP_BUTTON, BP_RADIOBUTTON, RBS_CHECKEDNORMAL },
	{ CtrlsImg::I_O0, 4, XP_BUTTON, BP_CHECKBOX, CBS_UNCHECKEDNORMAL },
	{ CtrlsImg::I_O1, 4, XP_BUTTON, BP_CHECKBOX, CBS_CHECKEDNORMAL },
	{ CtrlsImg::I_O2, 4, XP_BUTTON, BP_CHECKBOX, CBS_MIXEDNORMAL },
};

void Win32Look(Value *ch, int count, int widget, int part, int state = 1, bool contentm = false, bool img = false)
{
	for(int i = 0; i < count; i++) {
		XpElement e;
		e.widget = widget;
		e.part = part;
		e.state = state + i;
		e.contentm = contentm;
		if(img) {
			Image m = XpImage(widget, part, state + i);
			int hotspot;
			if(contentm) {
				Rect r(0, 0, 100, 100);
				Rect cr;
				HANDLE htheme = XpWidget(widget);
				XpTheme().GetThemeBackgroundContentRect(htheme, NULL, e.part, e.state, r, cr);
				hotspot = cr.left;
			}
			else
				hotspot = XpMargin(e);
			ch[i] = WithHotSpot(m, hotspot, hotspot);
		}
		else
			ch[i] = RawToValue(e);
	}
}

void Win32Look(Value& ch, int widget, int part, int state = 1, bool contentm = false, bool img = false)
{
	Win32Look(&ch, 1, widget, part, state, contentm, img);
}

String XpThemeInfo(LPCWSTR pszPropertyName)
{
	WCHAR theme[512], colors[512], size[512];
	XpTheme().GetCurrentThemeName(theme, 512, colors, 512, size, 512);
	WCHAR h[1024];
	XpTheme().GetThemeDocumentationProperty(theme, pszPropertyName, h, 1000);
	return ToUtf8(h);
}

struct sysColor {
	void (*set)(Color c);
	int syscolor;
};

static sysColor sSysColor[] = {
	{ SColorPaper_Write, COLOR_WINDOW },
	{ SColorFace_Write, COLOR_3DFACE },
	{ SColorText_Write, COLOR_WINDOWTEXT },
	{ SColorHighlight_Write, COLOR_HIGHLIGHT },
	{ SColorHighlightText_Write, COLOR_HIGHLIGHTTEXT },
	{ SColorMenu_Write, COLOR_3DFACE },
/*
	{ SColorMenu_Write, COLOR_MENU },
	{ SColorMenuText_Write, COLOR_MENUTEXT },
	{ SColorInfo_Write, COLOR_INFOBK },
	{ SColorInfoText_Write, COLOR_INFOTEXT },
	{ SColorLight_Write, COLOR_3DHILIGHT },
	{ SColorShadow_Write, COLOR_3DSHADOW },
*/
};

bool IsSysFlag(dword flag)
{
	BOOL b;
	return SystemParametersInfo(flag, 0, &b, 0) && b;
}

void ToImageIfDark(Value& v)
{ // Optimize performance - ChPaint will pick single-color body and use DrawRect
	if(sEmulateDarkTheme) {
		ImageDraw iw(64, 64);
		ChPaint(iw, Size(64, 64), v);
		Image m = iw;
		v = WithHotSpot(m, 8, 8);
	}
}

bool IsSystemThemeDark()
{
	String s = GetWinRegString("AppsUseLightTheme", "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", HKEY_CURRENT_USER);
	bool b = s.GetCount() && *s == 0;
	return GetEnv("UPP_DARKMODE__") == "1" ? !b : b;
}

void ChHostSkin0()
{
	if(Ctrl::IsUHDEnabled()) {
		HRESULT (STDAPICALLTYPE *SetProcessDpiAwareness)(int);
		DllFn(SetProcessDpiAwareness, "Shcore.dll", "SetProcessDpiAwareness");
		if(SetProcessDpiAwareness)
			SetProcessDpiAwareness(1);
		else {
			BOOL (STDAPICALLTYPE * SetProcessDPIAware)(void);
			DllFn(SetProcessDPIAware, "User32.dll", "SetProcessDPIAware");
			if(SetProcessDPIAware)
				(*SetProcessDPIAware)();
		}
	}

	NONCLIENTMETRICSW ncm;
#if (WINVER >= 0x0600 && !defined(__MINGW32_VERSION))
	ncm.cbSize = sizeof(ncm) - sizeof(ncm.iPaddedBorderWidth); // WinXP does not like it...
#else
	ncm.cbSize = sizeof(ncm);
#endif
	::SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
	String name = FromSystemCharsetW(ncm.lfMenuFont.lfFaceName);
	int height = abs((int)ncm.lfMenuFont.lfHeight);
	int q = Font::FindFaceNameIndex(name);
	if(height > 0 && height < 200) // sanity..
		Font::SetDefaultFont(Font(q >= 0 ? q : Font::SANSSERIF, height));

	XpClear();
	 
	if(sEmulateDarkTheme){
		SColorPaper_Write(Color(0x19, 0x19, 0x19));
		SColorFace_Write(Color(0x20, 0x20, 0x20));
		SColorText_Write(White());
		SColorMenu_Write(SColorFace());
		SColorLight_Write(FaceColor(0x20));
		SColorShadow_Write(FaceColor(0x20));
		SColorHighlight_Write(Color::FromCR(GetSysColor(COLOR_HIGHLIGHT)));
		SColorHighlightText_Write(Color::FromCR(GetSysColor(COLOR_HIGHLIGHTTEXT)));
		SColorMenuMark_Write(Color(0x80, 0x80, 0x80));
		DropEdge_Write(Color(0x80, 0x80, 0x80));
	}
	else for(sysColor *s = sSysColor; s < sSysColor + __countof(sSysColor); s++) // this also resets all imls via SColorPaper_Write!!!
		(*s->set)(sAdjust(Color::FromCR(GetSysColor(s->syscolor))));

	if(IsWin11())
		RoundStyleArrows();

	ChBaseSkin();
	
	GUI_GlobalStyle_Write(GUISTYLE_XP);
#ifndef PLATFORM_WINCE
	GUI_DragFullWindow_Write(IsSysFlag(SPI_GETDRAGFULLWINDOWS));
#endif
	GUI_PopUpEffect_Write(IsSysFlag(0x1002 /*SPI_GETMENUANIMATION*/) ?
	                      IsSysFlag(0x1012 /*SPI_GETMENUFADE*/) ? GUIEFFECT_FADE
	                                                            : GUIEFFECT_SLIDE
	                                                            : GUIEFFECT_NONE);
	GUI_DropShadows_Write(IsSysFlag(0x1024 /*SPI_GETDROPSHADOW*/));
	GUI_AltAccessKeys_Write(!IsSysFlag(0x100A /*SPI_GETKEYBOARDCUES*/));
	GUI_AKD_Conservative_Write(0);
	GUI_DragDistance_Write(GetSystemMetrics(SM_CXDRAG));
	GUI_DblClickTime_Write(GetDoubleClickTime());

	int slines;
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &slines, 0);
	GUI_WheelScrollLines_Write(slines);

	CtrlImg::Set(CtrlImg::I_information, Win32Icon(IDI_INFORMATION));
	CtrlImg::Set(CtrlImg::I_question, Win32Icon(IDI_QUESTION));
	CtrlImg::Set(CtrlImg::I_exclamation, Win32Icon(IDI_EXCLAMATION));
	CtrlImg::Set(CtrlImg::I_error, Win32Icon(IDI_ERROR));

	FrameButtonWidth_Write(GetSystemMetrics(SM_CYHSCROLL));
	ScrollBarArrowSize_Write(GetSystemMetrics(SM_CXHSCROLL));

	if(!sEmulateDarkTheme){
		dword x = GetSysColor(COLOR_GRAYTEXT);
		SColorDisabled_Write(sAdjust(x ? Color::FromCR(x) : Color::FromCR(GetSysColor(COLOR_3DSHADOW))));
	}
	else SColorDisabled_Write(Color(0x80, 0x80, 0x80));

	ChLookFn(XpLookFn);

	if(XpWidget(XP_BUTTON)) {
		GUI_GlobalStyle_Write(GUISTYLE_XP);
		EditFieldIsThin_Write(1);

		bool vista_aero = IsWinVista() && XpThemeInfo(L"ThemeName") == "Aero";

		if(vista_aero) {
			if(IsWin11()) {
				for(int i = 0; i < 4; i++) {
					ImagePainter sw(9, 9);
					sw.Clear(RGBAZero());
					sw.RoundedRectangle(0.5, 0.5, 8, 8, DPI(1))
						.Fill(i == CTRL_DISABLED ? SColorFace() : SColorPaper())
						.Stroke(1, i == CTRL_PRESSED ? SColorHighlight() : Gray());
					EditField::Style& s = EditField::StyleDefault().Write();
					s.activeedge = true;
					s.edge[i] = WithHotSpot(sw.GetResult(), DPI(1), DPI(1));
				}
			}
			else
			for(int i = 0; i < 4; i++) {
				int efp = 6;
				int efs = i + 1;
				int ebsx = max(2, XpInt(XP_EDIT, efp, efs, 2403/*TMT_BORDERSIZE*/));
				int ebsy = max(1, XpInt(XP_EDIT, efp, efs, 2403/*TMT_BORDERSIZE*/));
				Image ee = XpImage(XP_EDIT, efp, efs, SColorFace(), Size(10 * ebsx, 10 * ebsx));
				ImageBuffer eb(ee);
				eb.SetHotSpot(Point(ebsx, ebsy));
				ee = eb;
				EditField::Style& s = EditField::StyleDefault().Write();
				s.activeedge = true;
				if(!sEmulateDarkTheme) s.edge[i] = ee;
				else s.edge[i] = SColorShadow();
			}
		}
		else {
			int efp = EP_EDITTEXT;
			int efs = 1;
			int ebsx = max(2, XpInt(XP_EDIT, efp, efs, 2403/*TMT_BORDERSIZE*/));
			int ebsy = max(1, XpInt(XP_EDIT, efp, efs, 2403/*TMT_BORDERSIZE*/));
			Image ee = XpImage(XP_EDIT, efp, efs, SColorFace(), Size(10 * ebsx, 10 * ebsx));
			ImageBuffer eb(ee);
			eb.SetHotSpot(Point(ebsx, ebsy));
			ee = eb;
			EditField::Style& s = EditField::StyleDefault().Write();
			s.activeedge = false;
			s.edge[0] = ee;
		}

		int ebs = max(2, XpInt(XP_LISTVIEW, 0, 1, 2403/*TMT_BORDERSIZE*/));
		Image ee = XpImage(XP_LISTVIEW, 0, 1, SColorFace(), Size(10 * ebs, 10 * ebs));
		ImageBuffer eb(ee);
		eb.SetHotSpot(Point(ebs, ebs));
		ee = eb;
		if(!sEmulateDarkTheme) ViewEdge_Write(ee);
		else ViewEdge_Write(SColorShadow());

		for(chCtrlsImg *m = sImgs; m < sImgs + __countof(sImgs); m++)
			SetXpImages(m->id, m->count, m->widget, m->part, m->state);

		{
			Button::Style& s = Button::StyleNormal().Write();
			Win32Look(s.look, 4, XP_BUTTON, BP_PUSHBUTTON);
			SetXpColors(s.textcolor, 4, XP_BUTTON, BP_PUSHBUTTON, PBS_NORMAL, 3803/*TMT_TEXTCOLOR*/);
		}
		{
			Button::Style& s = Button::StyleOk().Write();
			Win32Look(s.look, 4, XP_BUTTON, BP_PUSHBUTTON);
			Win32Look(s.look[0], XP_BUTTON, BP_PUSHBUTTON, PBS_DEFAULTED);
		}
		{
			ToolBar::Style& s = ToolBar::StyleDefault().Write();
			Win32Look(s.buttonstyle.look, 6, XP_TOOLBAR, 1, 1);
			Win32Look(ToolButton::StyleDefault().Write().look, 6, XP_TOOLBAR, 1, 1);
			if(!sEmulateDarkTheme){
				Win32Look(s.arealook, XP_REBAR, 0, 1);
				ToImageIfDark(s.arealook);
			}
			s.breaksep.l2 = SColorLight();
		}
		Win32Look(StatusBar::StyleDefault().Write().look, XP_STATUS, 0, 1);
		{
			ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
			Win32Look(s.hupper, 4, XP_SCROLLBAR, SBP_LOWERTRACKHORZ);
			Win32Look(s.hlower, 4, XP_SCROLLBAR, SBP_UPPERTRACKHORZ);
			Win32Look(s.vupper, 4, XP_SCROLLBAR, SBP_LOWERTRACKVERT);
			Win32Look(s.vlower, 4, XP_SCROLLBAR, SBP_UPPERTRACKVERT);
			Win32Look(s.up.look, 4, XP_SCROLLBAR, SBP_ARROWBTN, ABS_UPNORMAL);
			Win32Look(s.down.look, 4, XP_SCROLLBAR, SBP_ARROWBTN, ABS_DOWNNORMAL);
			Win32Look(s.left.look, 4, XP_SCROLLBAR, SBP_ARROWBTN, ABS_LEFTNORMAL);
			Win32Look(s.right.look, 4, XP_SCROLLBAR, SBP_ARROWBTN, ABS_RIGHTNORMAL);
		}
		{
			TabCtrl::Style& s = TabCtrl::StyleDefault().Write();
			if(!sEmulateDarkTheme){
				Win32Look(s.normal, 4, XP_TAB, TABP_TABITEM);
				Win32Look(s.first, 4, XP_TAB, TABP_TABITEMLEFTEDGE);
				Win32Look(s.last, 4, XP_TAB, TABP_TABITEM);
				Win32Look(s.both, 4, XP_TAB, TABP_TABITEMBOTHEDGE);
				Win32Look(s.body, XP_TAB, TABP_PANE);
				ToImageIfDark(s.body);
			}
			else {
				for(int i = 0; i < 4; i++)
					SyntheticTab(i, IsWin11() ? 2 : 0, Gray(), 1);
			}
		}
		{
			MultiButton::Style& s = MultiButton::StyleDefault().Write();
			s.usetrivial = true;
			if(vista_aero) {
				s.edge[0] = Null;
				Win32Look(s.look, 4, XP_COMBOBOX, 5);
				s.trivialborder = s.border = 0;
				s.sep1 = XpColor(XP_COMBOBOX, 5, 1, 3822/*TMT_BORDERCOLORHINT*/);
				s.sep2 = Null;
				s.sepm = 2;
				{
					MultiButton::Style& s = MultiButton::StyleFrame().Write();
					for(int i = 0; i < 4; i++) {
						s.left[i] = Unglyph(XpImage(XP_COMBOBOX, 7, i + 1, Null, Size(40, 40)));
						s.right[i] = Unglyph(XpImage(XP_COMBOBOX, 6, i + 1, Null, Size(40, 40)));
						s.lmiddle[i] = s.rmiddle[i] = HorzBlend(s.right[i], s.left[i], 6, 14);
						Win32Look(s.look[i], XP_COMBOBOX, 4, i + 1);
					}
					s.activeedge = true;
					s.sep1 = Null;
				}
				if(IsWin11())
					s.sep1 = SLtGray(); // affects separator line in TheIDE build method
			}
			else {
				Win32Look(s.trivial, 4, XP_COMBOBOX, CP_DROPDOWNBUTTON);
				Win32Look(s.edge[0], XP_COMBOBOX, 0, 0);
				s.border = s.trivialborder = XpInt(XP_COMBOBOX, CP_DROPDOWNBUTTON, CBXS_NORMAL, 2403/*TMT_BORDERSIZE*/);
			}
		}

		{
			HeaderCtrl::Style& hs = HeaderCtrl::StyleDefault().Write();
			for(int i = 0; i < 4; i++) {
				Image h = CreateImage(Size(10, 10), FaceColor(decode(i, CTRL_NORMAL, 5,
				                                                     CTRL_HOT, IsDarkTheme() ? 10 : 0,
				                                                     CTRL_PRESSED, -5,
				                                                     -8)));
				hs.look[i] = ChHot(WithBottomLine(WithRightLine(h, SColorShadow(), 1), SColorShadow()));
			}
		}
		{
			ProgressIndicator::Style& s = ProgressIndicator::StyleDefault().Write();
			Win32Look(s.hlook, XP_PROGRESS, PP_BAR, 0, true);
			Win32Look(s.vlook, XP_PROGRESS, PP_BARVERT, 0, true);
			Win32Look(s.hchunk, XP_PROGRESS, PP_CHUNK, 1, false, sEmulateDarkTheme);
			Win32Look(s.vchunk, XP_PROGRESS, PP_CHUNKVERT, 1, false, sEmulateDarkTheme);
		}
		{
			MenuBar::Style& s = MenuBar::StyleDefault().Write();
			if(vista_aero) {
				s.topitemtext[0] = s.topitemtext[1] = s.topitemtext[2] = XpColor(XP_MENU, 8 /*MENU_BARITEM*/,
					2 /*HOT*/, 3803/*TMT_TEXTCOLOR*/);
				s.itemtext = XpColor(XP_MENU, 14 /*MENU_POPUPITEM*/,
					2 /*HOT*/, 3803/*TMT_TEXTCOLOR*/);
				Win32Look(s.item, XP_MENU, 14 /*MENU_POPUPITEM*/, 2 /*HOT*/);
				Win32Look(s.topitem[1], XP_MENU, 8 , 2 /*HOT*/);
				Win32Look(s.topitem[2], XP_MENU, 8 , 3 /*HOT*/);
				Win32Look(s.popupiconbar, XP_MENU, 13, 1);
				s.leftgap = DPI(32);
				s.textgap = Zx(6);
				s.lsepm = Zx(32);
				s.separator.l1 = Blend(SColorMenu(), SColorShadow());
				s.separator.l2 = SColorLight();
			}
			if(!sEmulateDarkTheme){
				Win32Look(s.arealook, XP_REBAR, 0, 1);
				ToImageIfDark(s.arealook);
			}
			else{
				s.topbar = s.arealook = SColorFace();
				s.topitem[1] = s.topitem[2] = s.popupbody = s.popupiconbar = Blend(SColorFace(), SColorShadow(),92);
				s.popupframe = Black();
				s.item = SColorShadow();
			}
			CtrlImg::Set(CtrlImg::I_MenuCheck0, CtrlsImg::O0());
			CtrlImg::Set(CtrlImg::I_MenuCheck1, CtrlsImg::O1());
			CtrlImg::Set(CtrlImg::I_MenuRadio0, CtrlsImg::S0());
			CtrlImg::Set(CtrlImg::I_MenuRadio1, CtrlsImg::S1());
		}

/*		CtrlImg::Set("hthumb", XpImage(XP_TRACKBAR, TKP_THUMB, TUS_NORMAL, Null, Size(10, 20)));
		CtrlImg::Set("hthumb1", XpImage(XP_TRACKBAR, TKP_THUMB, TUS_PRESSED, Null, Size(10, 20)));
		CtrlImg::Set("vthumb", XpImage(XP_TRACKBAR, TKP_THUMBVERT, TUS_NORMAL, Null, Size(20, 10)));
		CtrlImg::Set("vthumb1", XpImage(XP_TRACKBAR, TKP_THUMBVERT, TUS_PRESSED, Null, Size(20, 10)));
*/
		XpElement e;
		for(int i = 0; i < 4; i++) {
			{
				ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
				if(IsWin11()) {
					s.arrowsize = 0;
					s.thumbmin = DPI(24);
					int g = IsDarkTheme() ? get_i(i, 80, 100, 70, 70)
					                      : get_i(i, 192, 200, 128, 128);
					s.vthumb[i] = MakeRoundScrollbarThumb(DPI(16), DPI(4), GrayColor(g));
					s.hthumb[i] = RotateClockwise(s.vthumb[i]);
				}
				else {
					e.widget = XP_SCROLLBAR;
					e.state = 1 + i;
					e.contentm = false;
					e.part = SBP_THUMBBTNHORZ;
					s.hthumb[i] = ChLookWith(RawToValue(e), XpImage(XP_SCROLLBAR, SBP_GRIPPERHORZ, 1));
					e.part = SBP_THUMBBTNVERT;
					s.vthumb[i] = ChLookWith(RawToValue(e), XpImage(XP_SCROLLBAR, SBP_GRIPPERVERT, 1));
				}
			}
			Color paper = i == 3 ? SColorFace : SColorPaper;
			if(IsWin11())
				paper = Null;
			Image m = XpImage(XP_COMBOBOX, CP_DROPDOWNBUTTON, CBXS_NORMAL + i, paper, Size(32, 32));
			Image mm = m;
			Size isz = m.GetSize();
			int cbs = XpInt(XP_COMBOBOX, CP_DROPDOWNBUTTON, CBXS_NORMAL + i, 2403/*TMT_BORDERSIZE*/);
			if(cbs == 0) {
				Image h = HorzSymm(m);
				Button::StyleLeftEdge().Write().look[i] = Unglyph(Crop(h, 1, 1, isz.cx - 1, isz.cy - 2));
				m = Crop(h, 0, 1, isz.cx - 1, isz.cy - 2);
			}
			bool outer = false;
			if(cbs == 1 &&
			   XpBool(XP_COMBOBOX, CP_DROPDOWNBUTTON, CBXS_NORMAL + i, 2203/*TMT_BORDERONLY*/) == 0 &&
			   XpInt(XP_COMBOBOX, 0, 0, 2403/*TMT_BORDERSIZE*/) == 1 &&
			   XpBool(XP_COMBOBOX, 0, 0, 2203/*TMT_BORDERONLY*/) == 0) {
					m = Crop(m, 0, 1, isz.cx - 1, isz.cy - 2);
					e.widget = XP_COMBOBOX;
					e.part = CP_DROPDOWNBUTTON;
					e.state = CBXS_NORMAL + i;
					e.whista = true;
					outer = true;
			}
			Color c;
			double gf;
			m = Unglyph(m, c, gf);
			if(i == 0 && (gf > 150 || vista_aero))
				CtrlsImg::Set(CtrlsImg::I_DA, CtrlsImg::kDA());
			Button::StyleEdge().Write().look[i] = m;
			if(cbs)
				Button::StyleLeftEdge().Write().look[i] = m;

			MultiButton::StyleDefault().Write().simple[i] = m;
			MultiButton::StyleFrame().Write().simple[i] = m;

			Button::StyleNormal().Write().monocolor[i] = c;
			Button::StyleOk().Write().monocolor[i] = c;
			Button::StyleEdge().Write().monocolor[i] = c;
			Button::StyleLeftEdge().Write().monocolor[i] = c;
			Button::StyleScroll().Write().monocolor[i] = c;

			MultiButton::StyleDefault().Write().monocolor[i] = c;
			MultiButton::StyleFrame().Write().monocolor[i] = c;
			if(!vista_aero) {
				MultiButton::Style& s = MultiButton::StyleDefault().Write();
				MultiButton::Style& fs = MultiButton::StyleFrame().Write();
				Image cm = Unglyph(XpImage(XP_COMBOBOX, CP_DROPDOWNBUTTON, i + 1, Null, Size(40, 40)));
				fs.right[i] = s.right[i] = cm;
				fs.left[i] = s.left[i] = MirrorHorz(s.right[i]);
				if(s.border == 0) {
					mm = Crop(Unglyph(cm), 0, 0, 12, 20);
					ImageBuffer ib(mm);
					ib.SetHotSpot(cm.GetHotSpot());
					mm = ib;
				}
				else
					mm = m;
				fs.rmiddle[i] = s.rmiddle[i] = mm;
				fs.lmiddle[i] = s.lmiddle[i] = MirrorHorz(mm);
			}

			m = Unglyph(XpImage(XP_SCROLLBAR, SBP_ARROWBTN, i + ABS_UPNORMAL, paper, Size(40, 40)));
			Button::StyleScroll().Write().look[i] =
			      VertBlend(m, Unglyph(XpImage(XP_SCROLLBAR, SBP_ARROWBTN, i + ABS_DOWNNORMAL,
			                                   paper, Size(40, 40))),
			                14, 26);
		}

		{
			SpinButtons::Style& s = SpinButtons::StyleDefault().Write();
			if(!IsWin11()) {
				Win32Look(s.inc.look, 4, XP_SPIN, SPNP_UP);
				Win32Look(s.dec.look, 4, XP_SPIN, SPNP_DOWN);
			}
			s.width = FrameButtonWidth();
		}
	//	{
	//		SpinButtons::Style& s = SpinButtons::StyleOnSides().Write();
	//		Win32Look(s.inc.look, 4, XP_SCROLLBAR, SBP_ARROWBTN, ABS_UPNORMAL);
	//		Win32Look(s.dec.look, 4, XP_SCROLLBAR, SBP_ARROWBTN, ABS_DOWNNORMAL);
	//	}

//		LabelBoxTextColor_Write(XpColor(XP_BUTTON, BP_GROUPBOX, GBS_NORMAL, 3803/*TMT_TEXTCOLOR*/));
//		LabelBoxColor_Write(XpColor(XP_BUTTON, BP_GROUPBOX, GBS_NORMAL, 3822/*TMT_BORDERCOLORHINT*/));
	}
	else
		ChClassicSkin();
}

void ChHostSkin()
{
	sEmulateDarkTheme = Ctrl::IsDarkThemeEnabled() && IsSystemThemeDark() && !IsDark(Color::FromCR(GetSysColor(COLOR_WINDOW)));
	ChHostSkin0();
}

void ChHostSkinLight()
{
	sEmulateDarkTheme = false;
	ChHostSkin0();
}

void ChHostSkinDark()
{
	sEmulateDarkTheme = true;
	ChHostSkin0();
}

}

#endif
