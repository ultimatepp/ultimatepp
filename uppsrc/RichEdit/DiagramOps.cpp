#include "RichEdit.h"

namespace Upp {

void DiagramEditor::SetCursor(int i)
{
	cursor = i;
	if(i < 0) {
		sel.Clear();
		return;
	}
	sel.FindAdd(i);
	GetAttrs();
}

DiagramItem& DiagramEditor::CursorItem()
{
	static DiagramItem nil;
	if(cursor >= 0 && cursor < data.item.GetCount())
		return data.item[cursor];
	return nil;
}

DiagramItem& DiagramEditor::AddItem(int shape)
{
	int i = data.item.GetCount();
	if(shape == 0) { // insert lines before shapes
		i = 0;
		while(i < data.item.GetCount() && data.item[i].IsLine())
			i++;
		data.item.Insert(i);
	}
	DiagramItem& m = data.item.At(i);
	SetCursor(i);
	return m;
}

void DiagramEditor::SetAttrs(DiagramItem& m, dword attrs)
{
	if(attrs & ATTR_SHAPE)
		m.shape = ~shape;
	if(attrs & ATTR_CAP0)
		m.cap[0] = ~line_start;
	if(attrs & ATTR_CAP1)
		m.cap[1] = ~line_end;
	if(attrs & ATTR_WIDTH)
		m.width = ~line_width;
	if(attrs & ATTR_DASH)
		m.dash = ~line_dash;
	if(attrs & ATTR_INK)
		m.ink = ~ink;
	if(attrs & ATTR_PAPER)
		m.paper = ~paper;
}

void DiagramEditor::SetAttrs(dword attrs)
{
	for(int i = 0; i < sel.GetCount(); i++)
		SetAttrs(data.item[sel[i]], attrs);
	if(tool >= 0)
		SetAttrs(tl[tool], attrs);
	Sync();
	Commit();
}

void DiagramEditor::GetAttrs(const DiagramItem& m)
{
	shape <<= m.shape;
	line_start <<= m.cap[0];
	line_end <<= m.cap[1];
	line_width <<= m.width;
	line_dash <<= m.dash;
	ink <<= m.ink;
	paper <<= m.paper;
}


void DiagramEditor::GetAttrs()
{
	GetAttrs(CursorItem());
}

void DiagramEditor::MoveFrontBack(bool back)
{
	FinishText();
	Array<DiagramItem> item;
	Index<int> newsel;
	int        newcursor = -1;
	for(int pass = 0; pass < 2; pass++)
		for(int i = 0; i < data.item.GetCount(); i++) {
			bool isel = sel.Find(i) >= 0;
			bool b = pass;
			if(back)
				b = !b;
			if(isel == b) {
				int ii = item.GetCount();
				if(isel)
					newsel.Add(ii);
				if(cursor == i)
					newcursor = ii;
				item.Add(data.item[i]);
			}
		}
	data.item = pick(item);
	sel = pick(newsel);
	cursor = newcursor;
	Sync();
}

void DiagramEditor::Align(bool horz, int align)
{
	FinishText();

	if(!IsCursor())
		return;

	auto HoVe = [&](Pointf& p) -> double& {
		return horz ? p.x : p.y;
	};

	Rect cr = GetCursorRect();
	Pointf cp1 = cr.TopLeft();
	Pointf cp2 = cr.BottomRight();
	double csz = HoVe(cp2) - HoVe(cp1);
	for(int ii : sel) {
		if(ii != cursor) {
			DiagramItem& m = data.item[ii];
			m.Normalize();
			Pointf& p1 = m.pt[0];
			Pointf& p2 = m.pt[1];
			double sz = abs(HoVe(p2) - HoVe(p1));
			if(align == ALIGN_LEFT) {
				HoVe(p1) = HoVe(cp1);
				HoVe(p2) = HoVe(cp1) + sz;
			}
			if(align == ALIGN_RIGHT) {
				HoVe(p2) = HoVe(cp2);
				HoVe(p1) = HoVe(cp2) - sz;
			}
			if(align == ALIGN_CENTER) {
				HoVe(p1) = HoVe(cp1) + csz / 2 - sz / 2;
				HoVe(p2) = HoVe(p1) + sz;
			}
			if(align == ALIGN_JUSTIFY)
				HoVe(p2) = HoVe(p1) + csz;
		}
	}
	Commit();
}

void DiagramEditor::PrepareConns()
{
	conns.Clear();
	VectorMap<Pointf, Vector<Tuple<int, int>>> map;
	for(int pass = 0; pass < 2; pass++)
		for(int i = 0; i < data.item.GetCount(); i++) {
			const DiagramItem& m = data.item[i];
			if(m.IsLine() == pass) {
				Vector<Pointf> cp = m.GetConnections();
				for(int j = 0; j < cp.GetCount(); j++)
					map.GetAdd(cp[j]) << MakeTuple(i, j);
			}
		}
	for(int i = 0; i < data.item.GetCount(); i++) {
		const DiagramItem& m = data.item[i];
		if(m.IsLine()) {
			for(int j = 0; j < 2; j++) {
				auto *q = map.FindPtr(m.pt[j]);
				if(q) {
					for(auto w : *q) {
						Cn& c = conns.Add();
						c.mi = w.a;
						c.ci = w.b;
						c.li = i;
						c.pi = j;
					}
				}
			}
		}
	}
}

void DiagramEditor::UseConns()
{
	for(const Cn& cn: conns)
		if(sel.Find(cn.li) < 0)
			data.item[cn.li].pt[cn.pi] = data.item[cn.mi].GetConnections()[cn.ci];
}



/* TODO remove

struct SizeDlg : WithSizeLayout<TopWindow> {
	SizeDlg();

	static Size sz[];

	void Set(Size sz);
	Size Get() const;
	void Sync();
};

Size SizeDlg::sz[] =
{
	Size(1250, 1000),
	Size(1600, 900),
	Size(1600, 800),
	Size(1000, 1250),
};

Size SizeDlg::Get() const
{
	int q = ~size;
	if(q >= 0 && q < __countof(sz)) {
		return sz[q];
	}
	auto cl = [](int x) { return clamp(x, 64, 10000); };
	return Size(cl(~cx), cl(~cy));
}

void SizeDlg::Sync()
{
	Size sz = Get();
	cx <<= sz.cx;
	cy <<= sz.cy;
	bool b = ~size == 4;
	cx.Enable(b);
	cy.Enable(b);
}

void SizeDlg::Set(Size sz)
{
	cx <<= sz.cx;
	cy <<= sz.cy;
	int i = FindIndex(SubRange(sz, __countof(sz)), sz);
	size = i < 0 ? 4 : i;
	Sync();
}

SizeDlg::SizeDlg()
{
	CtrlLayoutOKCancel(*this, "Canvas size");
	size << [=] { Sync(); };
}

void DiagramEditor::ChangeSize()
{
	SizeDlg dlg;
	dlg.Set(data.sz);
	if(dlg.ExecuteOK()) {
		data.sz = dlg.Get();
		Commit();
	}
}
*/
}