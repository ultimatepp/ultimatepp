#include "RichEdit.h"

namespace Upp {

void DiagramEditor::RightDown(Point p, dword keyflags)
{
	Map(p);

	FinishText();

	int ii = FindItem(p);
	if(ii >= 0) {
		DiagramItem& m = data.item[ii];
		if(m.IsLine()) {
			SetCursor(ii);
			Point h = GetHandle(cursor, p);
			if(h.x) {
				int i = h.x > 0;
				ColumnPopUp menu;
				Caps(menu, i == 0);
				int cap = menu.Execute();
				if(cap < 0)
					return;
				m.cap[i] = cap;
				GetAttrs();
				Sync();
				return;
			}
			if(m.IsClick(p, data)) {
				ColumnPopUp menu;
				Dashes(menu);
				menu.count = DiagramItem::DASH_COUNT + 15;
				menu.columns = 5;
				menu.WhenPaintItem = [=](Draw& w, Size isz, int ii, bool sel) {
					PopPaint(w, ii < DiagramItem::DASH_COUNT ? DashIcon(ii) : WidthIcon(ii - DiagramItem::DASH_COUNT), sel);
				};

				int n = menu.Execute();
				if(n < 0)
					return;
				if(n < DiagramItem::DASH_COUNT)
					m.dash = n;
				else
					m.width = n - DiagramItem::DASH_COUNT;
				GetAttrs();
				Sync();
				return;
			}
		}
	}

	tool = -1;
	SetBar();

	ColumnPopUp shape;
	Shapes(shape);
	
	int si = shape.Execute();
	
	if(si < 0)
		return;

	Sizef size;
	String mdata;
	if(si == DiagramItem::SHAPE_SVGPATH) {
		mdata = SelectFontSymbolSvg(size);
		if(IsNull(mdata))
			return;
	}

	if(si == DiagramItem::SHAPE_IMAGE) {
		String path = SelectFileOpen("Images (*.png *.gif *.jpg *.bmp *.svg)\t*.png *.gif *.jpg *.bmp *.svg");

		if(GetFileLength(path) > 17000000) {
			Exclamation("Image is too large!");
			return;
		}
		mdata = LoadFile(path);
		if(IsNull(mdata))
			return;
		bool loaded = false;
		if(IsSVG(mdata)) {
			loaded = true;
		}
		else {
			StringStream ss(mdata);
			One<StreamRaster> r = StreamRaster::OpenAny(ss);
			loaded = true;
		}
		if(IsNull(size)) {
			Exclamation(t_("Unsupported image format."));
			return;
		}
	}
	
	CancelSelection();

	Point p0 = p;
	Grid(p);
	Pointf cp = Null; // connect line with nearest connection point
	if(si == DiagramItem::SHAPE_LINE) {
		double mind = DBL_MAX;
		for(const DiagramItem& m : data.item)
			for(Pointf c : m.GetConnections()) {
				double d = Squared(c - (Pointf)p0);
				if(d < mind) {
					cp = c;
					mind = d;
				}
			}
	}
	
	Size sz;

	DiagramItem& m = AddItem(si);
	if(mdata.GetCount())
		m.blob_id = data.AddBlob(mdata);
	m.shape = si; // shape must be set before SetAttrs to avoid Normalise
	Sizef szf = m.GetStdSize(data);
	while(max(szf.cx, szf.cy) > 1000)
		szf *= 0.5;
	if(IsNull(cp)) {
		m.pos = p;
		m.size = szf / 2;
	}
	else {
		m.pos = cp;
		m.size = Pointf(p) - cp;
	}
	if(si == DiagramItem::SHAPE_IMAGE) {
		m.ink = Null;
		m.paper = Black();
		m.width = 0;
		SetAttrs(ATTR_ALL & ~(ATTR_SHAPE|ATTR_PAPER|ATTR_INK|ATTR_WIDTH));
	}
	else
	if(si == DiagramItem::SHAPE_SVGPATH) {
		m.ink = Null;
		m.paper = Black();
		m.width = 0;
		SetAttrs(ATTR_ALL & ~(ATTR_SHAPE|ATTR_PAPER|ATTR_INK));
	}
	else
		SetAttrs(ATTR_ALL & ~ATTR_SHAPE);
	Sync();
	GetAttrs();
}

void DiagramEditor::RightUp(Point, dword keyflags)
{
	Commit();
}

}