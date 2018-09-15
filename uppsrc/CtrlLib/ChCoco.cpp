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
	return Hot3(MirrorVert(iw));
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
		s.textcolor[i] = i == CTRL_DISABLED ? SColorDisabled()
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

void ChHostSkin()
{
	CtrlImg::Reset();
	CtrlsImg::Reset();
	ChReset();

	GUI_GlobalStyle_Write(GUISTYLE_XP);


	DUMP(Coco_Metric(0)); // kThemeMetricScrollBarWidth
	DUMP(Coco_Metric(7)); // kThemeMetricFocusRectOutset
	DUMP(Coco_Metric(19)); // kThemeMetricPushButtonHeight
	

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
			s.vthumb[status] = Hot3(thumb);

			s.hupper[status] = s.hlower[status] =
				Hot3(Coco_ThemeImage(SColorFace(), 40, s.barsize, 0, COCO_SCROLLTRACK, 1, status));
			thumb = Coco_ThemeImage(50, s.barsize, 0, COCO_SCROLLTHUMB, 1, status);
			bounds = FindBounds(thumb);
			thumb = Crop(thumb, Rect(bounds.left, 0, bounds.right, thumb.GetHeight()));
			s.hthumb[status] = Hot3(thumb);
		}
	}
	
//	DDUMP(Coco_ThemeColor(1));

//	Coco_ThemeColor(1));
}

};

#endif