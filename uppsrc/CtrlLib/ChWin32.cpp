#include "CtrlLib.h"

NAMESPACE_UPP

#define LLOG(x)   // RLOG(x)
#define LTIMING(x) // RTIMING(x)

void ChSysInit();

#ifdef GUI_WIN

#ifdef PLATFORM_WINCE

void ChSysInit()
{
	CtrlImg::Reset();
	CtrlsImg::Reset();
	ChReset();
	GUI_GlobalStyle_Write(GUISTYLE_CLASSIC);
}

void ChHostSkin()
{
	ChClassicSkin();
}

#else

END_UPP_NAMESPACE

#include <uxtheme.h>
#if defined(_MSC_VER) && _MSC_VER > 1400	// Visual C > 2005
#include <vssym32.h>
#else
#include <tmschema.h>
#endif

NAMESPACE_UPP

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

	bool  whista;
	bool  contentm;

	bool  operator==(const XpElement& e) const {
		return e.widget == widget && e.part == part && e.state == state;
	}

	unsigned GetHashValue() const { return widget ^ part ^ state; }

	XpElement() { contentm = whista = false; }
};

static HANDLE xp_widget_handle[XP_COUNT];
static VectorMap<XpElement, int> xp_margin;
static VectorMap<XpElement, int> xp_opaque;

void XpClear()
{
	memset(xp_widget_handle, 0, sizeof(xp_widget_handle));
	xp_margin.Clear();
	xp_opaque.Clear();
}

HANDLE XpWidget(int widget)
{
	if(xp_widget_handle[widget] == NULL && XpTheme())
		xp_widget_handle[widget] = XpTheme().OpenThemeData(NULL,
		                            (WCHAR *)(const wchar *)WString(xp_widget_name[widget]));
	return xp_widget_handle[widget];
}

Image XpImage(int widget, int part, int state, Color color = Null, Size sz = Null)
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
		ImageDraw iw(sz);
		iw.DrawRect(sz, Nvl(color, c));
		HDC hdc = iw.BeginGdi();
		Rect r(sz);
		XpTheme().DrawThemeBackground(theme, hdc, part, state, r, NULL);
		iw.EndGdi();
		m[q] = iw;
		if(!IsNull(color))
			return m[q];
		c = Black;
	}
	return RecreateAlpha(m[0], m[1]);
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

int XpIsOpaque(const XpElement& e)
{
	int q = xp_opaque.Find(e);
	if(q >= 0)
		return xp_opaque[q];
	Image m1 = XpImage(e.widget, e.part, e.state, Black, Size(30, 30));
	Image m2 = XpImage(e.widget, e.part, e.state, White, Size(30, 30));
	q = m1 == m2;
	xp_opaque.Add(e, q);
	return q;
}

Color XpColor(int widget, int part, int state, int type)
{
	COLORREF cref = 0;
	HANDLE theme = XpWidget(widget);
	if(!theme)
		return Null;
	int r = XpTheme().GetThemeColor(theme, part, state, type, &cref);
	return r == S_OK ? Color::FromCR(cref) : Null;
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


Value XpLookFn(Draw& w, const Rect& rect, const Value& v, int op)
{
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
		case LOOK_ISOPAQUE:
			return XpIsOpaque(e);
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
				SystemDraw *sw = dynamic_cast<SystemDraw *>(&w);
				if(sw) {
					HDC hdc = sw->BeginGdi();
					XpTheme().DrawThemeBackground(htheme, hdc, e.part, e.state, r, NULL);
					sw->EndGdi();
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

void Win32Look(Value *ch, int count, int widget, int part, int state = 1, bool contentm = false)
{
	for(int i = 0; i < count; i++) {
		XpElement e;
		e.widget = widget;
		e.part = part;
		e.state = state + i;
		e.contentm = contentm;
		ch[i] = RawToValue(e);
	}
}

void Win32Look(Value& ch, int widget, int part, int state = 1, bool contentm = false)
{
	Win32Look(&ch, 1, widget, part, state, contentm);
}

String XpThemeInfo(LPCWSTR pszPropertyName)
{
	wchar theme[512], colors[512], size[512];
	XpTheme().GetCurrentThemeName(theme, 512, colors, 512, size, 512);
	wchar h[1024];
	XpTheme().GetThemeDocumentationProperty(theme, pszPropertyName, h, 1000);
	return FromUnicode(h);
}

void ChHostSkin()
{
	ChSysInit();
	if(XpWidget(XP_BUTTON)) {
		LLOG("XP theme !");
		GUI_GlobalStyle_Write(GUISTYLE_XP);
		ColoredOverride(CtrlsImg::Iml(), CtrlsImg::Iml());
		CtrlsImg::Reset();
		EditFieldIsThin_Write(1);

		bool vista_aero = IsWinVista() && XpThemeInfo(L"ThemeName") == "Aero";

		if(vista_aero) {
			int efp = 6;
			for(int i = 0; i < 4; i++) {
				int efs = i + 1;
				int ebsx = max(2, XpInt(XP_EDIT, efp, efs, 2403/*TMT_BORDERSIZE*/));
				int ebsy = max(1, XpInt(XP_EDIT, efp, efs, 2403/*TMT_BORDERSIZE*/));
				Image ee = XpImage(XP_EDIT, efp, efs, SColorFace(), Size(10 * ebsx, 10 * ebsx));
				ImageBuffer eb(ee);
				eb.SetHotSpot(Point(ebsx, ebsy));
				ee = eb;
				EditField::Style& s = EditField::StyleDefault().Write();
				s.activeedge = true;
				s.edge[i] = ee;
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
		ViewEdge_Write(ee);

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
			Win32Look(s.arealook, XP_REBAR, 0, 1);
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
			Win32Look(s.normal, 4, XP_TAB, TABP_TABITEM);
			Win32Look(s.first, 4, XP_TAB, TABP_TABITEMLEFTEDGE);
			Win32Look(s.last, 4, XP_TAB, TABP_TABITEM);
			Win32Look(s.both, 4, XP_TAB, TABP_TABITEMBOTHEDGE);
			Win32Look(s.body, XP_TAB, TABP_PANE);
		}
		{
			SpinButtons::Style& s = SpinButtons::StyleDefault().Write();
			Win32Look(s.inc.look, 4, XP_SPIN, SPNP_UP);
			Win32Look(s.dec.look, 4, XP_SPIN, SPNP_DOWN);
			s.width = FrameButtonWidth();
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
			}
			else {
				Win32Look(s.trivial, 4, XP_COMBOBOX, CP_DROPDOWNBUTTON);
				Win32Look(s.edge[0], XP_COMBOBOX, 0, 0);
				s.border = s.trivialborder = XpInt(XP_COMBOBOX, CP_DROPDOWNBUTTON, CBXS_NORMAL, 2403/*TMT_BORDERSIZE*/);
			}
		}
		{
			HeaderCtrl::Style& s = HeaderCtrl::StyleDefault().Write();
			Win32Look(s.look, 4, XP_HEADER, HP_HEADERITEM);
			Image hm = Unmultiply(XpImage(XP_HEADER, HP_HEADERITEM, 1, SColorPaper, Size(20, 10)));
			RGBA textc = SColorText();
			int diff = Diff(hm[4][19], textc);
			int hdroll = 0;
			for(int i = 0; i < 4; i++) {
				int d = Diff(hm[4][i], textc);
				if(d < diff) {
					diff = d;
					hdroll = i + 1;
				}
				d = Diff(hm[4][19 - i], textc);
				if(d < diff) {
					diff = d;
					hdroll = -i;
				}
			}
			s.gridadjustment = hdroll;
		}
		{
			ProgressIndicator::Style& s = ProgressIndicator::StyleDefault().Write();
			Win32Look(s.hlook, XP_PROGRESS, PP_BAR, 0, true);
			Win32Look(s.vlook, XP_PROGRESS, PP_BARVERT, 0, true);
			Win32Look(s.hchunk, XP_PROGRESS, PP_CHUNK);
			Win32Look(s.vchunk, XP_PROGRESS, PP_CHUNKVERT);
		}
		{
			MenuBar::Style& s = MenuBar::StyleDefault().Write();
			if(vista_aero) {
				s.topitemtext[0] = s.topitemtext[1] = s.topitemtext[2] =
					s.itemtext = XpColor(XP_MENU, 8 /*MENU_POPUPITEM*/,
					2 /*HOT*/, 3803/*TMT_TEXTCOLOR*/);
				Win32Look(s.item, XP_MENU, 14 /*MENU_POPUPITEM*/, 2 /*HOT*/);
				Win32Look(s.topitem[1], XP_MENU, 8 , 2 /*HOT*/);
				Win32Look(s.topitem[2], XP_MENU, 8 , 3 /*HOT*/);
				Win32Look(s.popupiconbar, XP_MENU, 13, 1);
				s.leftgap = Zx(32);
				s.textgap = Zx(6);
				s.lsepm = Zx(32);
				s.separator.l1 = Blend(SColorMenu(), SColorShadow());
				s.separator.l2 = SColorLight();
			}
			Win32Look(s.arealook, XP_REBAR, 0, 1);
		}

		CtrlImg::Set("hthumb", XpImage(XP_TRACKBAR, TKP_THUMB, TUS_NORMAL, Null, Size(10, 20)));
		CtrlImg::Set("hthumb1", XpImage(XP_TRACKBAR, TKP_THUMB, TUS_PRESSED, Null, Size(10, 20)));
		CtrlImg::Set("vthumb", XpImage(XP_TRACKBAR, TKP_THUMBVERT, TUS_NORMAL, Null, Size(20, 10)));
		CtrlImg::Set("vthumb1", XpImage(XP_TRACKBAR, TKP_THUMBVERT, TUS_PRESSED, Null, Size(20, 10)));

		XpElement e;
		for(int i = 0; i < 4; i++) {
			{
				ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
				e.widget = XP_SCROLLBAR;
				e.state = 1 + i;
				e.contentm = false;
				e.part = SBP_THUMBBTNHORZ;
				s.hthumb[i] = ChLookWith(RawToValue(e), XpImage(XP_SCROLLBAR, SBP_GRIPPERHORZ, 1));
				e.part = SBP_THUMBBTNVERT;
				s.vthumb[i] = ChLookWith(RawToValue(e), XpImage(XP_SCROLLBAR, SBP_GRIPPERVERT, 1));
			}
			Color paper = i == 3 ? SColorFace : SColorPaper;
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
				CtrlsImg::Set(CtrlsImg::I_DA, ClassicCtrlsImg::DA());
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

		LabelBoxTextColor_Write(XpColor(XP_BUTTON, BP_GROUPBOX, GBS_NORMAL, 3803/*TMT_TEXTCOLOR*/));
		LabelBoxColor_Write(XpColor(XP_BUTTON, BP_GROUPBOX, GBS_NORMAL, 3822/*TMT_BORDERCOLORHINT*/));

		ChLookFn(XpLookFn);
	}
	else
		ChClassicSkin();
}

struct sysColor {
	void (*set)(Color c);
	int syscolor;
};

static sysColor sSysColor[] = {
	{ SColorFace_Write, COLOR_3DFACE },
	{ SColorPaper_Write, COLOR_WINDOW },
	{ SColorText_Write, COLOR_WINDOWTEXT },
	{ SColorHighlight_Write, COLOR_HIGHLIGHT },
	{ SColorHighlightText_Write, COLOR_HIGHLIGHTTEXT },
	{ SColorMenu_Write, COLOR_MENU },
	{ SColorMenuText_Write, COLOR_MENUTEXT },
	{ SColorInfo_Write, COLOR_INFOBK },
	{ SColorInfoText_Write, COLOR_INFOTEXT },
	{ SColorLight_Write, COLOR_3DHILIGHT },
	{ SColorShadow_Write, COLOR_3DSHADOW },
};

bool IsSysFlag(dword flag)
{
	BOOL b;
	return SystemParametersInfo(flag, 0, &b, 0) && b;
}

void ChSysInit()
{
	CtrlImg::Reset();
	CtrlsImg::Reset();
	ChReset();
	XpClear();

	BOOL (STDAPICALLTYPE * SetProcessDPIAware)(void);
	DllFn(SetProcessDPIAware, "User32.dll", "SetProcessDPIAware");
	if(SetProcessDPIAware && Ctrl::IsHiDPIEnabled())
		(*SetProcessDPIAware)();
	NONCLIENTMETRICS ncm;
#if (WINVER >= 0x0600)
	ncm.cbSize = sizeof(ncm) - sizeof(ncm.iPaddedBorderWidth); // WinXP does not like it...
#else
	ncm.cbSize = sizeof(ncm);
#endif
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
	String name = FromSystemCharset(ncm.lfMenuFont.lfFaceName);
	int height = abs((int)ncm.lfMenuFont.lfHeight);
	
	DDUMP(name);
	DDUMP(height);
	
	int q = Font::FindFaceNameIndex(name);
	if(height > 0 && height < 200) // sanity..
		Font::SetDefaultFont(Font(q >= 0 ? q : Font::SANSSERIF, height));
	
	GUI_HiDPI_Write(GetStdFontCy() > 22);

	GUI_GlobalStyle_Write(IsWinXP() && !ScreenInPaletteMode() && IsSysFlag(0x1022 /*SPI_GETFLATMENU*/)
	                      ? GUISTYLE_XP : GUISTYLE_CLASSIC);
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
	for(sysColor *s = sSysColor; s < sSysColor + __countof(sSysColor); s++)
		(*s->set)(Color::FromCR(GetSysColor(s->syscolor)));
	dword x = GetSysColor(COLOR_GRAYTEXT);
	SColorDisabled_Write(x ? Color::FromCR(x) : Color::FromCR(GetSysColor(COLOR_3DSHADOW)));
}

#endif

#endif

END_UPP_NAMESPACE
