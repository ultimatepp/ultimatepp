#include "RichText.h"

namespace Upp {

Index<String> DiagramItem::Shape = { "line", "rect", "round_rect",
                                     "ellipse", "diamond", "oval", "parallelogram",
                                     "triangle1", "triangle2",
                                     "arrow_left", "arrow_right", "arrow_horz",
                                     "arrow_down", "arrow_up", "arrow_vert",
};

Vector<Pointf> DiagramItem::GetConnections() const
{
	Vector<Pointf> p;
	if(shape > SHAPE_PARALLELOGRAM)
		return p;
	if(IsLine()) {
		p << pt[0] << pt[1];
		return p;
	}
	Rectf r = GetRect();
	p << r.TopCenter() << r.BottomCenter();
	if(shape != SHAPE_PARALLELOGRAM)
		p << r.CenterLeft() << r.CenterRight();
	return p;
}

void DiagramItem::Paint(Painter& w, dword style, const Index<Pointf> *conn) const
{
	bool dark = style & DARK;
	
	auto SC = [&](Color c) { return dark ? DarkThemeCached(c) : c; };

	Color ink = SC(this->ink);
	Color paper = SC(this->paper);
	
	PaintInfo pi;
	pi.darktheme = dark;
	pi.zoom = Diagram::TextZoom();

	RichText txt = ParseQTF(qtf);

	static Vector<double> dashes[5] = { { 0 }, { 1, 1 }, { 2 }, { 1, 2 }, { 1, 2 } };
	
	auto DoDash = [&] {
		if(dash) {
			Vector<double> d = clone(dashes[clamp(dash, 0, __countof(dashes))]);
			for(double& h : d)
				h *= width;
			w.Dash(d, 0);
		}
	};
	
	RGBA sel1 = 150 * SColorHighlight();
	RGBA sel2 = 150 * Gray();
	
	auto Stroke = [&] {
		if(width)
			w.Stroke(width, ink);
		else
		if(style & GRID)
			w.Stroke(0.2, sel1);
	};
	
	if(IsLine()) {
		Pointf v = pt[1] - pt[0];
		if(style) {
			w.Move(pt[0]).Line(pt[1]).EndPath();
			w.Begin();
			if((style & EDITOR) && width == 0)
				w.Dash("5 1").Stroke(1, 100 * sel2);
			if(style & (Display::CURSOR | Display::SELECT)) {
				w.LineCap(LINECAP_ROUND).Stroke(width + 12, (style & Display::SELECT ? 30 : 200) * sel2);
				double r = (width + 12) / 2 - 1;
				w.Circle(pt[0], r).Fill(sel1);
				w.Circle(pt[1], r).Fill(sel1);
			}
			w.End();
		}
		double d = Length(v);
		v = Upp::Normalize(v);

		Pointf a1 = pt[0];
		Pointf a2 = pt[1];
		if(d > 4 * width) { // enough length to have caps
			if(cap[0] == CAP_ARROW)
				a1 += v * 4 * width;
			if(cap[1] == CAP_ARROW)
				a2 -= v * 4 * width;
		}
		
		w.Move(a1).Line(a2);
		DoDash();
		Stroke();
		
		Pointf o = Orthogonal(v);
		if(d > 4 * width) {
			auto PaintCap = [&](int k, Pointf p, Pointf a) {
				Pointf oo = max(3.0, width * 2) * o;
				switch(k) {
				case CAP_NONE:
					w.Circle(p, width / 2).Fill(ink);
					break;
				case CAP_ARROW:
					w.Move(p).Line(a + oo).Line(a - oo).Fill(ink);
					break;
				case CAP_CIRCLE:
					w.Circle(p, 5).Fill(ink);
					break;
				}
			};
			PaintCap(cap[0], pt[0], a1 + v);
			PaintCap(cap[1], pt[1], a2 - v);
		}
		 
		int cx = Distance(pt[0], pt[1]);
		int txt_cy = txt.GetHeight(pi.zoom, cx);
		
		w.Begin();
		double angle = Bearing(pt[1] - pt[0]);
		if(angle >= -M_PI / 2 && angle <= M_PI / 2) {
			w.Translate(pt[0] - o * (txt_cy + 10));
			w.Rotate(angle);
		}
		else {
			w.Translate(pt[1] + o * (txt_cy + 10));
			w.Rotate(angle + M_PI);
		}
		txt.Paint(w, 0, 0, cx, pi);
		w.End();
	}
	else {
		if(style & (Display::CURSOR | Display::SELECT)) {
//			w.RoundedRectangle(GetRect(), 5)
//			 .Fill((style & Display::SELECT ? 30 : 200) * sel2);
			w.RoundedRectangle(GetRect().Inflated(2), 5)
			 .Stroke(6, (style & Display::SELECT ? 30 : 200) * sel1);
		}
		
		Rectf r(pt[0], pt[1]);
		r.Normalize();
		r.Deflate(width / 2);
		Rect text_rect = r.Deflated(width + 2, 0);
		Pointf c = r.CenterPoint();
		double sz = min(r.Width(), r.Height());
		double arrow_width = min(r.Width() / 3, r.Height() / 2);
		double h4 = r.Height() / 4;
		double th4 = r.top + h4;
		double bh4 = r.bottom - h4;
		double arrow_height = min(r.Height() / 3, r.Width() / 2);
		double w4 = r.Width() / 4;
		double lw4 = r.left + w4;
		double rw4 = r.right - w4;
		switch(shape) {
		case SHAPE_ROUNDRECT:
			w.RoundedRectangle(r.left, r.top, r.GetWidth(), r.GetHeight(), sz > 30 ? 8 : sz > 15 ? 4 : 2);
			break;
		case SHAPE_OVAL:
			if(r.GetWidth() > r.GetHeight()) {
				double ra = r.GetHeight() / 2;
				w.Move(r.left + ra, r.top)
				 .Line(r.right - ra, r.top)
				 .Arc(r.right - ra, r.top + ra, ra, -M_PI / 2, M_PI)
				 .Line(r.left + ra, r.bottom)
				 .Arc(r.left + ra, r.top + ra, ra, M_PI / 2, M_PI);
				break;
			}
		case SHAPE_ELLIPSE:
			w.Ellipse(r);
			break;
		case SHAPE_DIAMOND:
			w.Move(c.x, r.top).Line(r.right, c.y).Line(c.x, r.bottom).Line(r.left, c.y).Close();
			break;
		case SHAPE_PARALLELOGRAM:
			w.Move(r.left + r.Width() / 6, r.top).Line(r.right, r.top)
			 .Line(r.right - r.Width() / 6, r.bottom).Line(r.left, r.bottom).Close();
			break;
		case SHAPE_TRIANGLE: {
				text_rect.left  += int(r.Width() / 4);
				text_rect.right -= int(r.Width() / 4);
				text_rect.top += int(r.Width() / 3);
				w.Move(r.left + r.Width() / 2, r.top)
				 .Line(r.right, r.bottom)
				 .Line(r.left, r.bottom)
				 .Close();
			}
			break;
		case SHAPE_ITRIANGLE: {
				text_rect.left  += int(r.Width() / 4);
				text_rect.right -= int(r.Width() / 4);
				text_rect.bottom -= int(r.Width() / 3);
				w.Move(r.left + r.Width() / 2, r.bottom)
				 .Line(r.right, r.top)
				 .Line(r.left, r.top)
				 .Close();
			}
			break;		case SHAPE_ARROWLEFT: {
				double a = r.left + arrow_width;
				text_rect.left += int(arrow_width / 3);
				w.Move(r.left, r.top + r.Height() / 2)
				 .Line(a, r.top)
				 .Line(a, th4)
				 .Line(r.right, th4)
				 .Line(r.right, bh4)
				 .Line(a, bh4)
				 .Line(a, r.bottom)
				 .Close();
			}
			break;
		case SHAPE_ARROWRIGHT:
			 {
				double a = r.right - arrow_width;
				text_rect.right -= int(arrow_width / 3);
				w.Move(r.right, r.top + r.Height() / 2)
				 .Line(a, r.top)
				 .Line(a, th4)
				 .Line(r.left, th4)
				 .Line(r.left, bh4)
				 .Line(a, bh4)
				 .Line(a, r.bottom)
				 .Close();
			}
			break;
		case SHAPE_ARROWHORZ:
			 {
				double a1 = r.left + arrow_width;
				text_rect.left += int(arrow_width / 3);
				double a2 = r.right - arrow_width;
				text_rect.right -= int(arrow_width / 3);
				w.Move(r.left, r.top + r.Height() / 2)
				 .Line(a1, r.top)
				 .Line(a1, th4)
				 .Line(a2, th4)
				 .Line(a2, r.top)
				 .Line(r.right, r.top + r.Height() / 2)
				 .Line(a2, r.bottom)
				 .Line(a2, bh4)
				 .Line(a1, bh4)
				 .Line(a1, r.bottom)
				 .Close();
			}
			break;
		case SHAPE_ARROWUP: {
				double a = r.top + arrow_height;
				text_rect.left += w4;
				text_rect.right -= w4;
				text_rect.top += 3 * arrow_height / 4;
				w.Move(r.left + r.Width() / 2, r.top)
				 .Line(r.right, a)
				 .Line(rw4, a)
				 .Line(rw4, r.bottom)
				 .Line(lw4, r.bottom)
				 .Line(lw4, a)
				 .Line(r.left, a)
				 .Close();
			}
			break;
		case SHAPE_ARROWDOWN: {
				double a = r.bottom - arrow_height;
				text_rect.left += w4;
				text_rect.right -= w4;
				text_rect.bottom -= 3 * arrow_height / 4;
				w.Move(r.left + r.Width() / 2, r.bottom)
				 .Line(r.right, a)
				 .Line(rw4, a)
				 .Line(rw4, r.top)
				 .Line(lw4, r.top)
				 .Line(lw4, a)
				 .Line(r.left, a)
				 .Close();
			}
			break;
		case SHAPE_ARROWVERT: {
				double a1 = r.top + arrow_height;
				double a2 = r.bottom - arrow_height;
				text_rect.left += w4;
				text_rect.right -= w4;
				w.Move(r.left + r.Width() / 2, r.top)
				 .Line(r.right, a1)
				 .Line(rw4, a1)
				 .Line(rw4, a2)
				 .Line(r.right, a2)
				 .Line(r.left + r.Width() / 2, r.bottom)
				 .Line(r.left, a2)
				 .Line(lw4, a2)
				 .Line(lw4, a1)
				 .Line(r.left, a1)
				 .Close();
			}
			break;
		default:
			w.Rectangle(r);
			break;
		}
		DoDash();
		w.Fill(paper);
		Stroke();
		
		int txt_cy = txt.GetHeight(pi.zoom, text_rect.GetWidth());
		txt.Paint(w, text_rect.left, text_rect.top + (text_rect.GetHeight() - txt_cy) / 2, text_rect.GetWidth(), pi);

		if(style & GRID)
			for(Pointf p : GetConnections()) {
				w.Circle(p, 5);
				if(conn && conn->Find(p) >= 0)
					w.Fill(128 * SYellow());
				w.Stroke(1, 190 * SColorHighlight());
			}
	}
}

}