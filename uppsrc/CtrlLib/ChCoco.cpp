#include "ChCoco.h"

#ifdef PLAFORM_COCOA

namespace Upp {

Image Coco_ThemeImage(int cx, int cy, int margin, int type, int value, int state, bool focus, Color bg)
{
	Size isz(cx + 2 * margin, cy + 2 * margin);
	ImageDraw iw(isz);
	iw.DrawRect(isz, bg);
	Coco_ThemePaint(iw.GetCGHandle(), Rect(margin, margin, cx, cy), type, value, state, focus);
	return iw;
}

Image Coco_ThemeImage(int cx, int cy, int margin, int type, int value, int state, bool focus)
{
	return MirrorVert(RecreateAlpha(Coco_ThemeImage(cx, cy, margin, type, value, state, focus, White()),
	                                Coco_ThemeImage(cx, cy, margin, type, value, state, focus, Black())));
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

Color AvgColor(const Image& m)
{
	return AvgColor(m, m.GetSize());
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
		CtrlsImg::Set(imli++, h[i]);
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
	Point p1 = sz / 3;
	Point p2 = sz - p1;
	for(int i = 0; i < 4; i++) {
		s.look[i] = WithHotSpots(h[i], p1.x, p1.y, p2.x, p2.y);
		Image gg = CreateImage(h[i].GetSize(), SColorFace());
		Over(gg, h[i]);
		s.textcolor[i] = i == CTRL_DISABLED ? SColorDisabled()
		                 : Grayscale(AvgColor(gg, Rect(p1, p2))) > 160 ? SColorText() : White();
	}
	s.overpaint = 5;
	s.pressoffset = Point(0, 0);
}

Color CocoBrush(int k)
{
	return AvgColor(Coco_ThemeImage(16, 16, 0, COCO_BRUSH, k));
}

Color CocoColor(int k)
{
	return AvgColor(Coco_ThemeImage(16, 16, 0, COCO_NSCOLOR, k));
}

void ChHostSkin()
{
	CtrlImg::Reset();
	CtrlsImg::Reset();
	ChReset();

/*
	DDUMP(Coco_Metric(0)); // kThemeMetricScrollBarWidth
	DDUMP(Coco_Metric(7)); // kThemeMetricFocusRectOutset
*/

	SColorFace_Write(CocoBrush(1)); // ThemeBrushDialogBackgroundActive
	SColorHighlight_Write(CocoColor(COCO_SELECTEDPAPER));
	SColorHighlightText_Write(CocoColor(COCO_SELECTEDTEXT));
	SColorText_Write(CocoColor(COCO_TEXT));
	SColorPaper_Write(CocoColor(COCO_PAPER));
//	SColorDisabled_Write(CocoColor(COCO_DISABLED)); // TODO...

	SOImages(CtrlsImg::I_S0, COCO_RADIOBUTTON, 0);
	SOImages(CtrlsImg::I_S1, COCO_RADIOBUTTON, 1);
	SOImages(CtrlsImg::I_O0, COCO_CHECKBOX, 0);
	SOImages(CtrlsImg::I_O1, COCO_CHECKBOX, 1);
	SOImages(CtrlsImg::I_O2, COCO_CHECKBOX, 2);

	SwapOKCancel_Write(true);

	CocoButton(Button::StyleNormal().Write(), 0);
	CocoButton(Button::StyleOk().Write(), 1);
	
//	DDUMP(Coco_ThemeColor(1));

//	Coco_ThemeColor(1));
}

};

#endif