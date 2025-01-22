#include "CtrlLib.h"

namespace Upp {

#define LLOG(x)  // DLOG(x)

ToolTip::ToolTip()
{
	SetFrame(BlackFrame());
}

Size ToolTip::GetMinSize() const
{
	return AddFrameSize(GetSmartTextSize(text, StdFont(), GetWorkArea().GetWidth() / 2) + 4);
}

void ToolTip::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, SColorInfo);
	DrawSmartText(w, 2, 2, sz.cx - 4, text, StdFont(), SColorInfoText(), 0, Null, IsDark(SColorInfo()));
}

void ToolTip::PopUp(Ctrl *owner, Point p, bool effect)
{
	LLOG("ToolTip::PopUp" << Desc(owner) << " @ " << p);
	Rect r = GetMouseWorkArea();
	Size sz = GetMinSize();
	p.x = max(p.x + sz.cx > r.right ? r.right - sz.cx : p.x, r.left);
	p.y = max(p.y + sz.cy > r.bottom ? r.bottom - sz.cy : p.y, r.top);
	if(GUI_PopUpEffect() == GUIEFFECT_SLIDE && effect)
		SetRect(p.x, p.y, sz.cx, 1);
	else
		SetRect(p.x, p.y, sz.cx, sz.cy);
	Ctrl::PopUp(owner, true, false, true);
	if(effect)
		Animate(*this, p.x, p.y, sz.cx, sz.cy);
}

Ptr<Ctrl> tipctrl;
bool      showmode;

ToolTip& AppToolTip()
{
	static ToolTip tip;
	return tip;
}

void CloseToolTip()
{
	ToolTip& q = AppToolTip();
	if(q.IsOpen()) {
		LLOG("CLOSE: CloseToolTip " << q.Get());
		Ctrl::Call([] { AppToolTip().Close(); }); // we can be called from another thread's disable of something
	}
}

void EndShowMode()
{
	showmode = false;
}

void ShowToolTip()
{
	LLOG("ShowToolTip");
	CloseToolTip();
	if(!GUI_ToolTips())
		return;
	if(tipctrl) {
		String text = tipctrl->GetTip();
		LLOG("-> showing tip: " << text << " tipctrl: " << UPP::Name(tipctrl));
		Ctrl *top = tipctrl->GetTopCtrl();
		ToolTip& q = AppToolTip();
		q.Set(text);
		if(text.GetCount() && top && (top->IsForeground() || top->IsPopUp())) {
			LLOG("-> foreground");
			Size sz = q.GetMinSize();
			Rect r = Ctrl::GetMouseWorkArea();
			Point p = GetMousePos() + Size(0, DPI(22));
			if(p.y + sz.cy > r.bottom)
				p = GetMousePos() - Size(0, DPI(6)) - sz.cy;
			q.PopUp(top, p, !showmode);
			showmode = true;
			KillTimeCallback((void *)EndShowMode);
			return;
		}
		LLOG("-> background / empty text, top = " << UPP::Name(top));
	}
	SetTimeCallback(500, callback(EndShowMode), (void *)EndShowMode);
}

void SyncToolTip(Ctrl *ctrl)
{
	if(!GUI_ToolTips())
		return;
	if(ctrl != tipctrl || ctrl && ctrl->GetTip() != AppToolTip().Get()) {
		LLOG("ToolTipHook / ctrl change " << UPP::Name(ctrl) << " -> " << UPP::Name(ctrl));
		tipctrl = ctrl;
		KillTimeCallback((void *)SyncToolTip);
		if(showmode)
			ShowToolTip();
		else {
			LLOG("ToolTipHook -> SetTimeCallback");
			SetTimeCallback(GUI_ToolTipDelay(), callback(ShowToolTip), (void *)SyncToolTip);
		}
	}
}

bool ToolTipHook(Ctrl *ctrl, bool inframe, int event, Point p, int zdelta, dword keyflags)
{
	if(event == Ctrl::MOUSEMOVE && ctrl != &AppToolTip())
	{
		SyncToolTip(Ctrl::GetVisibleChild(ctrl, p, inframe));
		LLOG("ToolTipHook MOUSEMOVE " << UPP::Name(ctrl) << " inframe: " << inframe);
	}
	if(event == Ctrl::MOUSELEAVE) {
		LLOG("ToolTipHook MOUSELEAVE " << UPP::Name(ctrl));
		CloseToolTip();
		KillTimeCallback((void *)EndShowMode);
		SetTimeCallback(1000, callback(EndShowMode), (void *)EndShowMode);
	}
	if((event & Ctrl::ACTION) == Ctrl::DOWN) {
		LLOG("ToolTipHook DOWN");
		CloseToolTip();
		EndShowMode();
	}
	return false;
}

bool ToolTipStateHook(Ctrl *ctrl, int reason)
{
	if((reason == Ctrl::CLOSE || reason == Ctrl::ENABLE || reason == Ctrl::DEACTIVATE)
	   && ctrl != &AppToolTip() && AppToolTip().IsOpen()) {
		LLOG("ToolTipStateHook -> close, reason: " << reason);
		CloseToolTip();
		EndShowMode();
	}
	return false;
}

INITBLOCK {
	LLOG("Ctrl::InstallMouseHook(ToolTipHook)");
	Ctrl::InstallMouseHook(ToolTipHook);
	Ctrl::InstallStateHook(ToolTipStateHook);
}

void LinkToolTipIn__() {}  // "Call" from elsewhere to force linking this module in...

class QTFPopUp : public RichTextCtrl {
public:
	virtual void Deactivate();
	virtual void CancelMode();

protected:
	int  width;
	int  maxheight;
	bool open;

	void DoClose();

public:
	void      PopUp(Ctrl *parent);

	QTFPopUp& Width(int _width)                   { width = _width; return *this; }
	QTFPopUp& MaxHeight(int _maxheight)           { maxheight = _maxheight; return *this; }

	QTFPopUp();
};

void QTFPopUp::Deactivate() {
	if(open)
		DoClose();
}

void QTFPopUp::CancelMode() {
	DoClose();
}

void QTFPopUp::DoClose() {
	if(open) {
		open = false;
		Close();
	}
}

void QTFPopUp::PopUp(Ctrl *parent) {
	Close();
	Rect r = Rect(0, 0, width, maxheight);
	GetFrame().FrameLayout(r);
	int cy = min(maxheight, GetHeight(r.Width()) + maxheight - r.Height());
	Rect area = GetMouseWorkArea();
	Point p = GetMousePos();
	r.top = max(area.top, p.y + 16);
	r.bottom = r.top + cy;
	if(r.bottom > area.bottom) {
		r.bottom = area.bottom;
		r.top = r.bottom - cy;
	}
	r.left = max(area.left, p.x - width / 2);
	r.right = r.left + width;
	if(r.right > area.right) {
		r.right = area.right;
		r.left = r.right - width;
	}
	open = false;
	SetRect(r);
	Ctrl::PopUp(parent);
	SetFocus();
	open = true;
}

QTFPopUp::QTFPopUp() {
	static ColorF helpborder[] = {
		(ColorF)2,
		&SColorText, &SColorText, &SColorText, &SColorText,
		&SColorFace, &SColorFace, &SColorFace, &SColorFace,
	};
	static BorderFrame helpframe(helpborder);
	width = 250;
	maxheight = 150;
	Margins(5).Background(LtYellow).SetFrame(helpframe);
	open = false;
}

void PerformDescription()
{
	Ctrl *top = Ctrl::GetActiveWindow();
	if(!top) return;
	Vector<Image> vi;
	vi.Add(CtrlImg::HelpCursor0());
	vi.Add(CtrlImg::HelpCursor1());
	if(PointLoop(*top, vi, 300)) {
		Point p = GetMousePos();
		Ctrl *ctrl = Ctrl::GetVisibleChild(top, p - top->GetRect().TopLeft(), true);
		while(ctrl) {
			String desc = ctrl->GetDescription();
			if(!IsEmpty(desc)) {
				static QTFPopUp qtf;
				qtf.SetQTF(desc);
				qtf.PopUp(top);
				return;
			}
			ctrl = ctrl->GetParent();
		}
	}
}

}
