#include "ide.h"

INITBLOCK {
	RegisterGlobalConfig("NavDlg2");
}

struct NavDlg : WithJumpLayout<TopWindow>, Navigator {
	typedef NavDlg CLASSNAME;
	
	virtual bool Key(dword key, int count);

	virtual int GetCurrentLine();
	
	void GoTo();
	void Ok()               { if(navlines.IsCursor()) Break(IDOK); }
	void ListSel();
	
	void Serialize(Stream& s);

	struct NavLine : Moveable<NavLine> {
		String path;
		Point  pos;
		bool   definition;
		
		bool operator<(const NavLine& b) const;
	};

	struct LineDisplay : Display {
		int DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style, int x) const;
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
		virtual Size GetStdSize(const Value& q) const;
	};

	NavDlg();
};

NavDlg::NavDlg()
{
	CtrlLayoutOKCancel(*this, "Navigator");
	dlgmode = true;
	search.WhenEnter.Clear();
	Sizeable().Zoomable();
	Icon(IdeImg::Navigator());
	list.WhenSel << THISBACK(ListSel);
	list.WhenLeftDouble = THISBACK(Ok);
	navlines.NoHeader().NoGrid();
	navlines.AddColumn().SetDisplay(Single<LineDisplay>());
	navlines.WhenLeftDouble = THISBACK(Ok);
}

void PaintTeXt(Draw& w, int& x, int y, const String& text, Font font, Color ink);

int NavDlg::LineDisplay::DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style, int x) const
{
	w.DrawRect(r, paper);
	const NavLine& l = q.To<NavLine>();
	x += r.left;
	String p = l.path;
	int y = r.top + (r.GetHeight() - StdFont().GetCy()) / 2;
	PaintTeXt(w, x, y, GetFileName(GetFileFolder(p)) + "/", StdFont(), ink);
	PaintTeXt(w, x, y, GetFileName(p), StdFont().Bold(), ink);
	PaintTeXt(w, x, y, " (", StdFont(), ink);
	PaintTeXt(w, x, y, AsString(l.pos.y), StdFont().Bold(), ink);
	PaintTeXt(w, x, y, ")", StdFont(), ink);
	return x - r.left;
}

void NavDlg::LineDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	DoPaint(w, r, q, ink, paper, style, min(r.GetWidth() - GetStdSize(q).cx, 0));
}

Size NavDlg::LineDisplay::GetStdSize(const Value& q) const
{
	NilDraw w;
	return Size(DoPaint(w, Size(999999, 999999), q, White(), White(), 0, 0), StdFont().Bold().GetCy());
}

bool NavDlg::NavLine::operator<(const NavLine& b) const
{
	return CombineCompare(GetFileExt(b.path), GetFileExt(path)) // .h > .c
	                     (GetFileName(path), GetFileName(b.path))
	                     (path, b.path)
	                     (pos.y, b.pos.y)
	                     (pos.x, b.pos.x) < 0;
}

void NavDlg::ListSel()
{
	int ii = list.GetCursor();
	if(theide && ii >= 0 && ii < litem.GetCount()) {
		const NavItem& cm = *litem[ii];
		Vector<NavLine> set;
		for(const auto& f : ~CodeIndex())
			for(const AnnotationItem& m : f.value.items) {
				if(m.id == cm.id) {
					NavLine& nl = set.Add();
					nl.path = f.key;
					nl.pos = m.pos;
					nl.definition = m.definition;
				}
				if(set.GetCount() > 1000) // sanity
					break;
			}
		Sort(set);
		navlines.Clear();
		for(const NavLine& l : set)
			navlines.Add(RawToValue(l));
		navlines.GoBegin();
	}
}

void NavDlg::Serialize(Stream& s)
{
	SerializePlacement(s);
	search.SerializeList(s);
}

bool NavDlg::Key(dword key, int count)
{
	if(key == K_UP || key == K_DOWN) {
		if(list.IsCursor())
			return list.Key(key, count);
		else
			list.GoBegin();
		return true;
	}
	return TopWindow::Key(key, count);
}

void NavDlg::GoTo()
{
	if(navlines.IsCursor()) {
		const NavLine& l = navlines.Get(0).To<NavLine>();
		theide->GotoPos(l.path, l.pos);
	}
}

int NavDlg::GetCurrentLine()
{
	return theide->editor.GetCurrentLine();
}

void Ide::NavigatorDlg()
{
	if(!editor.WaitCurrentFile())
		return;
	NavDlg dlg;
	LoadFromGlobal(dlg, "NavDlg");
	dlg.theide = this;
	dlg.Search();
	if(dlg.ExecuteOK())
		dlg.GoTo();
	dlg.search.AddHistory();
	StoreToGlobal(dlg, "NavDlg");
}
