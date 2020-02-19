#include "CtrlLib.h"

namespace Upp {

#define LLOG(x)  // RLOG(x)

#define FIXED_COLORS
#define IMAGECLASS CtrlsImg // contains some default definitions
#define IMAGEFILE <CtrlLib/Ctrls.iml>
#include <Draw/iml_source.h>

void ChSysInit();

Color AdjustColor(Color c, int adj)
{
	return Color(clamp(c.GetR() + adj, 0, 255),
	             clamp(c.GetG() + adj, 0, 255),
	             clamp(c.GetB() + adj, 0, 255));
};

Color FaceColor(int adj)
{
	return AdjustColor(SColorFace(), adj);
};

void SyntheticTab(int i, int roundness, Color ink)
{
	TabCtrl::Style& s = TabCtrl::StyleDefault().Write();
	s.body = MakeButton(0, FaceColor(8), DPI(1), ink);
	Image t = MakeButton(roundness, FaceColor(decode(i, CTRL_NORMAL, -20,
	                                                    CTRL_HOT, 2,
	                                                    CTRL_PRESSED, 8,
	                                                    -8)), DPI(1), ink,
	                                                    CORNER_TOP_LEFT|CORNER_TOP_RIGHT);
	s.first[i] = s.last[i] = s.both[i] = s.normal[i] = ChHot(Crop(t, 0, 0, t.GetWidth(), t.GetHeight() - DPI(3)), DPI(3));
	s.margin = 0;
	s.sel = Rect(0, DPI(1), 0, DPI(1));
	s.extendleft = DPI(2);
	s.text_color[i] = SColorText();
}

void MakeDialogIcons()
{
	auto MakeCircularIcon = [](Color c, const char *sign, double ssz = 0.6, Color sc = White()) -> Image
	{
		int sz = DPI(32);
		ImagePainter w(sz, sz);
		double r = 0.5 * sz;
		w.Clear(RGBAZero());
		w.Circle(r, r, r).Fill(Gray());
		w.Circle(r, r, r - DPI(1)).Fill(White());
		w.Circle(r, r, r - DPI(2)).Fill(Pointf(0.5 * r, 0.6 * r), Blend(White(), c), r, c);
		
		ssz *= sz;
		Rectf sr = RectfC((sz - ssz) / 2, (sz - ssz) / 2, ssz, ssz);
		Rectf br = GetSVGPathBoundingBox(sign);
		double scale = sr.GetHeight() / br.GetHeight();
		w.Translate(sr.left + (sr.Width() - br.Width() * scale) / 2, sr.top);
		w.Scale(scale);
		w.Translate(-br.TopLeft());
		w.Path(sign).Stroke(2, Black()).Fill(sc);
		
		return w;
	};

	static const char *s_information = "M28.75 28.31 Q32.63 28.31 35.28 31.00 Q37.94 33.69 37.94 37.50 Q37.94 41.31 35.25 44.00 Q32.56 46.69 28.75 46.69 Q25.00 46.69 22.31 44.00 Q19.63 41.31 19.63 37.50 Q19.63 33.69 22.28 31.00 Q24.94 28.31 28.75 28.31 ZM32.63 57.06 L19.81 101.50 Q18.75 105.31 18.75 106.50 Q18.75 107.19 19.31 107.78 Q19.88 108.38 20.50 108.38 Q21.56 108.38 22.63 107.44 Q25.44 105.13 29.38 99.06 L31.50 100.31 Q22.06 116.75 11.44 116.75 Q7.38 116.75 4.97 114.47 Q2.56 112.19 2.56 108.69 Q2.56 106.38 3.63 102.81 L12.31 72.94 Q13.56 68.63 13.56 66.44 Q13.56 65.06 12.38 64.00 Q11.19 62.94 9.13 62.94 Q8.19 62.94 6.88 63.00 L7.69 60.50 L28.88 57.06 Z";
	CtrlImg::Set(CtrlImg::I_information, MakeCircularIcon(Color(85, 127, 200), s_information, 0.6, Color(243, 255, 211)));
	static const char *s_question = "M15.81 79.13 Q17.56 81.00 18.63 83.16 Q19.69 85.31 19.69 87.50 Q19.69 89.00 19.13 90.28 Q18.56 91.56 17.59 92.50 Q16.63 93.44 15.31 93.97 Q14.00 94.50 12.44 94.50 Q11.00 94.50 9.69 94.03 Q8.38 93.56 7.31 92.63 Q6.25 91.69 5.63 90.31 Q5.00 88.94 5.00 87.06 Q5.00 84.25 6.59 81.09 Q8.19 77.94 11.28 75.31 Q14.38 72.69 19.00 70.94 Q23.63 69.19 29.63 69.19 Q35.56 69.19 40.16 70.94 Q44.75 72.69 47.91 75.59 Q51.06 78.50 52.69 82.25 Q54.31 86.00 54.31 90.06 Q54.31 98.06 47.88 105.06 Q45.25 107.94 41.94 110.56 Q38.63 113.19 35.72 116.06 Q32.81 118.94 30.84 122.34 Q28.88 125.75 28.88 130.13 Q28.88 131.00 28.97 132.16 Q29.06 133.31 29.25 134.56 Q28.25 134.94 27.09 135.09 Q25.94 135.25 24.88 135.25 Q23.88 131.31 23.88 127.94 Q23.88 124.69 24.69 122.06 Q25.50 119.44 26.78 117.28 Q28.06 115.13 29.66 113.28 Q31.25 111.44 32.88 109.75 Q34.81 107.63 36.50 105.63 Q38.19 103.63 39.41 101.31 Q40.63 99.00 41.34 96.22 Q42.06 93.44 42.06 89.81 Q42.06 85.56 41.06 82.59 Q40.06 79.63 38.25 77.75 Q36.44 75.88 33.91 75.03 Q31.38 74.19 28.25 74.19 Q24.50 74.19 21.19 75.38 Q17.88 76.56 15.81 79.13 ZM27.19 161.75 Q25.38 161.75 23.84 161.13 Q22.31 160.50 21.13 159.38 Q19.94 158.25 19.28 156.69 Q18.63 155.13 18.63 153.25 Q18.63 151.38 19.31 149.78 Q20.00 148.19 21.16 147.06 Q22.31 145.94 23.88 145.28 Q25.44 144.63 27.25 144.63 Q29.00 144.63 30.56 145.25 Q32.13 145.88 33.31 147.00 Q34.50 148.13 35.16 149.69 Q35.81 151.25 35.81 153.13 Q35.81 155.00 35.16 156.59 Q34.50 158.19 33.31 159.34 Q32.13 160.50 30.56 161.13 Q29.00 161.75 27.19 161.75 Z";
	CtrlImg::Set(CtrlImg::I_question, MakeCircularIcon(Color(85, 127, 200), s_question));
	static const char *s_exclamation = "M22.75 89.44 L20.00 89.44 Q19.56 82.31 17.13 72.19 L13.88 58.56 Q10.94 46.31 10.94 41.00 Q10.94 36.19 13.81 33.25 Q16.69 30.31 21.31 30.31 Q25.81 30.31 28.78 33.28 Q31.75 36.25 31.75 40.81 Q31.75 45.56 28.63 58.56 L25.31 72.19 Q23.56 79.44 22.75 89.44 ZM21.44 98.38 Q25.63 98.38 28.59 101.34 Q31.56 104.31 31.56 108.56 Q31.56 112.75 28.59 115.72 Q25.63 118.69 21.44 118.69 Q17.25 118.69 14.28 115.72 Q11.31 112.75 11.31 108.56 Q11.31 104.38 14.25 101.38 Q17.19 98.38 21.44 98.38 Z";
	CtrlImg::Set(CtrlImg::I_exclamation, MakeCircularIcon(Red(), s_exclamation));
	static const char *s_error = "M51.63 64.00 L22.00 34.38 L22.00 22.00 L34.38 22.00 L64.00 51.63 L94.00 22.00 L106.00 22.00 L106.00 34.38 L76.38 64.00 L106.00 93.63 L106.00 106.00 L94.00 106.00 L64.00 76.38 L34.38 106.00 L22.00 106.00 L22.00 94.00 Z";
	CtrlImg::Set(CtrlImg::I_error, MakeCircularIcon(Red(), s_error, 0.5, Yellow()));
}

void  DrawClassicButton(Draw& w, const Rect& r_, Color tl1, Color br1, Color tl2, Color br2, Color face)
{
	Rect r = r_;
	DrawFrame(w, r, tl1, br1);
	r.Deflate(1);
	DrawFrame(w, r, tl2, br2);
	r.Deflate(1);
	w.DrawRect(r, face);
}

Image MakeClassicButton(Color tl1, Color br1, Color tl2, Color br2, Color face)
{
	Rect r = Size(8, 8);
	ImageDraw iw(r.GetSize());
	DrawClassicButton(iw, r, tl1, br1, tl2, br2, face);
	return WithHotSpot(iw, 2, 2);
}

void ChClassicSkin()
{
	LLOG("ChInitWinClassic");

	ChSysInit();
	GUI_GlobalStyle_Write(GUISTYLE_CLASSIC);
	GUI_PopUpEffect_Write(Ctrl::IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
	
	Color grayface = Blend(SColorFace(), Gray());

	Image edge = IsDarkTheme() ? MakeClassicButton(grayface, White(), Gray(), LtGray(), grayface)
	                           : MakeClassicButton(Gray(), White(), Black(), LtGray(), SColorFace());
	CtrlsImg::Set(CtrlsImg::I_EFE, edge);
	CtrlsImg::Set(CtrlsImg::I_VE, edge);

	Color wg = Blend(SColorFace(), WhiteGray());
	{
		Button::Style& s = Button::StyleNormal().Write();
		Image edge = MakeClassicButton(wg, IsDarkTheme() ? Blend(Gray(), Black()) : Black(), White(), Gray(), SColorFace());
		s.look[CTRL_HOT] = s.look[CTRL_NORMAL] = IsDarkTheme() ? edge : MakeClassicButton(White(), Black(), wg, Gray(), SColorFace());
		s.look[CTRL_PRESSED] = MakeClassicButton(Gray(), Gray(), Gray(), Gray(), grayface);
		s.look[CTRL_DISABLED] = MakeClassicButton(SWhite(), Black(), wg, Gray(), SColorFace());
		s.monocolor[0] = s.monocolor[1] = s.monocolor[2] = s.monocolor[3] = SColorText();
		s.pressoffset.x = s.pressoffset.y = 1;
		s.transparent = false;

		Button::Style& es = Button::StyleEdge().Write();
		es.look[CTRL_HOT] = es.look[CTRL_NORMAL] = edge;
		es.look[CTRL_PRESSED] = s.look[CTRL_PRESSED];
		es.look[CTRL_DISABLED] = s.look[CTRL_DISABLED];
		
		for(int i = 0; i < 4; i++)
			Button::StyleOk().Write().look[i] = Button::StyleLeftEdge().Write().look[i] = Button::StyleScroll().Write().look[i] = es.look[i];
	}

	{
		ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
		auto SbWc = [&](Value *look) {
			Color wc = Blend(SColorFace(), Gray());
			look[CTRL_NORMAL] = wc;
			look[CTRL_HOT] = wc;
			look[CTRL_PRESSED] = SColorText();
			look[CTRL_DISABLED] = wc;
		};
		
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
	
	int c = DPI(14);

	for(int i = 0; i < 4; i++) {
		{
			TabCtrl::Style& s = TabCtrl::StyleDefault().Write();
			s.body = MakeClassicButton(White(), Black(), wg, Gray(), SColorFace());
			Color f = i == CTRL_PRESSED ? SColorFace : grayface;
			Image t = MakeClassicButton(White(), Black(), wg, Gray(), f);
			Size isz = t.GetSize();
			isz.cy -= 2;
			ImageDraw iw(isz);
			iw.DrawImage(0, 0, t);
			if(i == CTRL_PRESSED) { // the active tab
				iw.DrawRect(isz.cx - 1, isz.cy - 2, 2, 1, White());
				iw.DrawRect(isz.cx - 2, isz.cy - 1, 2, 1, wg);
			}
			t = iw;
			SetHotSpots(t, Point(2, 2));
			s.first[i] = s.last[i] = s.both[i] = s.normal[i] = t;
			s.margin = 0;
			s.sel = Rect(0, DPI(1), 0, DPI(1));
			s.extendleft = DPI(2);
			s.text_color[i] = SColorText();
		}
		static int adj[] = { 10, 80, -5, -10 };
		Color f = FaceColor(adj[i]);
		{
			for(int opt = 0; opt < 2; opt++) {
				ImagePainter p(c, c);
				p.Clear(RGBAZero());
				p.Circle(DPI(7), DPI(7), DPI(6)).Fill(f).Stroke(2, Pointf(DPI(2), DPI(2)), SGray(), Pointf(DPI(14), DPI(14)), White());
				p.Circle(DPI(7), DPI(7), DPI(6) - 1).Fill(f).Stroke(1, Pointf(DPI(2) + 1, DPI(2) + 1), SBlack(), Pointf(DPI(14) - 2, DPI(14) - 2), SColorFace());
				if(opt)
					p.Circle(DPI(7), DPI(7), DPI(4)).Fill(SColorText());
				CtrlsImg::Set((opt ? CtrlsImg::I_S1 : CtrlsImg::I_S0) + i, p);
			}
		}
		{
			for(int chk = 0; chk < 3; chk++) {
				ImagePainter p(c, c);
				p.Clear(RGBAZero());
				DrawClassicButton(p, Size(c, c), SGray(), White(), SBlack(), LtGray(), f);
				p.Scale(DPI(1));
				if(chk == 1)
					p.Move(3, 7).Line(7, 10).Line(11, 4).Stroke(2, SColorText());
				if(chk == 2)
					p.Rectangle(3, 6, 8, 2).Fill(SColorText());
				CtrlsImg::Set(decode(chk, 0, CtrlsImg::I_O0, 1, CtrlsImg::I_O1, CtrlsImg::I_O2) + i, p);
			}
		}
	}
	
	MakeDialogIcons();
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
	Sizef isz = m.GetSize();
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
		w.Fill(SColorFace());
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
	return MakeElement(Size((int)q, (int)q), radius, face, border_width, border_color, [&](Painter& w, const Rectf& r) {
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

void ChSynthetic(Image *button100x100, Color *text, bool macos)
{
	int roundness = DPI(3);
	int roundness2 = roundness;
	Color ink = SColorText();
	int lw = macos ? 1 : DPI(1);
	for(int i = 0; i < 4; i++) {
		Image m = button100x100[i];
		Image m2 = macos ? button100x100[i + 4] : m;
		auto Espots = [=](const Image& m) { return WithHotSpots(m, DPI(3), DPI(1), CH_EDITFIELD_IMAGE, DPI(3)); };
		if(i == 0) {
			ink = GetInk(m);
			if(macos && IsDarkTheme())
				ink = Gray();
			roundness = macos ? DPI(3) : GetRoundness(m) ? DPI(3) : 0;
			roundness2 = macos ? 0 : roundness;
			CtrlsImg::Set(CtrlsImg::I_EFE, Espots(MakeButton(roundness2, SColorPaper(), lw, ink)));
			CtrlsImg::Set(CtrlsImg::I_VE, WithHotSpots(MakeButton(DPI(0), SColorPaper(), lw, ink), DPI(2), DPI(2), 0, 0));
			LabelBox::SetLook(WithHotSpots(MakeButton(2 * roundness / 3, Image(), lw, ink), DPI(3), DPI(3), 0, 0));
		}
		Size sz = m.GetSize();
		m = Crop(m, sz.cx / 8, sz.cy / 8, 6 * sz.cx / 8, 6 * sz.cy / 8);
		m2 = Crop(m2, sz.cx / 8, sz.cy / 8, 6 * sz.cx / 8, 6 * sz.cy / 8);
		{
			EditField::Style& s = EditField::StyleDefault().Write();
			s.activeedge = true;
			s.edge[i] = Espots(MakeButton(roundness2,
			                              i == CTRL_DISABLED ? SColorFace() : SColorPaper(),
			                              macos && i == CTRL_PRESSED ? DPI(2) : lw,
			                              i == CTRL_PRESSED ? SColorHighlight() : ink));
			if(i == 0)
				s.coloredge = Espots(MakeButton(roundness2, Black(), DPI(2), Null));
		}
		{
			auto Set = [&](Button::Style& s, const Image& arrow = Null, Color ink2 = Null, Color border = Null) {
				Value l = MakeButton(0, m, DPI(1), Nvl(border, Nvl(ink2, ink)), 0);
				s.look[i] = IsNull(arrow) ? l : ChLookWith(l, arrow, ink2);
			};
			Color c = Blend(SColorFace(), ink);
			Color k = text[i];

			Set(Button::StyleScroll().Write(), Null, k, c);
			Set(Button::StyleEdge().Write());
			Set(Button::StyleLeftEdge().Write());
			ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
			
			Set(s.up, CtrlsImg::UA(), k, c);
			Set(s.down, CtrlsImg::DA(), k, c);
			Set(s.left, CtrlsImg::LA(), k, c);
			Set(s.right, CtrlsImg::RA(), k, c);
		}
		{
			MultiButton::Style& s = MultiButton::StyleDefault().Write();
			s.clipedge = true;
			s.border = s.trivialborder = 0;

			s.left[i] = MakeButton(roundness, m2, lw, ink, CORNER_TOP_LEFT|CORNER_BOTTOM_LEFT);
			s.trivial[i] = s.look[i] = s.right[i] = MakeButton(roundness, m2, lw, ink, CORNER_TOP_RIGHT|CORNER_BOTTOM_RIGHT);
			if(i == 0)
				s.coloredge = WithHotSpots(MakeButton(roundness, Black(), DPI(2), Null), DPI(3), lw, 0, 0);
			auto Middle = [&](Image m) {
				ImageBuffer ib(m);
				for(int y = 0; y < lw; y++)
					for(int x = 0; x < ib.GetWidth(); x++) {
						ib[y][x] = ink;
						ib[ib.GetHeight() - y - 1][x] = ink;
					}
				return WithHotSpot(ib, DPI(1), DPI(1));
			};
			s.lmiddle[i] = Middle(WithRightLine(m2, ink, lw));
			s.rmiddle[i] = Middle(WithLeftLine(m2, ink, lw));
			s.monocolor[i] = s.fmonocolor[i] = text[macos ? i + 4 : i];
			for(int i = 0; i < 4; i++)
				s.edge[i] = Espots(MakeButton(roundness, i == CTRL_DISABLED ? SColorFace() : SColorPaper(), lw, ink));
			s.margin = Rect(DPI(3), 2, lw, 2);
			s.activeedge = true;
			s.stdwidth = DPI(17);
		}
		{
			SpinButtons::Style& sp = SpinButtons::StyleDefault().Write();
			if(i == 0)
				sp.dec = sp.inc = Button::StyleNormal();
			auto Spin = [&](dword corners, const Image& sm) {
				return ChLookWith(WithLeftLine(MakeButton(roundness2, m, 0, Black(), corners), ink, lw), sm, text[i]);
			};
			sp.inc.look[i] = Spin(CORNER_TOP_RIGHT, CtrlImg::spinup());
			sp.dec.look[i] = Spin(CORNER_BOTTOM_RIGHT, CtrlImg::spindown());
			sp.width = DPI(16);
			sp.over = DPI(2);
		}
		{
			SpinButtons::Style& sp = SpinButtons::StyleOnSides().Write();
			if(i == 0)
				sp.dec = sp.inc = Button::StyleNormal();
			auto Spin = [&](dword corners, const Image& sm, bool left) {
				Image mm = MakeButton(roundness2, m, 0, Black(), corners);
				mm = left ? WithLeftLine(mm, ink, lw) : WithRightLine(mm, ink, lw);
				return ChLookWith(mm, sm, text[i]);
			};
			sp.inc.look[i] = Spin(CORNER_TOP_RIGHT|CORNER_BOTTOM_RIGHT, CtrlImg::plus(), true);
			sp.dec.look[i] = Spin(CORNER_TOP_LEFT|CORNER_BOTTOM_LEFT, CtrlImg::minus(), false);
			sp.width = DPI(16);
			sp.over = DPI(2);
		}
		{
			HeaderCtrl::Style& hs = HeaderCtrl::StyleDefault().Write();
			Image h = m;
			if(macos)
				h = CreateImage(Size(10, 10), FaceColor(decode(i, CTRL_NORMAL, 10,
			                                                      CTRL_HOT, IsDarkTheme() ? 15 : 0,
			                                                      CTRL_PRESSED, -5,
			                                                      -8)));
			hs.look[i] = ChHot(WithBottomLine(WithRightLine(h, ink, 1), ink));
		}
		if(i == CTRL_DISABLED) {
			ProgressIndicator::Style& s = ProgressIndicator::StyleDefault().Write();
			ImageBuffer ib(1, 8);
			ImageBuffer ibb(1, 8);
			Color c = macos ? AvgColor(button100x100[4]) : SColorHighlight();
			for(int i = 0; i < 8; i++) {
				int a[] = { 20, 40, 10, 0, -10, -20, -30, -40 };
				ib[i][0] = AdjustColor(c, a[i]);
				ibb[i][0] = Blend(SColorFace(), SColorPaper(), i * 255 / 7);
			}
			s.hchunk = MakeButton(roundness, Magnify(ib, 10, 1), DPI(1), ink);
			s.hlook = MakeButton(roundness, Magnify(ibb, 10, 1), DPI(1), ink);
			s.bound = true;
			s.nomargins = true;
		}
		if(i == CTRL_NORMAL || i == CTRL_PRESSED) {
			Image sm = MakeElement(Size(DPI(10), DPI(20)), roundness,
			                       CreateImage(Size(10, 10), GrayColor(224 - 20 * i)),
			                       lw, ink, [&](Painter& w, const Rectf& r) {
				double cx = r.GetWidth();
				double cy = r.GetHeight();
				double uy = 0.4 * cy;
				double by = 0.85 * cy;
				double uq = 0.5 * uy;
				w.Move(r.left, r.top + by)
				 .Line(r.left, r.top + uy)
				 .Quadratic(r.left, r.top + uq, r.left + cx / 2, r.top)
				 .Quadratic(r.left + cx, r.top + uq, r.left + cx, r.top + uy)
				 .Line(r.left + cx, r.top + by)
				 .Close();
			});
			CtrlImg::Set(i == CTRL_PRESSED ? CtrlImg::I_hthumb1 : CtrlImg::I_hthumb, sm);
			CtrlImg::Set(i == CTRL_PRESSED ? CtrlImg::I_vthumb1 : CtrlImg::I_vthumb, RotateClockwise(sm));
		}
		{
			SyntheticTab(i, roundness, ink);
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

	Color border = Gray();
		
	{
		for(int pass = 0; pass < 2; pass++) {
			Button::Style& s = pass ? Button::StyleOk().Write() : Button::StyleNormal().Write();
			int roundness = DPI(3);
			s.focusmargin = DPI(4);
			for(int i = 0; i < 4; i++) {
				static int adj[] = { 10, 80, -5, -10 };
				Color f = FaceColor(adj[i]);
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
			s.hthumb[status] = s.vthumb[status] = AdjustColor(border, adj[status]);
		}
	}
	
	{
		MenuBar::Style& s = MenuBar::StyleDefault().Write();
		s.topitem[1] = Blend(SColorHighlight(), SColorPaper());
	}
	GUI_PopUpEffect_Write(Ctrl::IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
	
	MakeDialogIcons();
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
