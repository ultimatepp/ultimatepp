//#include <CtrlCore/CtrlCore.h>
#include "TopFrame.h"

#ifdef GUI_WINGL

NAMESPACE_UPP

#define LLOG(x) LOG(x)

static Point glmousepos;

Point GetMousePos() {
	return glmousepos;
}

void Ctrl::MouseEventGl(Ptr<Ctrl> t, int event, Point p, int zdelta)
{
	Rect rr = t->GetRect();
	if((event & Ctrl::ACTION) == DOWN && !dynamic_cast<TopWindowFrame *>(~t)) {
		t->SetFocusWnd();
		if(t) t->SetForeground();
	}
	if(t)
		t->DispatchMouse(event, p - rr.TopLeft(), zdelta);
	if(t)
		t->PostInput();
}

void Ctrl::DoMouseGl(int event, Point p, int zdelta)
{
	glmousepos = p;
	int a = event & Ctrl::ACTION;
	if(a == Ctrl::UP && Ctrl::ignoreclick) {
		EndIgnore();
		return;
	}
	else
	if(a == Ctrl::DOWN && ignoreclick)
		return;
	LLOG("Mouse event: " << event << " position " << p << " zdelta " << zdelta << ", capture " << Upp::Name(captureCtrl));
	if(captureCtrl)
		MouseEventGl(captureCtrl->GetTopCtrl(), event, p, zdelta);
	else
		for(int i = topctrl.GetCount() - 1; i >= 0; i--) {
			Ptr<Ctrl> t = topctrl[i];
			Rect rr = t->GetRect();
			if(rr.Contains(p)) {
				MouseEventGl(t, event, p, zdelta);
				return;
			}
		}
	Ctrl *desktop = GetDesktop();
	if(desktop) {
		desktop->DispatchMouse(event, p, zdelta);
		desktop->PostInput();
	}
}

bool Ctrl::DoKeyGl(dword key, int cnt)
{
	bool b = DispatchKey(key, cnt);
	SyncCaret();
	Ctrl *desktop = GetDesktop();
	if(desktop)
		desktop->PostInput();
	return b;
}

Point glCursorPos = Null;
Image glCursorImage;
Rect  glCaretRect;
int   glCaretTm;

void Ctrl::SetCaret(int x, int y, int cx, int cy)
{
	GuiLock __;
	caretx = x;
	carety = y;
	caretcx = cx;
	caretcy = cy;
	glCaretTm = GetTickCount();
	SyncCaret();
}

void Ctrl::SyncCaret() {
	GuiLock __;
}

void Ctrl::CursorSync(Draw& w)
{
	Rect cr = Null;
	if(focusCtrl && (((GetTickCount() - glCaretTm) / 500) & 1) == 0)
		cr = (RectC(focusCtrl->caretx, focusCtrl->carety, focusCtrl->caretcx, focusCtrl->caretcy)
		      + focusCtrl->GetScreenView().TopLeft()) & focusCtrl->GetScreenView();

	glCaretRect = cr;
	if(!cr.IsEmpty())
		w.DrawRect(cr, Black);
	glCursorPos = GetMousePos() - glCursorImage.GetHotSpot();
	Size sz = glCursorImage.GetSize();
	w.DrawImage(glCursorPos.x, glCursorPos.y, sz.cx, sz.cy, glCursorImage);
}

void  Ctrl::SetMouseCursor(const Image& image)
{
	GuiLock __;
	if(image.GetSerialId() != glCursorImage.GetSerialId()) {
		glCursorImage = image;
		glCursorPos = Null;
	}
}

END_UPP_NAMESPACE

#endif