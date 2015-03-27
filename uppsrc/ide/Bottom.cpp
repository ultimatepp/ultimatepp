#include "ide.h"

RightTabs::RightTabs()
{
	cx = 3;
	cursor = -1;
	Transparent();
}

void RightTabs::Clear()
{
	tab.Clear();
	cx = 3;
	cursor = -1;
	Refresh();
}

void RightTabs::Repos()
{
	int y = 0;
	for(int i = 0; i < tab.GetCount(); i++) {
		tab[i].y = y;
		y += tab[i].GetHeight();
	}
	RefreshParentLayout();
}

void RightTabs::Add(const Image& img, const String& tip)
{
	Tab& t = tab.Add();
	t.img = img;
	t.tip = tip;
	cx = max(cx, img.GetSize().cx + 7);
	Repos();
}

void RightTabs::PaintTab(Draw& w, int x, int y, int cx, int cy, Color paper, const Image& img, Color hl)
{
	Color fc = FieldFrameColor();
	w.DrawRect(x, y + 1, cx - 1, cy - 2, paper);
	w.DrawRect(x, y, cx - 1, 1, fc);
	w.DrawRect(x + cx - 1, y + 1, 1, cy - 2, fc);
	w.DrawRect(x, y + cy - 1, cx - 1, 1, fc);
	Size isz = img.GetSize();
	int ix = (cx - isz.cx) / 2 + x;
	int iy = (cy - isz.cx) / 2 + y;
	if(!IsNull(hl)) {
		w.DrawImage(ix - 1, iy - 1, img, hl);
		w.DrawImage(ix - 1, iy + 1, img, hl);
		w.DrawImage(ix + 1, iy - 1, img, hl);
		w.DrawImage(ix + 1, iy + 1, img, hl);
	}
	w.DrawImage(ix, iy, img);
}

void RightTabs::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, Blend(SColorFace, SColorPaper));
	Color hc = Blend(Yellow, LtRed, 100);
	Color inactive = Blend(SColorPaper, SColorShadow);
	for(int i = 0; i < tab.GetCount(); i++) {
		Tab& t = tab[i];
		if(i != cursor)
			PaintTab(w, 0, t.y + 2, cx - 1, t.GetHeight() - 1, inactive, t.img, i == hl ? hc : Null);
	}
	if(cursor >= 0) {
		Tab& t = tab[cursor];
		PaintTab(w, 0, t.y, cx, t.GetHeight() + 3, SColorPaper, t.img, cursor == hl ? hc : Null);
	}
}

int RightTabs::GetPos(Point p)
{
	for(int i = 0; i < tab.GetCount(); i++)
		if(p.y < tab[i].GetRight())
			return i;
	return -1;
}

void RightTabs::SetCursor(int i)
{
	ASSERT(cursor < tab.GetCount());
	cursor = i;
	Refresh();
}

void RightTabs::LeftDown(Point p, dword)
{
	int c = GetPos(p);
	if(c >= 0 && c != cursor) {
		cursor = c;
		UpdateActionRefresh();
		if(GetParent())
			GetParent()->RefreshFrame();
	}
}

void RightTabs::MouseMove(Point p, dword)
{
	int c = GetPos(p);
	if(c != hl) {
		hl = c;
		Refresh();
		Tip(c >= 0 ? tab[c].tip : "");
	}
}

void RightTabs::MouseLeave()
{
	CancelMode();
}

void RightTabs::CancelMode()
{
	hl = -1;
	Refresh();
	if(GetParent())
		GetParent()->RefreshFrame();
}

void RightTabs::FramePaint(Draw& w, const Rect& rr)
{
	Rect r = rr;
	r.right -= cx;
	DrawFrame(w, r, FieldFrameColor());
	DrawFrame(w, r.Deflated(1), SColorPaper);
	if(cursor >= 0) {
		Tab& t = tab[cursor];
		w.DrawRect(r.right - 1, t.y + 1, 1, t.GetHeight() + 1, SColorFace);
	}
}

void RightTabs::FrameLayout(Rect& r)
{
	LayoutFrameRight(r, this, cx);
	r.top += 2;
	r.right -= 2;
	r.left += 2;
	r.bottom -= 2;
}

void RightTabs::FrameAddSize(Size& sz)
{
	sz += 4;
	sz.cx += cx + 2;
}

void Ide::BTabs()
{
	btabs.Clear();
	btabs.Add(IdeImg::close, "Close (Esc)");
	btabs.Add(IdeImg::console, "Console");
	btabs.Add(IdeImg::errors, "Errors");
	btabs.Add(IdeImg::console2, "Find in files");
	btabs.Add(IdeImg::calc, "Calculator");
	if(bottomctrl)
		btabs.Add(IdeImg::debug, "Debug");
}

void Ide::SyncBottom()
{
	int q = btabs.GetCursor();
	if(q == BCLOSE) {
		HideBottom();
		return;
	}
	if(editor_bottom.GetZoom() >= 0)
		editor_bottom.NoZoom();
	console.Show(q == BCONSOLE);
	error.Show(q == BERRORS);
	ffound.Show(q == BFINDINFILES);
	calc.Show(q == BCALC);
	if(bottomctrl)
		bottomctrl->Show(q == BDEBUG);
	calc.LoadHlStyles(editor.StoreHlStyles());
	calc.SetFont(editorfont);
	SetBar();
	if(q == BCALC)
		ActiveFocus(calc);
}

void Ide::SetBottom(int i)
{
	btabs.SetCursor(i);
	SyncBottom();
}

void Ide::ShowBottom(int i)
{
	if(i == btabs.GetCursor() && IsBottomShown())
		return;
	SetBottom(i);
}

void Ide::ToggleBottom(int i)
{
	if(i == btabs.GetCursor() && IsBottomShown())
		HideBottom();
	else
		SetBottom(i);
}

bool Ide::IsBottomShown() const
{
	return editor_bottom.GetZoom() < 0;
}

void Ide::SwapBottom()
{
	if(editor.Esc())
		return;
	if(editor.IsFindOpen())
		editor.FindClose();
	else
	if(!designer && !editor.HasFocusDeep())
		ActiveFocus(editor);
	else
	if(IsBottomShown())
		HideBottom();
	else {
		if(btabs.GetCursor() <= 0)
			btabs.SetCursor(1);
		SyncBottom();
	}
}

void Ide::HideBottom() {
	editor_bottom.Zoom(0);
	editor.SetWantFocus();
	SetBar();
}

void Ide::IdeSetBottom(Ctrl& ctrl)
{
	bottom.Add(ctrl.SizePos());
	bottomctrl = &ctrl;
	BTabs();
	SetBottom(BDEBUG);
}

void Ide::IdeRemoveBottom(Ctrl& ctrl)
{
	ctrl.Remove();
	bottomctrl = NULL;
	BTabs();
	HideBottom();
}

void Ide::IdeActivateBottom()
{
	SetBottom(BDEBUG);
}
