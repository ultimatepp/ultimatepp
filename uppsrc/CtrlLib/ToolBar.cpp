#include "CtrlLib.h"

namespace Upp {

#define LTIMING(x)  // TIMING(x)

CH_STYLE(ToolBar, Style, StyleDefault)
{
	buttonstyle = ToolButton::StyleDefault();
	buttonminsize = Size(16, 16);
	maxiconsize = Size(INT_MAX, INT_MAX);
	arealook = Null;
	look = SColorFace();
	breaksep = separator = SeparatorCtrl::StyleDefault();
}

ToolBar::ToolBar()
{
	pane.Margin(2, 3);
	lsepm = rsepm = 0;
	ssize = 8;
	lock = 0;
	ii = 0;
	SetStyle(StyleDefault());
	buttonminsize = Null;
	maxiconsize = Null;
	kind = ToolButton::NOLABEL;
	arealook = -1;
	nodarkadjust = false;
}

ToolBar::~ToolBar() {}

void PaintBarArea(Draw& w, Ctrl *x, const Value& look, int bottom)
{
	Ctrl *tc = x->GetTopCtrl();
	Rect sr = tc->GetScreenRect();
	sr.bottom = Nvl(bottom, tc->GetScreenView().top);
	sr.Offset(-x->GetScreenRect().TopLeft());
	ChPaint(w, sr, look);
}

void ToolBar::Paint(Draw& w)
{
	if(IsTransparent())
		return;
	Value look = style->look;
	if(!IsNull(style->arealook) && (arealook < 0 ? InFrame() : arealook)) {
		PaintBarArea(w, this, style->arealook, GetScreenRect().bottom);
		look = Null;
	}
	PaintBar(w, style->breaksep, look);
}

Bar::Item& ToolBar::AddItem(Event<> cb)
{
	ToolButton& m = item.At(ii++);
	m.ResetKeepStyle();
	m.WhenAction = cb;
	m.MinSize(IsNull(buttonminsize) ? style->buttonminsize : buttonminsize);
	m.MaxIconSize(IsNull(maxiconsize) ? style->maxiconsize : maxiconsize);
	m.Kind(kind);
	m.SetStyle(style->buttonstyle);
	m.NoDarkAdjust(nodarkadjust);
	m.Enable(true).Check(false).Bar::Item::Key((dword)0);
	m.Text("");
	m.Tip("");
	m.ClearInfo();
	pane.Add(&m, Null);
	return m;
}

Bar::Item& ToolBar::AddSubMenu(Event<Bar&> proc)
{
	if(item.GetCount())
		Separator();
	proc(*this);
	return NilItem();
}

void ToolBar::Clear()
{
	item.Clear();
	BarCtrl::Clear();
}

bool ToolBar::HotKey(dword key)
{
	return Bar::Scan(proc, key) || Ctrl::HotKey(key);
}

void ToolBar::Set(Event<Bar&> bar)
{
	LTIMING("SetToolBar");
	if(lock) return;
	lock++;
	BarCtrl::IClear();
	ii = 0;
	bar(*this);
	proc = bar;
	item.SetCount(ii);
	for(int i = 0; i < item.GetCount(); i++)
		item[i].FinalSync();
	BarCtrl::IFinish();
	SyncBar();
	lock--;
}

void ToolBar::Post(Event<Bar&> bar)
{
	KillTimeCallback(TIMEID_POST);
	SetTimeCallback(0, PTEBACK1(Set, bar), TIMEID_POST);
}

int ToolBar::GetStdHeight()
{
	Size sz = StyleDefault().maxiconsize;
	return sz.cy > 10000 ? DPI(22) + 3 : sz.cy + DPI(6) + 3;
}

StaticBarArea::StaticBarArea()
{
	upperframe = true;
}

void StaticBarArea::Paint(Draw& w)
{
	if(IsNull(ToolBar().StyleDefault().arealook))
		ChPaint(w, GetSize(), ToolBar().StyleDefault().look);
	else
		PaintBarArea(w, this, ToolBar().StyleDefault().arealook,
		             upperframe ? Null : GetScreenRect().bottom);
}

}
