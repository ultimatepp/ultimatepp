#if defined(flagWINGL) || defined(flagLINUXGL)

#include "TopFrame.h"

NAMESPACE_UPP

#define LLOG(x) //LOG(x)

static Point glmousepos;

Point GetMousePos() {
	return glmousepos;
}

void Ctrl::SysEndLoop()
{
}

void WakeUpGuiThread()
{
}

void Ctrl::MouseEventGl(Ptr<Ctrl> t, int event, Point p, int zdelta)
{
	if(!t->IsEnabled() && (t != (Ctrl*) &infoPanel && t != (Ctrl*) &console))
		return;
	Rect rr = t->GetRect();
	if((event & Ctrl::ACTION) == DOWN) {
		Ptr<Ctrl> q = t;
		TopWindowFrame *wf = dynamic_cast<TopWindowFrame *>(~t);
		if(wf)
			q = wf->window;			
		if(q) q->ClickActivateWnd();
		if(q) q->SetForeground();
		if(ignoreclick)
			return;
	}
	if(t)
		t->DispatchMouse(event, p - rr.TopLeft(), zdelta);
	if(t)
		t->PostInput();
}

Ctrl *Ctrl::FindMouseTopCtrl()
{
	for(int i = topctrl.GetCount() - 1; i >= 0; i--) {
		Ctrl *t = topctrl[i];
		if(t->GetRect().Contains(glmousepos))
			return t->IsEnabled() ? t : NULL;
	}
	return desktop->IsEnabled() ? desktop : NULL;
}

bool Ctrl::DoMouseGl(Ctrl* q, int event, Point p, int zdelta)
{
	Rect rr = q->GetRect();
	if(rr.Contains(p)) {
		MouseEventGl(q, event, p, zdelta);
		return true;
	}
	return false;
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
	LLOG("### Mouse event: " << event << " position " << p << " zdelta " << zdelta << ", capture " << Upp::Name(captureCtrl));
	if(captureCtrl)
		MouseEventGl(captureCtrl->GetTopCtrl(), event, p, zdelta);
	else
	{
		bool processed = consoleActive && DoMouseGl((Ctrl*) &console, event, p, zdelta);
		
		if(!processed && controlPanelActive)
			processed = DoMouseGl((Ctrl*) &infoPanel, event, p, zdelta);
		
		if(!processed)
		{
			for(int i = topctrl.GetCount() - 1; i >= 0; i--) {
				Ptr<Ctrl> t = topctrl[i];
				if(t != (Ctrl*) &infoPanel && t != (Ctrl*) &console && t->GetRect().Contains(p) &&
				   DoMouseGl(t, event, p, zdelta))
				   return;
			}
			Ctrl *desktop = GetDesktop();
			if(desktop) {
				desktop->DispatchMouse(event, p, zdelta);
				desktop->PostInput();
			}
		}
	}
}

bool Ctrl::DoKeyGl(dword key, int cnt)
{
	if(key == K_CTRL_GRAVE)
	{
		controlPanelActive = !controlPanelActive;
		consoleActive = !consoleActive;
		return true;
	}
	else if(key == K_CTRL_F12)
	{
		consoleActive = !consoleActive;
		return true;
	}
	else if(key == K_CTRL_F11)
	{
		controlPanelActive = !controlPanelActive;
		return true;
	}
	
	bool b = DispatchKey(key, cnt);
	SyncCaret();
	Ctrl *desktop = GetDesktop();
	if(desktop)
		desktop->PostInput();
	return b;
}

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
}

void Ctrl::MouseSync(Draw& w)
{
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