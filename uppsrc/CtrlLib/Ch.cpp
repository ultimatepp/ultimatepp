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

Image MakeElement(Size sz, double radius, const Image& face, double border_width, Color border_color, Event<Painter&, const Rectf&> shape)
{
	Rectf r(0, 0, sz.cx, sz.cy);
	ImagePainter w(r.GetSize());
	w.Clear(RGBAZero());
	Rectf dr = r.Deflated(border_width / 2.0);
	if(!IsNull(face)) {
		shape(w, dr);
		FillImage(w, r.Deflated(border_width / 2.0 - 1), face);
	}
	shape(w, dr);
	if(!IsNull(border_color))
		w.Stroke(border_width, border_color);
	Image m = w;
	Point p1(int(radius + border_width), int(radius + border_width));
	SetHotSpots(m, p1, (Point)r.BottomRight() - p1 - Point(1, 1));
	return m;
}

Image MakeButton(int radius, const Image& face, double border_width, Color border_color, dword corner)
{
	double q = radius + border_width + DPI(16);
	return MakeElement(Size(q, q), radius, face, border_width, border_color, [&](Painter& w, const Rectf& r) {
		RoundedRect(w, r, radius, radius, corner);
	});
}

Image MakeButton(int radius, Color face, double border_width, Color border_color, dword corner)
{
	return MakeButton(radius, CreateImage(Size(DPI(10), DPI(5)), face), border_width, border_color, corner);
}

Image Hot3(const Image& m)
{
	Size sz = m.GetSize();
	return WithHotSpots(m, sz.cx / 3, sz.cy / 3, sz.cx - sz.cx / 3, sz.cy - sz.cy / 3);
}

Image ChHot(const Image& m, int n)
{
	return WithHotSpots(m, DPI(n), DPI(n), 0, 0);
}

Color AvgColor(const Image& m, const Rect& rr)
{
	int r = 0;
	int g = 0;
	int b = 0;
	int n = 0;
	for(int y = rr.top; y < rr.bottom; y++)
		for(int x = rr.left; x < rr.right; x++) {
			RGBA c = m[y][x];
			Unmultiply(&c, &c, 1);
			if(c.a > 20) {
				r += c.r;
				g += c.g;
				b += c.b;
				n++;
			}
		}
	return n ? Color(r / n, g / n, b / n) : SWhite();
}

Color AvgColor(const Image& m, int margin)
{
	return AvgColor(m, Rect(m.GetSize()).Deflated(margin));
}

Color GetInk(const Image& m)
{
	RGBA avg = AvgColor(m);
	Color ink = SBlack();
	int   best = 0;
	for(RGBA c : m) {
		Unmultiply(&c, &c, 1);
		if(c.a > 100) {
			c.a = 255;
			int q = Grayscale(abs(c.r - avg.r), abs(c.g - avg.g), abs(c.b - avg.b));
			if(q > best) {
				best = q;
				ink = c;
			}
		}
	}
	return ink;
}

int GetRoundness(const Image& m)
{
	Size isz = m.GetSize();
	int bestd = 0, besth = 0;
	int di = 0, hi = 0;
	int hy = isz.cy / 2;
	RGBA avg = AvgColor(m);
	auto Chk = [&](int x, int y, int& best, int& besti) {
		if(x < isz.cx && y < isz.cy) {
			RGBA c = m[y][x];
			Unmultiply(&c, &c, 1);
			if(c.a > 100) {
				int q = Grayscale(abs(c.r - avg.r), abs(c.g - avg.g), abs(c.b - avg.b));
				if(q > best) {
					best = q;
					besti = x;
				}
			}
		}
	};
	for(int i = 0; i < 8; i++) {
		Chk(i, hy, besth, hi);
		Chk(i, i, bestd, di);
	}
	return max(di - hi, 0);
}

static Value sSample;

void SetChameleonSample(const Value& m, bool once)
{
	if(!once || IsNull(sSample))
		sSample = m;
}

Value GetChameleonSample()
{
	return sSample;
}

Image WithRect(Image m, int x, int y, int cx, int cy, Color c)
{
	ImageBuffer ib(m);
	for(int i = 0; i < cx; i++)
		for(int j = 0; j < cy; j++)
			ib[y + j][x + i] = c;
	return ib;
}

Image WithLeftLine(const Image& m, Color c, int w)
{
	return WithRect(m, 0, 0, w, m.GetHeight(), c);
}

Image WithRightLine(const Image& m, Color c, int w)
{
	return WithRect(m, m.GetWidth() - w, 0, w, m.GetHeight(), c);
}

Image WithTopLine(const Image& m, Color c, int w)
{
	return WithRect(m, 0, 0, m.GetWidth(), w, c);
}

Image WithBottomLine(const Image& m, Color c, int w)
{
	return WithRect(m, 0, m.GetHeight() - w, m.GetWidth(), w, c);
}

void ChSynthetic(Image button100x100[4], Color text[4])
{
	int roundness = DPI(3);
	Color ink = SColorText();
	for(int i = 0; i < 4; i++) {
		Image m = button100x100[i];
		if(i == 0) {
			ink = GetInk(m);
			roundness = GetRoundness(m) ? DPI(3) : 0;
			CtrlsImg::Set(CtrlsImg::I_EFE, WithHotSpots(MakeButton(roundness, SColorPaper(), DPI(1), ink), DPI(3), DPI(1), 0, 0));
			CtrlsImg::Set(CtrlsImg::I_VE, WithHotSpots(MakeButton(DPI(0), SColorPaper(), DPI(1), ink), DPI(2), DPI(2), 0, 0));
			EditField::StyleDefault().Write().edge[CTRL_DISABLED] = WithHotSpots(MakeButton(roundness, SColorFace(), DPI(1), ink), DPI(3), DPI(1), 0, 0);
			EditField::StyleDefault().Write().activeedge = true;
			LabelBox::SetLook(WithHotSpots(MakeButton(2 * roundness / 3, Image(), DPI(1), ink), DPI(3), DPI(3), 0, 0));
		}
		Size sz = m.GetSize();
		m = Crop(m, sz.cx / 8, sz.cy / 8, 6 * sz.cx / 8, 6 * sz.cy / 8);
		{
			auto Set = [&](Button::Style& s, const Image& arrow = Null) {
				Value l = MakeButton(0, m, DPI(1), ink, 0);
				s.look[i] = IsNull(l) ? l : ChLookWith(l, arrow);
			};
			Set(Button::StyleScroll().Write());
			Set(Button::StyleEdge().Write());
			Set(Button::StyleLeftEdge().Write());
			ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
			
			Set(s.up, CtrlsImg::UA());
			Set(s.down, CtrlsImg::DA());
			Set(s.left, CtrlsImg::LA());
			Set(s.right, CtrlsImg::RA());
		}
		{
			MultiButton::Style& s = MultiButton::StyleDefault().Write();
			s.clipedge = true;
			s.border = s.trivialborder = 0;

			s.left[i] = MakeButton(roundness, m, DPI(1), ink, CORNER_TOP_LEFT|CORNER_BOTTOM_LEFT);
			s.trivial[i] = s.look[i] = s.right[i] = MakeButton(roundness, m, DPI(1), ink, CORNER_TOP_RIGHT|CORNER_BOTTOM_RIGHT);
			auto Middle = [&](Image m) {
				ImageBuffer ib(m);
				for(int y = 0; y < DPI(1); y++)
					for(int x = 0; x < ib.GetWidth(); x++) {
						ib[y][x] = ink;
						ib[ib.GetHeight() - y - 1][x] = ink;
					}
				return WithHotSpot(ib, DPI(1), DPI(1));
			};
			s.lmiddle[i] = Middle(WithRightLine(m, ink));
			s.rmiddle[i] = Middle(WithLeftLine(m, ink));
			s.monocolor[i] = s.fmonocolor[i] = text[i];
			for(int i = 0; i < 4; i++)
				s.edge[i] = EditField::StyleDefault().edge[i];
			s.margin = Rect(DPI(3), 2, DPI(1), 2);
			s.activeedge = true;
			s.stdwidth = DPI(17);
		}
		{
			SpinButtons::Style& sp = SpinButtons::StyleDefault().Write();
			if(i == 0)
				sp.dec = sp.inc = Button::StyleNormal();
			auto Spin = [&](dword corners, const Image& sm) {
				return ChLookWith(WithLeftLine(MakeButton(roundness, m, 0, Black(), corners), ink), sm, text[i]);
			};
			sp.inc.look[i] = Spin(CORNER_TOP_RIGHT, CtrlImg::spinup());
			sp.dec.look[i] = Spin(CORNER_BOTTOM_RIGHT, CtrlImg::spindown());
			sp.width = DPI(14); // DPI(17) - DPI(3) (droplist stdwidth minus frame)
		}
		{
			HeaderCtrl::Style& hs = HeaderCtrl::StyleDefault().Write();
			hs.look[i] = ChHot(WithBottomLine(WithRightLine(m, ink, 1), ink));
		}
		if(i == CTRL_DISABLED) {
			ProgressIndicator::Style& s = ProgressIndicator::StyleDefault().Write();
			s.hlook = MakeButton(roundness, m, DPI(1), ink);
			s.hchunk = MakeButton(roundness, SColorHighlight(), DPI(1), ink);
			s.bound = true;
			s.nomargins = true;
		}
		if(i == CTRL_NORMAL || i == CTRL_PRESSED) {
			Image sm = MakeElement(Size(DPI(10), DPI(20)), roundness, m, DPI(1), ink, [&](Painter& w, const Rectf& r) {
				double cx = r.GetWidth();
				double cy = r.GetHeight();
				double uy = 0.4 * cy;
				double uq = 0.5 * uy;
				w.Move(r.left, r.top + cy)
				 .Line(r.left, r.top + uy)
				 .Quadratic(r.left, r.top + uq, r.left + cx / 2, r.top)
				 .Quadratic(r.left + cx, r.top + uq, r.left + cx, r.top + uy)
				 .Line(r.left + cx, r.top + cy)
				 .Close();
			});
			CtrlImg::Set(i == CTRL_PRESSED ? CtrlImg::I_hthumb1 : CtrlImg::I_hthumb, sm);
			CtrlImg::Set(i == CTRL_PRESSED ? CtrlImg::I_vthumb1 : CtrlImg::I_vthumb, RotateClockwise(sm));
		}
		{
			TabCtrl::Style& s = TabCtrl::StyleDefault().Write();
			auto Face = [](int adj) {
				Color c = SColorFace();
				return Color(clamp(c.GetR() + adj, 0, 255),
				             clamp(c.GetG() + adj, 0, 255),
				             clamp(c.GetB() + adj, 0, 255));
			};
			s.body = MakeButton(0, Face(8), DPI(1), ink);
			Image t = MakeButton(roundness, Face(decode(i, CTRL_NORMAL, -20,
			                                               CTRL_HOT, 2,
			                                               CTRL_PRESSED, 8,
			                                               -8)), DPI(1), ink, CORNER_TOP_LEFT|CORNER_TOP_RIGHT);
			s.first[i] = s.last[i] = s.both[i] = s.normal[i] = ChHot(Crop(t, 0, 0, t.GetWidth(), t.GetHeight() - DPI(3)), DPI(3));
			s.margin = 0;
			s.sel = Rect(0, DPI(1), 0, DPI(1));
			s.extendleft = DPI(2);
			s.text_color[i] = SColorText();
		}
	}
}

void ChBaseSkin()
{
	ChSysInit();
	GUI_GlobalStyle_Write(GUISTYLE_XP);
	GUI_PopUpEffect_Write(Ctrl::IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
	ColoredOverride(CtrlsImg::Iml(), CtrlsImg::Iml());
}

void ChStdSkin()
{
	ColoredOverride(CtrlsImg::Iml(), CtrlsImg::Iml());

	for(int i = 0; i < 6; i++)
		CtrlsImg::Set(CtrlsImg::I_DA + i, CtrlsImg::Get(CtrlsImg::I_kDA + i));
		
	int c = DPI(14);

	Color text[4];
	Image button[4], sbutton[4];
	auto Adjust = [](Color c, int adj) {
		return Color(clamp(c.GetR() + adj, 0, 255),
		             clamp(c.GetG() + adj, 0, 255),
		             clamp(c.GetB() + adj, 0, 255));
	};
	
	Color border = Gray();
		
	{
		for(int pass = 0; pass < 2; pass++) {
			Button::Style& s = pass ? Button::StyleOk().Write() : Button::StyleNormal().Write();
			int roundness = DPI(3);
			for(int i = 0; i < 4; i++) {
				static int adj[] = { 10, 80, -5, -10 };
				Color f = Adjust(SColorFace(), adj[i]);
				Color ink = i == CTRL_DISABLED ? SColorDisabled() : SColorText();
				s.look[i] = MakeButton(roundness, f, DPI(1 + pass), border);
				text[i] = s.monocolor[i] = s.textcolor[i] = ink;
				if(pass == 0) {
					sbutton[i] = MakeButton(DPI(3), f, DPI(1), border);
					button[i] = MakeButton(DPI(1), f, DPI(1), border);
					{
						for(int opt = 0; opt < 2; opt++) {
							ImagePainter p(c, c);
							p.Scale(DPI(1));
							p.Clear(RGBAZero());
							p.Circle(7, 7, 6).Fill(f).Stroke(1, border);
							if(opt)
								p.Circle(7, 7, 4).Fill(ink);
							CtrlsImg::Set((opt ? CtrlsImg::I_S1 : CtrlsImg::I_S0) + i, p);
						}
					}
					{
						for(int chk = 0; chk < 3; chk++) {
							ImagePainter p(c, c);
							p.Scale(DPI(1));
							p.Clear(RGBAZero());
							p.Rectangle(1, 1, 12, 12).Fill(f).Stroke(1, border);
							if(chk == 1)
								p.Move(3, 7).Line(7, 10).Line(11, 4).Stroke(2, ink);
							if(chk == 2)
								p.Rectangle(3, 6, 8, 2).Fill(ink);
							CtrlsImg::Set(decode(chk, 0, CtrlsImg::I_O0, 1, CtrlsImg::I_O1, CtrlsImg::I_O2) + i, p);
						}
					}
				}
			}
		}

		ChSynthetic(sbutton, text);

		{
			auto& s = ToolButton::StyleDefault().Write();
			s.look[CTRL_NORMAL] = Image();
			s.look[CTRL_HOT] = button[CTRL_HOT];
			s.look[CTRL_PRESSED] = button[CTRL_PRESSED];
			s.look[CTRL_DISABLED] = Image();
			s.look[CTRL_CHECKED] = button[CTRL_PRESSED];
			s.look[CTRL_HOTCHECKED] = button[CTRL_HOT];
		}

		CtrlImg::Set(CtrlImg::I_MenuCheck0, CtrlsImg::O0());
		CtrlImg::Set(CtrlImg::I_MenuCheck1, CtrlsImg::O1());
		CtrlImg::Set(CtrlImg::I_MenuRadio0, CtrlsImg::S0());
		CtrlImg::Set(CtrlImg::I_MenuRadio1, CtrlsImg::S1());
	}

	{
		ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
		ImagePainter p(c, c);
		p.Rectangle(0, 0, c, c).Fill(0, 0, SColorFace(), c, 0, SColorPaper());
		Image vtrough = p;

		for(int status = CTRL_NORMAL; status <= CTRL_DISABLED; status++) {
			s.hupper[status] = s.hlower[status] = ChHot(RotateClockwise(vtrough));
			s.vupper[status] = s.vlower[status] = ChHot(vtrough); // we have problems getting this right for vertical
			static int adj[] = { 20, 40, 10, -10 };
			s.hthumb[status] = s.vthumb[status] = Adjust(border, adj[status]);
		}
	}
	
	{
		MenuBar::Style& s = MenuBar::StyleDefault().Write();
		s.topitem[1] = Blend(SColorHighlight(), SColorPaper());
	}
	GUI_PopUpEffect_Write(Ctrl::IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
}

#ifdef GUI_X11

void ChHostSkin()
{
	int h = Ctrl::GetPrimaryScreenArea().Height();
	Font::SetDefaultFont(Arial(h > 1300 ? 26 : h > 800 ? 14 : 12));
	SColorFace_Write(Color(242, 241, 240));
	SColorMenu_Write(Color(242, 241, 240));
	SColorHighlight_Write(Color(50, 50, 250));

	ChStdSkin();
}

#endif

}
