#include "RichEdit.h"

namespace Upp {

void DiagramEditor::Map(Point& p)
{
	p = (Pointf)p / GetZoom() + (Pointf)(Point)sb;
}

Point DiagramEditor::GetHandle(int i, Point p) const
{ // -1 top/left, 1 right/botom
	Point h(0, 0);
	if(i >= 0) {
		const DiagramItem& m = data.item[i];
		if(m.IsLine()) {
			if(Distance(m.p1, p) < 6)
				return Point(-1, -1);
			if(Distance(m.p2, p) < 6)
				return Point(1, 1);
			
		}
		else {
			Rect r = m.GetRect();
			Rect rr = r.Inflated(5);
			r.Deflate(min(10, r.GetWidth() / 2), min(10, r.GetHeight() / 2));
			if(rr.Contains(p)) {
				if(p.y < r.top)
					h.y = -1;
				if(p.y > r.bottom)
					h.y = 1;
				if(p.x < r.left)
					h.x = -1;
				if(p.x > r.right)
					h.x = 1;
			}
		}
	}
	return h;
}

int   DiagramEditor::FindItem(Point p) const
{
	int mini = -1;
	double mina = INT_MAX;
	for(int i = data.item.GetCount() - 1; i >= 0; i--) {
		Rectf r = data.item[i].GetRect();
		if(data.item[i].IsClick(p) || data.item[i].IsTextClick(p)) {
			double a = r.Width() * r.Height();
			if(a < mina) {
				mina = a;
				mini = i;
			}
		}
	}
	return mini;
}

Image DiagramEditor::CursorImage(Point p, dword keyflags)
{
	Map(p);

	for(int i = 0; i < data.item.GetCount(); i++)
		if(data.item[i].IsTextClick(p))
			return Image::IBeam();
	
	int i = FindItem(p);

	Point h = HasCapture() ? draghandle : IsCursor() ? GetHandle(cursor, p) : Null;

	if(IsNull(h))
		return Image::Arrow();
	
	if(HasCapture() && i >= 0 && data.item[i].IsLine())
		return Image::Arrow();

	int m = h.x * h.y;
	if((h.x || h.y) && i >= 0 && data.item[i].IsLine())
		return Image::SizeAll();
	if(m > 0)
		return Image::SizeBottomRight();
	if(m < 0)
		return Image::SizeBottomLeft();
	if(h.x)
		return Image::SizeHorz();
	if(h.y)
		return Image::SizeVert();

	return Image::Arrow();
}

void DiagramEditor::LeftDouble(Point p, dword keyflags)
{
	if(IsCursor())
		StartText();
}

void DiagramEditor::LeftDown(Point p, dword keyflags)
{
	Map(p);

	FinishText();
	dragstart = dragcurrent = p;

	SetCapture();

	if(IsCursor()) { // resize
		Point h = GetHandle(cursor, p);
		if(h.x || h.y) {
			draghandle = h;
			Sync();
			return;
		}
	}

	newitem = false;
	int i = FindItem(p);
	if(i >= 0) { // start moving selection
		SetCursor(i);
		dragfrom = GetCursorRect();
		if(dragfrom.Contains(p)) {
			sdragfrom.SetCount(sel.GetCount());
			for(int i = 0; i < sel.GetCount(); i++)
				sdragfrom[i] = data.item[sel[i]];
			draghandle = Null;
		}
	}
	else
	if(mode < 0) { // rectangular selection
		sel.Clear();
		doselection = true;
	}
	else { // add new item
		CancelSelection();
		DiagramItem& m = data.item.Add();
		m.p1 = m.p2 = p;
		m.shape = mode;
		dragstart = p;
		draghandle = Point(1, 1);
		newitem = true;
		SetAttrs();
		SetCursor(data.item.GetCount() - 1);
		Sync();
	}

	Sync();
}

void DiagramEditor::MouseMove(Point p, dword keyflags)
{
	Map(p);

	if(HasCapture() && doselection) { // do rectangular selection
		dragcurrent = p;
		Rect r(dragstart, dragcurrent);
		r.Normalize();
		sel.Clear();
		KillCursor();
		for(int i = 0; i < data.item.GetCount(); i++)
			if(r.Contains(data.item[i].p1) && r.Contains(data.item[i].p2)) {
				sel.FindAdd(i);
				SetCursor(i);
			}
		Sync();
		return;
	}
	if(HasCapture() && IsCursor()) {
		DiagramItem& m = CursorItem();
		if(grid)
			p = m.IsLine() ? p / 16 * 16 : p / 8 * 8;
		if(IsNull(draghandle)) { // move selection
			Point offset = p - dragstart;
			Rect to = dragfrom.Offseted(offset);
			m.p1 = to.TopLeft();
			m.p2 = to.BottomRight();
			for(int i = 0; i < sel.GetCount(); i++) {
				int ii = sel[i];
				if(ii >= 0 && ii < data.item.GetCount() && i < sdragfrom.GetCount()) {
					(Point2 &)data.item[ii] = sdragfrom[i].Offseted(offset);
					data.item[ii].FixPosition();
				}
			}
		}
		else {
			if(draghandle.x)
				(draghandle.x < 0 ? m.p1.x : m.p2.x) = p.x;
			if(draghandle.y)
				(draghandle.y < 0 ? m.p1.y : m.p2.y) = p.y;
			DDUMP(m.GetRect());
		}
		m.FixPosition();
		Sync();
	}
}

void DiagramEditor::LeftUp(Point p, dword keyflags)
{
	Map(p);

	Sync();
	if(!doselection && IsCursor() && newitem) { // avoid empty items
		DiagramItem& m = CursorItem();
		if(Distance(m.p1, m.p2) < 4) {
			m.p1 -= Pointf(32, 32);
			m.p2 += Pointf(32, 32);
			m.FixPosition();
		}
	}
	doselection = false;
	Commit();
	if(Distance(dragstart, p) < 2) {
		if((keyflags & K_CTRL) == 0) {
			sel.Clear();
			sel.FindAdd(cursor);
		}
		if(CursorItem().IsTextClick(p) && !newitem)
			StartText();
	}
	newitem = false;
}

void DiagramEditor::RightDown(Point p, dword keyflags)
{
	Map(p);
	
	FinishText();
	Sync();
}

void DiagramEditor::RightUp(Point, dword keyflags)
{
	Commit();
}

}