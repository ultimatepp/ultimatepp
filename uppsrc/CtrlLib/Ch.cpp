#include "CtrlLib.h"

namespace Upp {

#define LLOG(x)  // RLOG(x)

#define IMAGECLASS CtrlsImg
#define IMAGEFILE <CtrlLib/Ctrls.iml>
#include <Draw/iml_source.h>

#define IMAGECLASS ClassicCtrlsImg
#define IMAGEFILE <CtrlLib/ClassicCtrls.iml>
#include <Draw/iml_source.h>

void ChSysInit();

void ChStdSkin()
{
	ChSysInit();
	GUI_GlobalStyle_Write(GUISTYLE_XP);
	GUI_PopUpEffect_Write(Ctrl::IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
	ColoredOverride(CtrlsImg::Iml(), CtrlsImg::Iml());
}

void SbWc(Value *look)
{
	Color wc = Blend(SColorFace(), SColorPaper(), 170);
	look[CTRL_NORMAL] = wc;
	look[CTRL_HOT] = wc;
	look[CTRL_PRESSED] = SColorText();
	look[CTRL_DISABLED] = wc;
}

void ChClassicSkin()
{
	LLOG("ChInitWinClassic");

	ChSysInit();
	GUI_GlobalStyle_Write(GUISTYLE_CLASSIC);
	GUI_PopUpEffect_Write(Ctrl::IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
	
	ColoredOverride(CtrlsImg::Iml(), ClassicCtrlsImg::Iml());
	for(int q = 0; q < 4; q++)
		CtrlsImg::Set(CtrlsImg::I_HTB + q, AdjustColors(CtrlsImg::Get(ClassicCtrlsImg::I_B + q)));

	{
		Button::Style& s = Button::StyleNormal().Write();
		s.monocolor[0] = s.monocolor[1] = s.monocolor[2] = s.monocolor[3] = SColorText();
		s.pressoffset.x = s.pressoffset.y = 1;
		s.transparent = false;
	}

	{
		ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
		SbWc(s.hupper);
		SbWc(s.hlower);
		SbWc(s.vupper);
		SbWc(s.vlower);
		for(int i = 0; i < 4; i++)
			s.vthumb[i] = s.hthumb[i] = Button::StyleNormal().look[i];
	}

	{
		MultiButton::Style& s = MultiButton::StyleDefault().Write();
		s.border = s.trivialborder = 2;
	}

	{
		SeparatorCtrl::Style& s = SeparatorCtrl::StyleDefault().Write();
		s.l1 = SColorShadow();
		s.l2 = SColorLight();
	}

	{
		MenuBar::Style& s = MenuBar::StyleDefault().Write();
		s.popupbody = SColorFace();
	}

//	LabelBoxTextColor_Write(SColorText());
}

#ifdef PLATFORM_X11

void ChSysInit()
{
	CtrlImg::Reset();
	CtrlsImg::Reset();
	ChReset();
}

#endif

void FillImage(Painter& p, const Rectf& r, const Image& m)
{
	Xform2D xform = Xform2D::Translation(r.left, r.top);
	Size isz = m.GetSize();
	xform = Xform2D::Scale(r.GetWidth() / isz.cx, r.GetHeight() / isz.cy) * xform;
	p.Fill(m, xform);
}

void RoundedRect(Painter& w, double x, double y, double cx, double cy, double rx, double ry, dword corners)
{
	if(corners & CORNER_TOP_LEFT)
		w.Move(x + rx, y).Arc(x + rx, y + ry, rx, ry, -M_PI / 2, -M_PI / 2);
	else
		w.Move(x, y);
	
	if(corners & CORNER_BOTTOM_LEFT)
		w.Line(x, y + cy - ry).Arc(x + rx, y + cy - ry, rx, ry, M_PI, -M_PI / 2);
	else
		w.Line(x, y + cy);

	if(corners & CORNER_BOTTOM_RIGHT)
		w.Line(x + cx - rx, y + cy).Arc(x + cx - rx, y + cy - ry, rx, ry, M_PI / 2, -M_PI / 2);
	else
		w.Line(x + cx, y + cy);
	
	if(corners & CORNER_TOP_RIGHT)
		w.Line(x + cx, y + ry).Arc(x + cx - rx, y + ry, rx, ry, 0, -M_PI / 2);
	else
		w.Line(x + cx, y);

	w.Close();
}

void RoundedRect(Painter& w, Rectf r, double rx, double ry, dword corner)
{
	RoundedRect(w, r.left, r.top, r.GetWidth(), r.GetHeight(), rx, ry, corner);
}

Image MakeButton(int radius, const Image& face, int border_width, Color border_color, dword corner)
{
	int q = radius + border_width + 16;
	Rectf r(0, 0, q, q);
	ImagePainter w(r.GetSize());
	w.Clear(RGBAZero());
	RoundedRect(w, r.Deflated(border_width - 1), radius, radius, corner);
	FillImage(w, r.Deflated(border_width - 1), face);
	RoundedRect(w, r.Deflated(border_width / 2.0), radius, radius, corner);
	w.Stroke(border_width, border_color);
	Image m = w;
	Point p1(radius, radius);
	SetHotSpots(m, p1, (Point)r.BottomRight() - p1);
	return m;
}

}
