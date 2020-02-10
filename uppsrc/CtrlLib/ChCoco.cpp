#include "CtrlLib.h"
#include "ChCocoMM.h"

#ifdef PLATFORM_COCOA

namespace Upp {

Image CocoImg(Color bg, int type, int value, int state)
{
	Size isz(140, 140);
	ImageDraw iw(DPI(isz));
	iw.DrawRect(0, 0, DPI(isz.cx), DPI(isz.cy), bg);
	Coco_PaintCh(iw.GetCGHandle(), type, value, state);
	return iw;
}

Image CocoImg(int type, int value = 0, int state = 0)
{
	Image m[2];
	for(int i = 0; i < 2; i++)
		m[i] = CocoImg(i ? Black() : White(), type, value, state);
	Image h = AutoCrop(RecreateAlpha(m[0], m[1]));
	int q = h.GetSize().cy / 4;
	SetHotSpots(h, Point(q, q));
	return h;
}

Color CocoColor(int k, Color bg = SColorFace())
{
	return AvgColor(CocoImg(bg, COCO_NSCOLOR, k, 0));
}

void SOImages(int imli, int type, int value)
{
	Image h[4];
	h[0] = CocoImg(type, value, CTRL_NORMAL);
	h[1] = CocoImg(type, value, CTRL_HOT); // same as Normal
	h[2] = CocoImg(type, value, CTRL_PRESSED);
	h[3] = CocoImg(type, value, CTRL_DISABLED);
	for(int i = 0; i < 4; i++)
		CtrlsImg::Set(imli++, Hot3(h[i]));
}

void CocoButton(Image *h, int type, int value)
{
	h[0] = CocoImg(type, value, CTRL_NORMAL);
	h[1] = CocoImg(type, value, CTRL_HOT); // same as Normal
	h[2] = CocoImg(type, value, CTRL_PRESSED);
	h[3] = CocoImg(type, value, CTRL_DISABLED);
}

void CocoButton(Button::Style& s, int type, int value)
{
	Image h[4];
	CocoButton(h, type, value);
	for(int i = 0; i < 4; i++) {
		s.look[i] = h[i];
		Image gg = CreateImage(h[i].GetSize(), SColorFace());
		Over(gg, h[i]);
		s.monocolor[i] = s.textcolor[i] = i == CTRL_DISABLED ? SColorDisabled()
		                 : Grayscale(AvgColor(gg, h[i].GetSize().cy / 3)) > 160 ? Black()
		                                                                        : White();
	}
	s.overpaint = 5;
	s.pressoffset = Point(0, 0);
}

void ChHostSkin()
{
	TIMING("ChHostSkin");

	CtrlImg::Reset();
	CtrlsImg::Reset();
	ChReset();

	GUI_GlobalStyle_Write(GUISTYLE_XP);

	SwapOKCancel_Write(true);
	SColorFace_Write(CocoColor(COCO_WINDOW, White())); // ThemeBrushDialogBackgroundActive
	SColorHighlight_Write(CocoColor(COCO_SELECTEDPAPER));
	SColorHighlightText_Write(CocoColor(COCO_SELECTEDTEXT));
	SColorText_Write(CocoColor(COCO_TEXT));
	SColorPaper_Write(CocoColor(COCO_PAPER));
	SColorDisabled_Write(CocoColor(COCO_DISABLED));

	ColoredOverride(CtrlsImg::Iml(), CtrlsImg::Iml());

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
		CocoButton(h, COCO_BEVELBUTTON, 0);
		s.look[CTRL_NORMAL] = Image();
		s.look[CTRL_HOT] = h[CTRL_HOT];
		s.look[CTRL_PRESSED] = h[CTRL_PRESSED];
		s.look[CTRL_DISABLED] = Image();
		CocoButton(h, COCO_BEVELBUTTON, 1);
		s.look[CTRL_CHECKED] = h[CTRL_NORMAL];
		s.look[CTRL_HOTCHECKED] = h[CTRL_HOT];
	}

	{
		ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
		s.arrowsize = 0;
		Image track = CocoImg(COCO_SCROLLTRACK);
		Image thumb = CocoImg(COCO_SCROLLTHUMB);

		s.barsize = track.GetHeight();
		s.thumbwidth = thumb.GetHeight();
		s.thumbmin = 2 * s.barsize;

		for(int status = CTRL_NORMAL; status <= CTRL_DISABLED; status++) {
			s.hupper[status] = s.hlower[status] = WithHotSpot(track, CH_SCROLLBAR_IMAGE, 0);;
			s.vupper[status] = s.vlower[status] = WithHotSpot(RotateAntiClockwise(track), CH_SCROLLBAR_IMAGE, 0);
			Image m = thumb;
			if(status == CTRL_HOT)
				Over(m, m);
			if(status == CTRL_PRESSED) {
				Over(m, m);
				Over(m, m);
			}
			s.hthumb[status] = WithHotSpot(m, CH_SCROLLBAR_IMAGE, 0);
			s.vthumb[status] = WithHotSpot(RotateClockwise(m), CH_SCROLLBAR_IMAGE, 0);
		}
	}

	auto field = [](int type) {
		Image m = CocoImg(SColorFace(), type, 0, 0);
		Rect r = m.GetSize();
		r.left = r.top = DPI(5);
		r.right = min(r.left + DPI(10), r.right);
		r.bottom = min(r.top + DPI(5), r.bottom);
		return AvgColor(m, r);
	};
	{
		EditString::Style& s = EditString::StyleDefault().Write();
		s.focus = s.paper = field(COCO_TEXTFIELD);
	}
	{
		MultiButton::Style& s = MultiButton::StyleDefault().Write();
		s.paper = field(COCO_POPUPBUTTON);
	}
	
	auto nsimg = [](int ii) { return CocoImg(COCO_NSIMAGE, ii); };
	CtrlImg::Set(CtrlImg::I_information, nsimg(1));
	CtrlImg::Set(CtrlImg::I_question, nsimg(0));
	CtrlImg::Set(CtrlImg::I_exclamation, nsimg(0));
	CtrlImg::Set(CtrlImg::I_error, nsimg(0));
	
	Image button100x100[8];
	Color text[8];
	
	for(int i = 0; i < 8; i++) {
		ImageDraw iw(100, 100);
		const Button::Style& s = i < 4 ? Button::StyleNormal() : Button::StyleOk();
		ChPaint(iw, 0, 0, 100, 100, s.look[i & 3]);
		button100x100[i] = iw;
		text[i] = s.monocolor[i & 3];
	}
	
	ChSynthetic(button100x100, text, true);
}

};

#endif