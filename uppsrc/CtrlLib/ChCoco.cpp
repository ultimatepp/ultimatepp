#include "ChCoco.h"

#ifdef PLATFORM_COCOA

namespace Upp {
	
Image Hot3(const Image& m)
{
	Size sz = m.GetSize();
	return WithHotSpots(m, sz.cx / 3, sz.cy / 3, sz.cx - sz.cx / 3, sz.cy - sz.cy / 3);
}

Image Coco_ThemeImage(Color bg, int cx, int cy, int margin, int type,
                      int value, int state, bool focus)
{
	Size isz(cx + 2 * margin, cy + 2 * margin);
	ImageDraw iw(isz);
	iw.DrawRect(isz, bg);
	Coco_ThemePaint(iw.GetCGHandle(), Rect(margin, margin, cx, cy), type, value, state, focus);
	return Hot3(iw);
}

Image Coco_ThemeImage(int cx, int cy, int margin, int type, int value, int state, bool focus)
{
	return Hot3(RecreateAlpha(Coco_ThemeImage(White(), cx, cy, margin, type, value, state, focus),
	                          Coco_ThemeImage(Black(), cx, cy, margin, type, value, state, focus)));
}

Color AvgColor(const Image& m, const Rect& rr)
{
	int n = rr.GetWidth() * rr.GetHeight();
	if(n <= 0)
		return White();
	int r = 0;
	int g = 0;
	int b = 0;
	for(int y = rr.top; y < rr.bottom; y++)
		for(int x = rr.left; x < rr.right; x++) {
			RGBA c = m[y][x];
			r += c.r;
			g += c.g;
			b += c.b;
		}
	return Color(r / n, g / n, b / n);
}

Color AvgColor(const Image& m, int margin = 0)
{
	return AvgColor(m, Rect(m.GetSize()).Deflated(margin));
}

void SOImages(int imli, int type, int value)
{
	Image h[4];
	h[0] = Coco_ThemeImage(DPI(20), DPI(20), 10, type, value, CTRL_NORMAL);
	h[1] = Coco_ThemeImage(DPI(20), DPI(20), 10, type, value, CTRL_HOT); // same as Normal
	h[2] = Coco_ThemeImage(DPI(20), DPI(20), 10, type, value, CTRL_PRESSED);
	h[3] = Coco_ThemeImage(DPI(20), DPI(20), 10, type, value, CTRL_DISABLED);
	
	AutoCrop(h, 4);
	
	for(int i = 0; i < 4; i++)
		CtrlsImg::Set(imli++, Hot3(h[i]));
}

void CocoButton(Image *h, int type, int value)
{
	h[0] = Coco_ThemeImage(DPI(40), DPI(32), 10, type, value, CTRL_NORMAL);
	h[1] = Coco_ThemeImage(DPI(40), DPI(32), 10, type, value, CTRL_HOT); // same as Normal
	h[2] = Coco_ThemeImage(DPI(40), DPI(32), 10, type, value, CTRL_PRESSED);
	h[3] = Coco_ThemeImage(DPI(40), DPI(32), 10, type, value, CTRL_DISABLED);

	AutoCrop(h, 4);
	
	for(int i = 0; i < 4; i++)
		h[i] = Hot3(h[i]);
}

void CocoButton(Button::Style& s, int type, int value)
{
	Image h[4];
	CocoButton(h, type, value);
	Size sz = h[0].GetSize();
	for(int i = 0; i < 4; i++) {
		s.look[i] = h[i];
		Image gg = CreateImage(h[i].GetSize(), SColorFace());
		Over(gg, h[i]);
		s.monocolor[i] = s.textcolor[i] = i == CTRL_DISABLED ? SColorDisabled()
		                 : Grayscale(AvgColor(gg, h[i].GetSize().cy / 3)) > 160 ? SColorText()
		                                                                        : White();
	}
	s.overpaint = 5;
	s.pressoffset = Point(0, 0);
}

Color CocoBrush(int k)
{
	return AvgColor(Coco_ThemeImage(16, 16, 0, COCO_BRUSH, k));
}

Color CocoColor(int k, Color bg = SColorFace())
{
	return AvgColor(Coco_ThemeImage(bg, 16, 16, 0, COCO_NSCOLOR, k));
}

Color GetInkColor(const Image& m)
{
	int x = m.GetSize().cx / 2;
	bool dark = !IsDark(SColorText());
	int best = 1000;
	Color bestc = SColorText();
	for(int y = 0; y < m.GetHeight(); y++) {
		Color c = m[y][x];
		int g = Grayscale(c);
		if(dark)
			g = 255 - g;
		if(g < best) {
			best = g;
			bestc = c;
		}
	}
	return bestc;
}

void ChHostSkin()
{
	CtrlImg::Reset();
	CtrlsImg::Reset();
	ChReset();

	GUI_GlobalStyle_Write(GUISTYLE_XP);

	DUMP(Coco_Metric(0)); // kThemeMetricScrollBarWidth
	DUMP(Coco_Metric(7)); // kThemeMetricFocusRectOutset
	DUMP(Coco_Metric(19)); // kThemeMetricPushButtonHeight

	int button_height = Coco_Metric(19); // kThemeMetricPushButtonHeight
	

	SwapOKCancel_Write(true);

	SColorFace_Write(CocoColor(COCO_WINDOW, White())); // ThemeBrushDialogBackgroundActive
	SColorHighlight_Write(CocoColor(COCO_SELECTEDPAPER));
	SColorHighlightText_Write(CocoColor(COCO_SELECTEDTEXT));
	SColorText_Write(CocoColor(COCO_TEXT));
	SColorPaper_Write(CocoColor(COCO_PAPER));
	SColorDisabled_Write(CocoColor(COCO_DISABLED));

	SOImages(CtrlsImg::I_S0, COCO_RADIOBUTTON, 0);
	SOImages(CtrlsImg::I_S1, COCO_RADIOBUTTON, 1);
	SOImages(CtrlsImg::I_O0, COCO_CHECKBOX, 0);
	SOImages(CtrlsImg::I_O1, COCO_CHECKBOX, 1);
	SOImages(CtrlsImg::I_O2, COCO_CHECKBOX, 2);

	CocoButton(Button::StyleNormal().Write(), COCO_BUTTON, 0);
	CocoButton(Button::StyleOk().Write(), COCO_BUTTON, 1);
	CocoButton(Button::StyleEdge().Write(), COCO_BEVELBUTTON, 0);
	CocoButton(Button::StyleScroll().Write(), COCO_BEVELBUTTON, 0);

	{
		auto& s = ToolButton::StyleDefault().Write();
		Image h[4];
		CocoButton(h, COCO_ROUNDEDBUTTON, 0);
		s.look[CTRL_NORMAL] = Image();
		s.look[CTRL_HOT] = h[CTRL_HOT];
		s.look[CTRL_PRESSED] = h[CTRL_PRESSED];
		s.look[CTRL_DISABLED] = Image();
		CocoButton(h, COCO_ROUNDEDBUTTON, 1);
		s.look[CTRL_CHECKED] = h[CTRL_NORMAL];
		s.look[CTRL_HOTCHECKED] = h[CTRL_HOT];
	}

	{
		Color menuink = CocoColor(COCO_SELECTEDMENUTEXT);
		SColorMenu_Write(AvgColor(AutoCrop(Coco_ThemeImage(30, 20, 10, COCO_MENU, 0, CTRL_NORMAL))));
		SColorMenuText_Write(SColorText());

		MenuBar::Style& s = MenuBar::StyleDefault().Write();
		s.pullshift.y = 0;
		
		SColorMenu_Write(SColorFace());
		
		s.topitem[1] = s.topitem[0] = SColorFace();
		s.topitemtext[1] = SColorText();
		Image m = AutoCrop(Coco_ThemeImage(50, 50, 10, COCO_MENUITEM, 0, CTRL_HOT));
		Rect r = m.GetSize();
		if(r.GetWidth() > 10 && r.GetHeight() > 10)
			r.Deflate(5);
		m = Crop(m, r);
		s.item = s.topitem[2] = Hot3(m);
		s.topitemtext[0] = SColorText();
		s.itemtext = s.topitemtext[2] = menuink;
		s.look = SColorFace();
		s.opaquetest = false;
	}
	
	{
		ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
		s.arrowsize = 0; // no arrows
		s.through = true;
		s.barsize = s.thumbwidth = Coco_Metric(0); // kThemeMetricScrollBarWidth
		for(int status = CTRL_NORMAL; status <= CTRL_DISABLED; status++) {
			s.vupper[status] = s.vlower[status] =
				Hot3(Coco_ThemeImage(SColorFace(), s.barsize, 40, 0, COCO_SCROLLTRACK, 0, status));
			Image thumb = Coco_ThemeImage(s.barsize, 50, 0, COCO_SCROLLTHUMB, 0, status);
			Rect bounds = FindBounds(thumb);
			thumb = Crop(thumb, Rect(0, bounds.top, thumb.GetWidth(), bounds.bottom));
			thumb = AddMargins(thumb, 0, 1, 0, 1, RGBAZero());
			s.vthumb[status] = Hot3(thumb);

			s.hupper[status] = s.hlower[status] =
				Hot3(Coco_ThemeImage(SColorFace(), 40, s.barsize, 0, COCO_SCROLLTRACK, 1, status));
			thumb = Coco_ThemeImage(50, s.barsize, 0, COCO_SCROLLTHUMB, 1, status);
			bounds = FindBounds(thumb);
			thumb = Crop(thumb, Rect(bounds.left, 0, bounds.right, thumb.GetHeight()));
			thumb = AddMargins(thumb, 1, 0, 1, 0 , RGBAZero());
			s.hthumb[status] = Hot3(thumb);
		}
	}

	ColoredOverride(CtrlsImg::Iml(), CtrlsImg::Iml());
	
	{
		
		Color e = GetInkColor(Coco_ThemeImage(SColorFace(), 100, 50, 10, COCO_COMBOBOX));
		CtrlsImg::Set(CtrlsImg::I_EFE,
		    WithHotSpots(AddMargins(CreateImage(Size(1, 1), SColorPaper()), 1, 1, 1, 1, e),
		                 1, 1, 0, 0));

		CtrlsImg::Set(CtrlsImg::I_VE,
		    WithHotSpots(AddMargins(CreateImage(Size(3, 3), SColorPaper()), 1, 1, 1, 1, e),
		                 2, 2, 0, 0));

		MultiButton::Style& s = MultiButton::StyleDefault().Write();
//		s.trivialsep = true;
//		s.edge[0] = Null;
		s.clipedge = true;
		s.border = s.trivialborder = 0;


		for(int i = CTRL_NORMAL; i <= CTRL_DISABLED; i++) {
			Image m = Coco_ThemeImage(150, button_height + 1, 0, COCO_BUTTON, 1, i); // TODO: ChWithOffset...
			Size isz = m.GetSize();
			int x3 = isz.cx / 3;
			s.left[i] = Hot3(Crop(m, 0, 0, x3, isz.cy));
			s.trivial[i] = s.look[i] = s.right[i] = Hot3(Crop(m, 2 * x3, 0, isz.cx - 2 * x3, isz.cy));
			
			Image mm = Crop(m, x3, 0, x3, isz.cy);
			s.lmiddle[i] = Hot3(AddMargins(mm, 1, 0, 0, 0, SColorPaper()));
			s.rmiddle[i] = Hot3(AddMargins(mm, 0, 0, 1, 0, SColorPaper()));
			
			s.monocolor[i] = s.fmonocolor[i] = Button::StyleOk().monocolor[i];
			
		}
	}
	{
		SpinButtons::Style& sp = SpinButtons::StyleDefault().Write();
		sp.dec = sp.inc = Button::StyleNormal();
		
		for(int i = CTRL_NORMAL; i <= CTRL_DISABLED; i++) {
			Image m = AutoCrop(Coco_ThemeImage(50, button_height + 1, 0, COCO_BUTTON, 0, i));
			Size isz = m.GetSize();
			int cy = isz.cy / 2;
			Color c = Button::StyleNormal().monocolor[i];
			sp.inc.look[i] = ChLookWith(Hot3(Crop(m, 0, 0, isz.cx, cy)), CtrlImg::spinup(), c);
			sp.dec.look[i] = ChLookWith(Hot3(Crop(m, 0, cy, isz.cx, isz.cy - cy)), CtrlImg::spindown(), c);
		}
	}

	auto nsimg = [](int ii) { return AutoCrop(Coco_ThemeImage(48, 48, 10, COCO_NSIMAGE, ii)); };
	CtrlImg::Set(CtrlImg::I_information, nsimg(1));
	CtrlImg::Set(CtrlImg::I_question, nsimg(0));
	CtrlImg::Set(CtrlImg::I_exclamation, nsimg(0));
	CtrlImg::Set(CtrlImg::I_error, nsimg(0));
	
//	DDUMP(Coco_ThemeColor(1));

//	Coco_ThemeColor(1));
}

};

#endif