#include "CtrlCore.h"

#define LLOG(x) DLOG(x)

namespace Upp {

void (*Ctrl::cancel_preedit)(); // hook for implementation to hide cancel host preedit

void Ctrl::CancelPreedit()
{
	HidePreedit();
	if(cancel_preedit)
		cancel_preedit();
}

struct PreeditCtrl : Ctrl {
	WString text;
	Font    font;
	Ctrl   *owner = NULL;
	int     cursor = INT_MAX;

	virtual void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(GetSize(), SWhite());
		w.DrawText(DPI(2), sz.cy - font.GetCy(), text, font, SBlack());
		if(cursor < text.GetCount())
			w.DrawRect(DPI(2) + GetTextSize(text.Mid(0, cursor), font).cx, 0, DPI(1), sz.cy, InvertColor);
	}
	
	PreeditCtrl() { SetFrame(BlackFrame()); }
};

Rect Ctrl::GetPreeditScreenRect()
{ // preedit position in screen coordinates, zero width
	if(HasFocusDeep()) {
		Point p = focusCtrl->GetPreedit();
		if(!IsNull(p)) {
			p += focusCtrl->GetScreenView().TopLeft();
			return RectC(p.x, p.y - 1, 0, focusCtrl->GetPreeditFont().GetCy() + 1);
		}
	}
	return Null;
}

Point Ctrl::GetPreedit()
{
	if(HasFocus()) {
		Rect r = GetCaret();
		if(r.GetHeight() > 0)
			return r.TopRight();
	}
	return Null;
}

Font Ctrl::GetPreeditFont()
{
	static int pheight = -1;
	static Font pfont;
	if(!focusCtrl)
		return StdFont();
	int height = max(focusCtrl->GetCaret().GetHeight(), DPI(7));
	if(height != pheight) {
		pheight = height;
		while(pheight > 0) {
			pfont = StdFont(height);
			if(pfont.GetCy() < pheight)
				break;
			height--;
		}
		if(height == 0)
			pfont = StdFont();
	}
	return pfont;
}

void Ctrl::SyncPreedit()
{
	PreeditCtrl& p = Single<PreeditCtrl>();
	if(p.owner == this && focusCtrl) {
		Rect r = GetPreeditScreenRect();
		p.font = focusCtrl->GetPreeditFont();
		r.right = r.left + GetTextSize(p.text, p.font).cx + DPI(4);
		int wr = GetWorkArea().right;
		if(r.right > wr) {
			int w = r.GetWidth();
			r.right = min(wr, r.left - DPI(2));
			r.left = r.right - w;
		}
		p.SetRect(r);
	}
}

void Ctrl::ShowPreedit(const WString& text, int cursor)
{
	if(text.GetCount() == 0) {
		HidePreedit();
		return;
	}
	PreeditCtrl& p = Single<PreeditCtrl>();
	p.text = text;
	p.cursor = cursor;
	p.owner = this;
	SyncPreedit();
	if(!p.IsOpen())
		p.PopUp(this, true, false, true);
	p.Refresh();
}

void Ctrl::HidePreedit()
{
	PreeditCtrl& p = Single<PreeditCtrl>();
	if(p.IsOpen()) {
		p.Close();
		p.owner = NULL;
	}
}

void Ctrl::PreeditSync(void (*enable_preedit)(Ctrl *top, bool enable))
{ // enables / disables preedit
	static Ptr<Ctrl> preedit;
	Ctrl *fw = focusCtrl && !IsNull(focusCtrl->GetPreedit()) ? focusCtrl->GetTopCtrl() : nullptr;
	if(fw != preedit) {
		if(preedit)
			enable_preedit(preedit, false);
		if(fw)
			enable_preedit(fw, true);
	}
	preedit = fw;
}

};