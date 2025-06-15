#include "RichEdit.h"

namespace Upp {

void DiagramEditor::SetAttrs()
{
	for(int i = 0; i < sel.GetCount(); i++) {
		DiagramItem& m = data.item[sel[i]];
		m.shape = ~shape;
		m.line_start = ~line_start;
		m.line_end = ~line_end;
		m.width = ~line_width;
		m.dash = ~line_dash;
		m.ink = ~ink;
		m.paper = ~paper;
	}
	Sync();
	Commit();
}

void DiagramEditor::GetAttrs()
{
	DiagramItem& m = CursorItem();
	shape <<= m.shape;
	line_start <<= m.line_start;
	line_end <<= m.line_end;
	line_width <<= m.width;
	line_dash <<= m.dash;
	ink <<= m.ink;
	paper <<= m.paper;
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
			Pointf& p1 = m.p1;
			Pointf& p2 = m.p2;
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