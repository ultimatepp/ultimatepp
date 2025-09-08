#include "RichText.h"

namespace Upp {

Index<String> DiagramItem::LineCap = { "none", "arrow", "circle", "disc", "dim", "T" };

Index<String> DiagramItem::Shape = { "line", "rect", "round_rect",
                                     "ellipse", "diamond", "oval", "parallelogram",
                                     "cylinder",
                                     "triangle1", "triangle2",
                                     "arrow_left", "arrow_right", "arrow_horz",
                                     "arrow_down", "arrow_up", "arrow_vert",
                                     "arc",
                                     "svgpath", "image"
};

Vector<Pointf> DiagramItem::GetConnections() const
{
	Vector<Pointf> p;
	if(shape > SHAPE_ITRIANGLE || rotate)
		return p;
	if(IsLine()) {
		p << pt[0] << pt[1];
		return p;
	}
	Rectf r = GetRect();
	p << r.TopCenter() << r.BottomCenter();
	if(findarg(shape, SHAPE_PARALLELOGRAM, SHAPE_TRIANGLE, SHAPE_ITRIANGLE) < 0)
		p << r.CenterLeft() << r.CenterRight();
	if(shape == SHAPE_TRIANGLE)
		p << r.BottomLeft() << r.BottomRight();
	if(shape == SHAPE_ITRIANGLE)
		p << r.TopLeft() << r.TopRight();
	if(rotate) {
		Xform2D rot = Rotation();
		Pointf c = r.CenterPoint();
		for(Pointf& x : p) {
			x -= c;
			x = rot.Transform(x);
			x += c;
		}
	}
	return p;
}

const Vector<double>& DiagramItem::GetDash(int i)
{
	static Vector<double> dashes[10] = { { 0 }, { 1, 1 }, { 2 }, { 1, 2 }, { 2, 1 }, { 2, 1, 1, 1 }, { 3, 1 }, { 1, 3 }, { 1, 4 }, { 2, 5 } };
	return dashes[clamp(i, 0, 9)];
}

void DiagramItem::Paint(Painter& w, const Diagram& diagram, dword style, const Index<Pointf> *conn) const
{
	bool dark = style & DARK;
	
	auto SC = [&](Color c) { return dark ? DarkThemeCached(c) : c; };

	Color ink = SC(this->ink);
	Color paper = SC(this->paper);
	
	PaintInfo pi;
	pi.darktheme = dark;
	pi.zoom = Diagram::TextZoom();

	RichText txt = ParseQTF(qtf);

	auto DoDash = [&] {
		if(dash) {
			Vector<double> d = clone(GetDash(dash));
			for(double& h : d)
				h *= width;
			w.Dash(d, 0);
		}
	};
	
	RGBA sel1 = 150 * SColorHighlight();
	RGBA sel2 = 150 * Gray();
	
	double stroke = width;
	
	auto Stroke = [&] {
		if(stroke)
			w.Stroke(stroke, ink);
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
			if(findarg(cap[0], CAP_ARROW, CAP_DIM) >= 0)
				a1 += v * 4 * width;
			if(findarg(cap[1], CAP_ARROW, CAP_DIM) >= 0)
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
				case CAP_T:
					w.Move(p - 2 * oo).Line(p + 2 * oo).Stroke(1, ink);
					break;
				case CAP_DIM:
					w.Move(p - 2 * oo).Line(p + 2 * oo).Stroke(1, ink);
				case CAP_ARROW:
					w.Move(p).Line(a + oo).Line(a - oo).Fill(ink);
					break;
				case CAP_DISC:
					w.Circle(p, 5).Fill(ink);
					break;
				case CAP_CIRCLE:
					w.Circle(p, 5).Fill(paper).Stroke(1, ink);
					break;
				}
			};
			PaintCap(cap[0], pt[0], a1 + v);
			PaintCap(cap[1], pt[1], a2 - v);
		}
		 
		int cx = (int)Distance(pt[0], pt[1]);
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
		Rectf r(pt[0], pt[1]);
		r.Normalize();
		r.Deflate(width / 2);
		double w1 = r.GetWidth();
		double h = r.GetHeight();

		double cx = r.GetWidth();
		double cy = r.GetHeight();

		Rectf text_rect = r.Deflated(width + 2, 0).Offseted(-r.TopLeft());

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

		w.Begin();
		w.Translate(r.left, r.top);
		if(rotate) {
			w.Translate(w2, h2);
			w.Rotate(M_2PI * rotate / 360);
			w.Translate(-w2, -h2);
		}

		if(style & (Display::CURSOR | Display::SELECT)) {
			w.RoundedRectangle(-2, -2, w1 + 4, h + 4, 5)
			 .Stroke(6, (style & Display::SELECT ? 30 : 200) * sel1);
			w.RoundedRectangle(-1, -1, w1 + 2, h + 2, 2)
			 .Stroke(2, Gray());
			if(style & Display::CURSOR)
				w.Circle(0, h, 6).Stroke(1, Red());
		}


		w.Begin();
		if(flip_horz) {
			w.Translate(cx, 0);
			w.Scale(-1, 1);
		}

		if(flip_vert) {
			w.Translate(0, cy);
			w.Scale(1, -1);
		}

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
				w.Move(cx / 2, 0)
				 .Line(cx, a1)
				 .Line(cx - w4, a1)
				 .Line(cx - w4, a2)
				 .Line(cx, a2)
				 .Line(cx / 2, cy)
				 .Line(0, a2)
				 .Line(w4, a2)
				 .Line(w4, a1)
				 .Line(0, a1)
				 .Close();
			}
			break;
		case SHAPE_ARC:
			if(w2 > 0) {
				double a = (sqr(w2) - sqr(cy)) / (2 * cy);
				double f = atan(w2 / a);
				if(a >= 0)
					w.Move(0, cy).Arc(w2, a + cy, a + cy, -M_PI / 2 - f, 2 * f);
				else
					w.Move(0, cy).Arc(w2, cy, w2, cy, M_PI, M_PI);
			}
			break;
		case SHAPE_SVGPATH: {
				Rectf bb = diagram.GetBlobSvgPathBoundingBox(blob_id);
				Sizef size = bb.GetSize();
				if(size.cx > 0 && size.cy > 0) {
					w.Scale(w1 / size.cx, h / size.cy);
					stroke *= min(size.cx / w1, size.cy / h);
					w.Translate(-bb.TopLeft());
					w.Path(diagram.GetBlob(blob_id));
				}
			}
			break;
		case SHAPE_IMAGE: {
				if(style & FAST) {
					w.Rectangle(0, 0, cx, cy).Fill(Gray());
					break;
				}
				String s = diagram.GetBlob(blob_id);
				if(s.GetCount()) {
					if(IsSVG(s)) {
						Rectf f = GetSVGBoundingBox(s);
						w.Scale(cx / f.GetWidth(), cy / f.GetHeight());
						w.Translate(-f.left, -f.top);
						RenderSVG(w, s, Event<String, String&>(), paper);
					}
					else {
						StringStream ss(s);
						Value v = MakeValue(
							[&] {
								return blob_id;
							},
							[&](Value& v) {
								v = Null;
								One<StreamRaster> r = StreamRaster::OpenAny(ss);
								if(r) {
									Image m = r->GetImage();
									v = m;
									return int(m.GetLength() * sizeof(RGBA));
								}
								return 0;
							}
						);
						if(v.Is<Image>()) {
							Image img = v;
							w.Rectangle(0, 0, cx, cy).Fill(img, Xform2D::Scale(cx / img.GetWidth(), cy / img.GetHeight()));
						}
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
			if(shape != SHAPE_ARC)
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
		
		Rectf tr2 = text_rect;
		if(flip_horz) {
			text_rect.right = cx - tr2.left;
			text_rect.left = cx - tr2.right;
		}

		if(flip_vert) {
			text_rect.top = cy - tr2.bottom;
			text_rect.bottom = cy - tr2.top;
		}
	
		Rect tr = text_rect;
		int txt_cy = txt.GetHeight(pi.zoom, tr.GetWidth());
		txt.Paint(w, tr.left, tr.top + (tr.GetHeight() - txt_cy) / 2, tr.GetWidth(), pi);

		w.End();

		if((style & GRID) && !rotate)
			for(Pointf p : GetConnections()) {
				w.Circle(p, 5);
				if(conn && conn->Find(p) >= 0)
					w.Fill(128 * SYellow());
				w.Stroke(1, 190 * SColorHighlight());
			}
	}
}

Sizef DiagramItem::GetStdSize(const Diagram& diagram) const
{
	switch(shape) {
	case SHAPE_SVGPATH:
		return diagram.GetBlobSvgPathBoundingBox(blob_id).GetSize();
	case SHAPE_IMAGE: {
			String s = diagram.GetBlob(blob_id);
			if(s.GetCount()) {
				if(IsSVG(s))
					return GetSVGBoundingBox(s).GetSize();
				else {
					StringStream ss(s);
					Value v = MakeValue(
						[&] {
							return blob_id;
						},
						[&](Value& v) {
							v = Null;
							One<StreamRaster> r = StreamRaster::OpenAny(ss);
							if(r) {
								v = r->GetSize();
								return 32;
							}
							return 0;
						}
					);
					if(v.Is<Size>())
						return v.To<Size>();
				}
			}
		}
	}

	if(shape == SHAPE_CYLINDER)
		return Size(100, 128);

	if(findarg(shape, SHAPE_CYLINDER, SHAPE_ARROWDOWN, SHAPE_ARROWUP, SHAPE_ARROWVERT) >= 0)
		return Size(64, 128);

	return Size(128, 64);
}

}
