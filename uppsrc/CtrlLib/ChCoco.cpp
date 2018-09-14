#include "ChCoco.h"

#ifdef PLAFORM_COCOA

namespace Upp {
	
Image Hot3(const Image& m)
{
	Size sz = m.GetSize();
	return WithHotSpots(m, sz.cx / 3, sz.cy / 3, sz.cx - sz.cx / 3, sz.cy - sz.cy / 3);
}

Image Coco_ThemeImage(Color bg, int cx, int cy, int margin, int type,
                      int value = 0, int state = CTRL_NORMAL, bool focus = false)
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

void CocoButton(Button::Style& s, int value)
{
	Image h[4];
	h[0] = Coco_ThemeImage(DPI(40), DPI(32), 10, COCO_BUTTON, value, CTRL_NORMAL);
	h[1] = Coco_ThemeImage(DPI(40), DPI(32), 10, COCO_BUTTON, value, CTRL_HOT); // same as Normal
	h[2] = Coco_ThemeImage(DPI(40), DPI(32), 10, COCO_BUTTON, value, CTRL_PRESSED);
	h[3] = Coco_ThemeImage(DPI(40), DPI(32), 10, COCO_BUTTON, value, CTRL_DISABLED);

	AutoCrop(h, 4);

	Size sz = h[0].GetSize();
	for(int i = 0; i < 4; i++) {
		Image img = Hot3(h[i]);
		s.look[i] = img;
		Image gg = CreateImage(h[i].GetSize(), SColorFace());
		Over(gg, h[i]);
		s.textcolor[i] = i == CTRL_DISABLED ? SColorDisabled()
		                 : Grayscale(AvgColor(gg, img.GetSize().cy / 3)) > 160 ? SColorText()
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

	CocoButton(Button::StyleNormal().Write(), 0);
	CocoButton(Button::StyleOk().Write(), 1);
	
	{
		Color menuink = CocoColor(COCO_SELECTEDMENUTEXT);
		SColorMenu_Write(AvgColor(AutoCrop(Coco_ThemeImage(30, 20, 10, COCO_MENU, 0, CTRL_NORMAL))));
		SColorMenuText_Write(SColorText());

		MenuBar::Style& s = MenuBar::StyleDefault().Write();
		s.pullshift.y = 0;
		
		SColorMenu_Write(SColorFace());
		
//		s.popupframe = WithHotSpot(mimg, m, m);
//		s.popupbody = Crop(mimg, m, m, 32 - 2 * m, 32 - 2 * m);
//		s.leftgap = DPI(16) + Zx(6);
//		s.itemtext = ChGtkColor(2, menu_item);
//		s.menutext = SColorMenuText();

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
//		s.breaksep.l1 = Color(img[31][15]);
//		s.breaksep.l2 = Null;
	}
	
//	DDUMP(Coco_ThemeColor(1));

//	Coco_ThemeColor(1));
}

};

#endif