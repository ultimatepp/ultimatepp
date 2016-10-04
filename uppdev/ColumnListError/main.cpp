#include "ColumnListError.h"

MyList::MyList() {
	SetDisplay(*this);
}

void MyList::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const {
	if(style & CURSOR)
		w.DrawRect(r,SLtGray());
	else
		w.DrawRect(r,SWhite());
	w.DrawText(r.left,r.top,q.ToString());
}

Size MyList::GetStdSize() {
	Size sz;
	sz.cx=300;
	sz.cy=40;
	return sz;
}

ListViewer::ListViewer() {
//	Add(tb);
	Add(cl);
	cl.WhenEnterItem = THISBACK(DoNothing);
	UpdateToolbar();
}

void ListViewer::Layout() {
	Size sz=GetSize();
	tb.SetRect(0,0,sz.cx,50);
	cl.SetRect(0,60,100,sz.cy-60);
}

void ListViewer::Paint(Draw& draw) {
	Size sz=GetSize();
	draw.DrawRect(0,0,sz.cx,sz.cy,SBlue);
	if(!cl.IsCursor())
		cl.SetCursor(0);
}
void ListViewer::SetToolBar(Bar& tb) {
	tb.Add(true,"Hello test",ColumnListErrorImg::A(),THISBACK(DoNothing));
}

ColumnListError::ColumnListError()
{
//	AddFrame(tb);
	Add(cl);
	Add(lv);
	lv.Show();
	cl.Hide();
	SetRect(0,0,400,600);
	UpdateToolbar();
}
void ColumnListError::AddEntries() {
	lv.InsertCL("0");
	lv.InsertCL("1");
	lv.InsertCL("2");
	lv.InsertCL("3");
}
void ColumnListError::SetToolBar(Bar& tb) {
	tb.Add(true,"Hello test",ColumnListErrorImg::A(),THISBACK(ShowCl));
}
void ColumnListError::Layout() {
	Size sz = GetSize();
	cl.SetRect(0,60,100,sz.cy);
	lv.SetRect(0,0,sz.cx,sz.cy);
}
void ColumnListError::ShowCl() {
	cl.Show(!cl.IsShown());
	lv.Show(!lv.IsShown());
}
void ColumnListError::Paint(Draw& w) {
	Size sz=GetSize();
	if(!cl.IsCursor())
		cl.SetCursor(0);
	w.DrawRect(0,0,sz.cx,sz.cy,SColorPaper);
}

GUI_APP_MAIN
{
	ColumnListError cle;
	cle.AddEntries();
	cle.Run();
}
