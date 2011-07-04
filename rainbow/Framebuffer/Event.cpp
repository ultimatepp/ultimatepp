#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

#define LLOG(x) LOG(x)

static Point fbmousepos;

Point GetMousePos() {
	return fbmousepos;
}

void Ctrl::DoMouseFB(int event, Point p, int zdelta)
{
	fbmousepos = p;
	int a = event & Ctrl::ACTION;
	if(a == Ctrl::UP && Ctrl::ignoreclick) {
		EndIgnore();
		return;
	}
	else
	if(a == Ctrl::DOWN && ignoreclick)
		return;
	LLOG("Mouse event: " << event << " position " << p << " zdelta " << zdelta);
	for(int i = topctrl.GetCount() - 1; i >= 0; i--) {
		Ptr<Ctrl> t = topctrl[i];
		Rect rr = t->GetRect();
		if(rr.Contains(p)) {
			if(t && a == DOWN && !dynamic_cast<TopWindow::Frame *>(~t)) {
				t->SetFocusWnd();
				t->SetForeground();
			}
			t->DispatchMouse(event, p - rr.TopLeft(), zdelta);
			if(t)
				t->PostInput();
			return;
		}
	}
	Ctrl *desktop = GetDesktop();
	if(desktop) {
		desktop->DispatchMouse(event, p, zdelta);
		desktop->PostInput();
	}
}

bool Ctrl::DoKeyFB(dword key, int cnt)
{
	bool b = DispatchKey(key, cnt);
	SyncCaret();
	Ctrl *desktop = GetDesktop();
	if(desktop)
		desktop->PostInput();
	return b;
}

Image Ctrl::GetBak(Rect& tr)
{
	Image bak;
	tr.Intersect(framebuffer.GetSize());
	if(!tr.IsEmpty()) {
		Image h = framebuffer;
		bak = CreateImage(tr.GetSize(), Black);
		Copy(bak, Point(0, 0), h, tr);
		framebuffer = h;
	}
	return bak;
}

void Ctrl::RemoveCursor()
{
	if(!IsNull(fbCursorBakPos)) {
		Copy(framebuffer, fbCursorBakPos, fbCursorBak, fbCursorBak.GetSize());
		AddInvalid(Rect(fbCursorBakPos, fbCursorBak.GetSize()));
	}
	fbCursorPos = fbCursorBakPos = Null;
	fbCursorBak = Null;
}

void Ctrl::RemoveCaret()
{
	if(!IsNull(fbCaretRect)) {
		Copy(framebuffer, fbCaretRect.TopLeft(), fbCaretBak, fbCaretBak.GetSize());
		AddInvalid(fbCaretRect);
	}
	fbCaretRect = Null;
	fbCaretBak = Null;
}

void Ctrl::SetCaret(int x, int y, int cx, int cy)
{
	GuiLock __;
	caretx = x;
	carety = y;
	caretcx = cx;
	caretcy = cy;
	fbCaretTm = GetTickCount();
	SyncCaret();
}

void Ctrl::SyncCaret() {
	GuiLock __;
}

void Ctrl::CursorSync()
{
	Point p = GetMousePos() - fbCursorImage.GetHotSpot();
	Rect cr = Null;
	if(focusCtrl && (((GetTickCount() - fbCaretTm) / 500) & 1) == 0)
		cr = RectC(focusCtrl->caretx, focusCtrl->carety, focusCtrl->caretcx, focusCtrl->caretcy)
		     + focusCtrl->GetScreenView().TopLeft();
	if(fbCursorPos != p || cr != fbCaretRect) {
		RemoveCursor();
		RemoveCaret();

		fbCursorPos = p;
		Size sz = fbCursorImage.GetSize();
		Rect tr(p, sz);
		fbCursorBak = GetBak(tr);
		fbCursorBakPos = tr.TopLeft();

		if(!cr.IsEmpty()) {
			fbCaretBak = GetBak(cr);
			fbCaretRect = cr;
			for(int y = cr.top; y < cr.bottom; y++) {
				RGBA *s = framebuffer[y] + cr.left;
				const RGBA *e = framebuffer[y] + cr.right;
				while(s < e) {
					s->r = ~s->r;
					s->g = ~s->g;
					s->b = ~s->b;
					s++;
				}
			}
			AddInvalid(fbCaretRect);
		}

		Over(framebuffer, p, fbCursorImage, sz);
		AddInvalid(tr);
	}
}

void  Ctrl::SetMouseCursor(const Image& image)
{
	GuiLock __;
	if(image.GetSerialId() != fbCursorImage.GetSerialId()) {
		fbCursorImage = image;
		fbCursorPos = Null;
	}
}

END_UPP_NAMESPACE
