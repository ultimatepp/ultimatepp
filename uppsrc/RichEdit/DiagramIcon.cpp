#include "RichEdit.h"

namespace Upp {

Image DiagramEditor::MakeIcon(DiagramItem& m, Size isz)
{
	struct IconMaker : ImageMaker {
		Size         isz;
		DiagramItem  m;
		bool         dark;
		String Key() const override {
			String key = StoreAsString(const_cast<DiagramItem&>(m));
			RawCat(key, isz);
			RawCat(key, dark);
			return key;
		}
		Image Make() const override {
			ImagePainter iw(isz);
			iw.Clear();
			m.Paint(iw, Diagram(), dark ? DiagramItem::DARK : 0);
			return iw;
		}
	};

	IconMaker mk;
	mk.m = m;
	mk.isz = isz;
	mk.dark = IsDarkContent();
	return MakeImage(mk);
}

Image DiagramEditor::ShapeIcon(int i)
{
	Size isz = IconSz();
	DiagramItem m;
	m.pt[0] = Point(2, 2);
	m.pt[1] = Point(isz.cx - 2, isz.cy - 2);
	m.width = DPI(1);
	m.shape = i;
	m.paper = Null;
	return MakeIcon(m, isz);
}

Image DiagramEditor::CapIcon(int start, int end)
{
	Size isz = IconSz();
	DiagramItem m;
	m.pt[0] = Point(DPI(6), isz.cy / 2);
	m.pt[1] = Point(isz.cx - DPI(6), isz.cy / 2);
	m.shape = DiagramItem::SHAPE_LINE;
	m.width = DPI(2);
	m.cap[0] = start;
	m.cap[1] = end;
	return MakeIcon(m, isz);
}

Image DiagramEditor::DashIcon(int i)
{
	return MakeValue(
		[=] { return String((char *)&i, sizeof(i)); },
		[=](Value& v) {
			Size isz = IconSz();
			ImagePainter p(isz);
			p.Clear();
		
			Vector<double> h = clone(DiagramItem::GetDash(i));
			for(double& x : h)
				x *= DPI(2);
			p.Move(DPI(2), isz.cy / 2 - (i & 1) * 0.5)
			 .RelLine(isz.cx - DPI(4), 0)
			 .Dash(h, 0)
			 .Stroke(DPI(2), dark ? White() : Black());
			Image m = p;
			v = m;
			return m.GetLength() * sizeof(RGBA);
		}
	).To<Image>();
}

Image DiagramEditor::WidthIcon(int i)
{
	return MakeValue(
		[=] { return String((char *)&i, sizeof(i)); },
		[=](Value& v) {
			Size isz = IconSz();
			Color ink = dark ? White() : Black();
			ImagePainter p(isz);
			p.Clear();
			p.Move(DPI(2), isz.cy / 2.0 - (i & 1) * 0.5)
			 .RelLine(isz.cx - DPI(4), 0)
			 .Stroke(i, ink);
			Image m = p;
			v = m;
			return m.GetLength() * sizeof(RGBA);
		}
	).To<Image>();
}

void DiagramEditor::Dashes(ColumnPopUp& m)
{
	m.count = DiagramItem::DASH_COUNT;
	m.columns = 4;
	m.isz = IconSz() + Size(DPI(4), DPI(4));
	m.WhenPaintItem = [=](Draw& w, Size isz, int ii, bool sel) {
		PopPaint(w, DashIcon(ii), sel);
	};
}

void DiagramEditor::PopPaint(Draw& w, const Image& m, bool sel)
{
	Point p = Rect(IconSz() + Size(DPI(4), DPI(4))).CenterPos(m.GetSize());
	if(sel)
		w.DrawImage(p.x, p.y, m, SColorHighlightText());
	else
		w.DrawImage(p.x, p.y, m);
}

void DiagramEditor::Shapes(ColumnPopUp& shape)
{
	shape.count = DiagramItem::SHAPE_COUNT;
	shape.columns = 5;
	shape.isz = IconSz() + Size(DPI(4), DPI(4));
	shape.WhenPaintItem = [=](Draw& w, Size isz, int ii, bool sel) {
		PopPaint(w, ii == DiagramItem::SHAPE_SVGPATH ? DiagramImg::FontSvg() :
		            ii == DiagramItem::SHAPE_IMAGE   ? CtrlImg::open()
		                                             : ShapeIcon(ii),
		         sel && ii != DiagramItem::SHAPE_IMAGE);
	};
}

void DiagramEditor::Caps(ColumnPopUp& m, bool left)
{
	m.count = DiagramItem::CAP_COUNT;
	m.columns = 3;
	m.isz = IconSz() + Size(DPI(4), DPI(4));
	m.WhenPaintItem = [=](Draw& w, Size isz, int ii, bool sel) {
		PopPaint(w, left ? CapIcon(ii, 0) : CapIcon(0, ii), sel);
	};
}

void DiagramEditor::Widths(ColumnPopUp& m)
{
	m.count = 15;
	m.columns = 5;
	m.isz = IconSz() + Size(DPI(4), DPI(4));
	m.WhenPaintItem = [=](Draw& w, Size isz, int ii, bool sel) {
		PopPaint(w, WidthIcon(ii), sel);
	};
}

void Upp::DiagramEditor::DropColumns::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	popup.WhenPaintItem(w, r.GetSize(), (int)q, false);
}

DiagramEditor::DropColumns::DropColumns()
{
	AddButton().Main().WhenPush << [=] {
		SetData(popup.Execute(GetScreenRect(), this));
		Action();
	};
	SetDisplay(*this);
}

}
