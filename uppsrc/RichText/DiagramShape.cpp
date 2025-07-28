#include "RichText.h"

namespace Upp {

Index<String> DiagramItem::Shape = { "line", "rect", "round_rect",
                                     "ellipse", "diamond", "oval", "parallelogram",
                                     "cylinder",
                                     "triangle1", "triangle2",
                                     "arrow_left", "arrow_right", "arrow_horz",
                                     "arrow_down", "arrow_up", "arrow_vert",
                                     "svgpath", "image"
};

Vector<Pointf> DiagramItem::GetConnections() const
{
	Vector<Pointf> p;
	if(shape > SHAPE_CYLINDER)
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

void DiagramItem::Paint(Painter& w, const VectorMap<int, String>& data, dword style, const Index<Pointf> *conn) const
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

		w.Begin();
		w.Offset(r.left, r.top);

		double w1 = r.GetWidth();
		double h = r.GetHeight();

		double cx = r.GetWidth();
		double cy = r.GetHeight();

		Rect text_rect = r.Deflated(width + 2, 0);

		double sz = min(cx, cy);
		double arrow_width = min(cx / 3, cy / 3);
		double arrow_height = min(cx / 3, cy / 3);
		double h2 = h / 2;
		double h4 = h / 4;
		double bh4 = h - h4;
		double w2 = w1 / 2;
		double w4 = w1 / 4;
		Pointf m(w2, h2);
		double hc, thc, bhc; // cylinder
		switch(shape) {
		case SHAPE_ROUNDRECT:
			w.RoundedRectangle(0, 0, w1, h, sz > 30 ? 8 : sz > 15 ? 4 : 2);
			break;
		case SHAPE_OVAL:
			if(w1 > h) {
				double ra = h2;
				w.Move(ra, 0)
				 .Line(w1 - ra, 0)
				 .Arc(w1 - ra, 0 + ra, ra, -M_PI / 2, M_PI)
				 .Line(ra, cy)
				 .Arc(ra, 0 + ra, ra, M_PI / 2, M_PI);
				break;
			}
		case SHAPE_ELLIPSE:
			w.Ellipse(m.x, m.y, w2, h2);
			break;
		case SHAPE_DIAMOND:
			w.Move(m.x, 0).Line(cx, m.y).Line(m.x, cy).Line(0, m.y).Close();
			break;
		case SHAPE_PARALLELOGRAM:
			w.Move(cx / 6, 0).Line(cx, 0).Line(5 * cx / 6, cy).Line(0, cy).Close();
			break;
		case SHAPE_CYLINDER:
			text_rect.top += int(w1 / 4);
			hc = h / 6;
			thc = 0 + hc;
			bhc = cy - hc;
			w.Move(0, thc)
			 .Arc(0 + w2, thc, w2, hc, M_PI, M_PI)
			 .Line(cx, bhc)
			 .Arc(0 + w2, bhc, w2, hc, 0, M_PI)
			 .Line(0, bhc);
			 break;
		case SHAPE_TRIANGLE: {
				text_rect.left  += w4;
				text_rect.right -= w4;
				text_rect.top += int(cx / 3);
				w.Move(w2, 0).Line(cx, cy).Line(0, cy).Close();
			}
			break;
		case SHAPE_ITRIANGLE: {
				text_rect.left  += int(cx / 4);
				text_rect.right -= int(cx / 4);
				text_rect.bottom -= int(cx / 3);
				w.Move(w2, cy).Line(cx, 0).Line(0, 0).Close();
			}
			break;
		case SHAPE_ARROWLEFT: {
				double a = 0 + arrow_width;
				text_rect.left += int(arrow_width / 3);
				w.Move(0, h2).Line(a, 0).Line(a, h4).Line(cx, h4)
				 .Line(cx, bh4).Line(a, bh4).Line(a, cy).Close();
			}
			break;
		case SHAPE_ARROWRIGHT:
			 {
				double a = cx - arrow_width;
				text_rect.right -= int(arrow_width / 3);
				w.Move(cx, cy / 2)
				 .Line(a, 0)
				 .Line(a, h4)
				 .Line(0, h4)
				 .Line(0, bh4)
				 .Line(a, bh4)
				 .Line(a, cy)
				 .Close();
			}
			break;
		case SHAPE_ARROWHORZ:
			 {
				double a1 = 0 + arrow_width;
				text_rect.left += int(arrow_width / 3);
				double a2 = cx - arrow_width;
				text_rect.right -= int(arrow_width / 3);
				w.Move(0, h2)
				 .Line(a1, 0)
				 .Line(a1, h4)
				 .Line(a2, h4)
				 .Line(a2, 0)
				 .Line(cx, h2)
				 .Line(a2, cy)
				 .Line(a2, bh4)
				 .Line(a1, bh4)
				 .Line(a1, cy)
				 .Close();
			}
			break;
		case SHAPE_ARROWUP: {
				double a = arrow_height;
				text_rect.left += w4;
				text_rect.right -= w4;
				text_rect.top += 3 * arrow_height / 4;
				w.Move(w2, 0)
				 .Line(cx, a)
				 .Line(cx - w4, a)
				 .Line(cx - w4, cy)
				 .Line(w4, cy)
				 .Line(w4, a)
				 .Line(0, a)
				 .Close();
			}
			break;
		case SHAPE_ARROWDOWN: {
				double a = cy - arrow_height;
				text_rect.left += w4;
				text_rect.right -= w4;
				text_rect.bottom -= 3 * arrow_height / 4;
				w.Move(0 + cx / 2, cy)
				 .Line(cx, a)
				 .Line(cx - w4, a)
				 .Line(cx - w4, 0)
				 .Line(w4, 0)
				 .Line(w4, a)
				 .Line(0, a)
				 .Close();
			}
			break;
		case SHAPE_ARROWVERT: {
				double a1 = 0 + arrow_height;
				double a2 = cy - arrow_height;
				text_rect.left += w4;
				text_rect.right -= w4;
				w.Move(0 + cx / 2, 0)
				 .Line(cx, a1)
				 .Line(cx - w4, a1)
				 .Line(cx - w4, a2)
				 .Line(cx, a2)
				 .Line(0 + cx / 2, cy)
				 .Line(0, a2)
				 .Line(w4, a2)
				 .Line(w4, a1)
				 .Line(0, a1)
				 .Close();
			}
			break;
		case SHAPE_SVGPATH:
			if(data.GetCount() && !IsNull(size)) {
				w.Scale(w1 / size.cx, h / size.cy);
				w.Path(data.Get(blob_id, String()));
			}
			break;
		case SHAPE_IMAGE:
			if(data.GetCount()) {
				String s = data.Get(blob_id, String());
				if(s.GetCount()) {
					StringStream ss(s);
					One<StreamRaster> r = StreamRaster::OpenAny(ss);
					if(r) {
						w.DrawImage(0, 0, cx, cy, r->GetImage());
					}
					else
					if(IsSVG(s)) {
						Rectf f = GetSVGBoundingBox(s);
						Sizef isz = f.GetSize();
						w.Scale(cx / f.GetWidth(), cy / f.GetHeight());
						w.Translate(-f.left, -f.top);
						RenderSVG(w, s, Event<String, String&>(), paper);
					}
				}
			}
			break;
		default:
			w.Rectangle(0, 0, cx, cy);
			break;
		}
		
		if(shape != SHAPE_IMAGE) {
			DoDash();
			w.Fill(paper);
			Stroke();
		}
		
		switch(shape) {
		case SHAPE_CYLINDER:
			w.Move(0, thc)
			 .Arc(0 + w2, thc, w2, hc, M_PI, -M_PI);
			DoDash();
			Stroke();
			break;
		}
		
		w.End();
		
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