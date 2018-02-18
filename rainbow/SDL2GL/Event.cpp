#include "Local.h"

#ifdef GUI_SDL20GL

NAMESPACE_UPP

#define LLOG(x)  LOG(x)
#define LDUMP(x) //DDUMP(x)

static Point fbmousepos;

int SDLwidth;
int SDLheight;

Point GetMousePos() {
	return fbmousepos;
}

dword mouseb = 0;
dword modkeys = 0;

enum KM {
	KM_NONE  = 0x00,

	KM_LSHIFT= 0x01,
	KM_RSHIFT= 0x02,
	KM_LCTRL = 0x04,
	KM_RCTRL = 0x08,
	KM_LALT  = 0x10,
	KM_RALT  = 0x20,

	KM_CAPS  = 0x40,
	KM_NUM   = 0x80,
	
	KM_CTRL = KM_LCTRL | KM_RCTRL,
	KM_SHIFT = KM_LSHIFT | KM_RSHIFT,
	KM_ALT = KM_LALT | KM_RALT,
};

bool GetMouseLeft()   { return mouseb & (1<<0); }
bool GetMouseRight()  { return mouseb & (1<<1); }
bool GetMouseMiddle() { return mouseb & (1<<2); }
bool GetShift()       { return modkeys & KM_SHIFT; }
bool GetCtrl()        { return modkeys & KM_CTRL; }
bool GetAlt()         { return modkeys & KM_ALT; }
bool GetCapsLock()    { return modkeys & KM_CAPS; }

dword fbKEYtoK(dword chr) {
	if(chr == SDLK_TAB)
		chr = K_TAB;
	else
	if(chr == SDLK_SPACE)
		chr = K_SPACE;
	else
	if(chr == SDLK_RETURN)
		chr = K_RETURN;
	else
		chr = chr + K_DELTA;
	if(chr == K_ALT_KEY || chr == K_CTRL_KEY || chr == K_SHIFT_KEY)
		return chr;
	if(GetCtrl()) chr |= K_CTRL;
	if(GetAlt()) chr |= K_ALT;
	if(GetShift()) chr |= K_SHIFT;
	return chr;
}

dword lastbdowntime[8] = {0};
dword isdblclick[8] = {0};

void Ctrl::HandleSDLEvent(SDL_Event* event)
{
	LLOG("HandleSDLEvent " << event->type);
	SDL_Event next_event;
	dword keycode;
	switch(event->type) {
//		case SDL_ACTIVEEVENT: //SDL_ActiveEvent
//			break;
	case SDL_TEXTINPUT: {
			//send respective keyup things as char events as well
		WString text = FromUtf8(event->text.text);
		for(int i = 0; i < text.GetCount(); i++) {
			int c = text[i];
			if(c != 127)
				Ctrl::DoKeyFB(c, 1);
		}
		break;
	}
	case SDL_KEYDOWN:
		switch(event->key.keysym.sym) {
			case SDLK_LSHIFT: modkeys |= KM_LSHIFT; break;
			case SDLK_RSHIFT: modkeys |= KM_RSHIFT; break;	
			case SDLK_LCTRL: modkeys |= KM_LCTRL; break;
			case SDLK_RCTRL: modkeys |= KM_RCTRL; break;	
			case SDLK_LALT: modkeys |= KM_LALT; break;
			case SDLK_RALT: modkeys |= KM_RALT; break;
		}
		
		keycode = fbKEYtoK((dword)event->key.keysym.sym);
		
		if(keycode != K_SPACE) { //dont send space on keydown
			static int repeat_count;
			SDL_PumpEvents();
			if(SDL_PeepEvents(&next_event, 1, SDL_PEEKEVENT, SDL_KEYDOWN, SDL_KEYDOWN) &&
			   next_event.key.keysym.sym == event->key.keysym.sym) {
				repeat_count++; // Keyboard repeat compression
				break;
			}
			Ctrl::DoKeyFB(keycode, 1 + repeat_count);
			repeat_count = 0;
		}
		break;
	case SDL_KEYUP: //SDL_KeyboardEvent
		switch(event->key.keysym.sym) {
			case SDLK_LSHIFT: modkeys &= ~KM_LSHIFT; break;
			case SDLK_RSHIFT: modkeys &= ~KM_RSHIFT; break;	
			case SDLK_LCTRL: modkeys &= ~KM_LCTRL; break;
			case SDLK_RCTRL: modkeys &= ~KM_RCTRL; break;	
			case SDLK_LALT: modkeys &= ~KM_LALT; break;
			case SDLK_RALT: modkeys &= ~KM_RALT; break;	
		}

		Ctrl::DoKeyFB(fbKEYtoK((dword)event->key.keysym.sym) | K_KEYUP, 1);
		break;
	case SDL_MOUSEMOTION:
		SDL_PumpEvents();
		if(SDL_PeepEvents(&next_event, 1, SDL_PEEKEVENT, SDL_MOUSEMOTION, SDL_MOUSEMOTION) > 0)
			break; // MouseMove compression
		Ctrl::DoMouseFB(Ctrl::MOUSEMOVE, Point(event->motion.x, event->motion.y));
		break;
	case SDL_MOUSEWHEEL:
		Ctrl::DoMouseFB(Ctrl::MOUSEWHEEL, GetMousePos(), sgn(event->wheel.y) * 120);
		break;
	case SDL_MOUSEBUTTONDOWN: {
			Point p(event->button.x, event->button.y);
			int bi = event->button.button;
			dword ct = SDL_GetTicks();
			if(isdblclick[bi] && (abs(int(ct) - int(lastbdowntime[bi])) < 400))
			{
				switch(bi)
				{
					case SDL_BUTTON_LEFT: Ctrl::DoMouseFB(Ctrl::LEFTDOUBLE, p); break;
					case SDL_BUTTON_RIGHT: Ctrl::DoMouseFB(Ctrl::RIGHTDOUBLE, p); break;
					case SDL_BUTTON_MIDDLE: Ctrl::DoMouseFB(Ctrl::MIDDLEDOUBLE, p); break;
				}
				isdblclick[bi] = 0; //reset, to go ahead sending repeats
			}
			else
			{
				lastbdowntime[bi] = ct;
				isdblclick[bi] = 0; //prepare for repeat
				switch(bi)
				{
					case SDL_BUTTON_LEFT: mouseb |= (1<<0); Ctrl::DoMouseFB(Ctrl::LEFTDOWN, p); break;
					case SDL_BUTTON_RIGHT: mouseb |= (1<<1); Ctrl::DoMouseFB(Ctrl::RIGHTDOWN, p); break;
					case SDL_BUTTON_MIDDLE: mouseb |= (1<<2); Ctrl::DoMouseFB(Ctrl::MIDDLEDOWN, p); break;
				}
			}
		}
		break;
	case SDL_MOUSEBUTTONUP: {
			int bi = event->button.button;
			isdblclick[bi] = 1; //indicate maybe a dblclick
	
			Point p(event->button.x, event->button.y);
			switch(bi)
			{
				case SDL_BUTTON_LEFT: mouseb &= ~(1<<0); Ctrl::DoMouseFB(Ctrl::LEFTUP, p); break;
				case SDL_BUTTON_RIGHT: mouseb &= ~(1<<1); Ctrl::DoMouseFB(Ctrl::RIGHTUP, p); break;
				case SDL_BUTTON_MIDDLE: mouseb &= ~(1<<2); Ctrl::DoMouseFB(Ctrl::MIDDLEUP, p); break;
			}
		}
		break;
/*		case SDL_VIDEORESIZE: //SDL_ResizeEvent
		{
			width = event->resize.w;
			height = event->resize.h;
	
			SDL_FreeSurface(screen);
			screen = CreateScreen(width, height, bpp, videoflags);
			ASSERT(screen);
			Ctrl::SetFramebufferSize(Size(width, height));
		}
			break;
		case SDL_VIDEOEXPOSE: //SDL_ExposeEvent
			break;*/
	case SDL_WINDOWEVENT:
        switch (event->window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            break;
        case SDL_WINDOWEVENT_MOVED:
            break;
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			SDLwidth = event->window.data1;
			SDLheight = event->window.data2;
        	break;
        case SDL_WINDOWEVENT_RESIZED:
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            break;
        case SDL_WINDOWEVENT_RESTORED:
            break;
        case SDL_WINDOWEVENT_ENTER:
        	sdlMouseIsIn = true;
        	Invalidate();
            break;
        case SDL_WINDOWEVENT_LEAVE:
        	sdlMouseIsIn = false;
        	Invalidate();
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            break;
        case SDL_WINDOWEVENT_CLOSE:
            break;
        }
		break;
	case SDL_QUIT: //SDL_QuitEvent
		Ctrl::EndSession();
		break;
	}
}

void Ctrl::MouseEventFB(Ptr<Ctrl> t, int event, Point p, int zdelta)
{
	if(!t->IsEnabled())
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
		if(t->GetRect().Contains(fbmousepos))
			return t->IsEnabled() ? t : NULL;
	}
	return desktop->IsEnabled() ? desktop : NULL;
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
	LLOG("### Mouse event: " << event << " position " << p << " zdelta " << zdelta << ", capture " << Upp::Name(captureCtrl));
	if(captureCtrl)
		MouseEventFB(captureCtrl->GetTopCtrl(), event, p, zdelta);
	else
		for(int i = topctrl.GetCount() - 1; i >= 0; i--) {
			Ptr<Ctrl> t = topctrl[i];
			Rect rr = t->GetRect();
			if(rr.Contains(p)) {
				MouseEventFB(t, event, p, zdelta);
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
	LLOG("DoKeyFB " << GetKeyDesc(key) << ", " << cnt);

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
	fbCaretTm = GetTickCount();
	SyncCaret();
}

void Ctrl::SyncCaret()
{
	CursorSync();
}

void Ctrl::CursorSync()
{
	LLOG("@ CursorSync");
	Point p = GetMousePos() - fbCursorImage.GetHotSpot();
	Rect cr = Null;
	if(focusCtrl && (((GetTickCount() - fbCaretTm) / 500) & 1) == 0)
		cr = (RectC(focusCtrl->caretx, focusCtrl->carety, focusCtrl->caretcx, focusCtrl->caretcy)
		      + focusCtrl->GetScreenView().TopLeft()) & focusCtrl->GetScreenView();
	if(fbCursorPos != p && !SystemCursor || cr != fbCaretRect) {
		fbCaretRect = cr;
		fbCursorPos = p;
		Invalidate();
	}
}

bool Ctrl::ProcessEvent(bool *quit)
{
	LLOG("@ ProcessEvent");
	ASSERT(IsMainThread());
	if(!GetMouseLeft() && !GetMouseRight() && !GetMouseMiddle())
		ReleaseCtrlCapture();
	bool ret = false;
	SDL_Event event;
	if(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT && quit)
			*quit = true;
		HandleSDLEvent(&event);
		ret = true;
	}
	DefferedFocusSync();
	SyncCaret();
	SyncTopWindows();
	return ret;
}

bool Ctrl::ProcessEvents(bool *quit)
{
	//LOGBLOCK("@ ProcessEvents");
//	MemoryCheckDebug();
	bool ret = ProcessEvent(quit);
	while(ProcessEvent(quit) && (!LoopCtrl || LoopCtrl->InLoop()));
	TimeStop tm;
	LLOG("TimerProc invoked at " << msecs());
	TimerProc(GetTickCount());
	LLOG("TimerProc elapsed: " << tm);
	SweepMkImageCache();
	DoPaint();
	return ret;
}

void Ctrl::EventLoop(Ctrl *ctrl)
{
	GuiLock __;
	ASSERT(IsMainThread());
	ASSERT(LoopLevel == 0 || ctrl);
	LoopLevel++;
	LLOG("Entering event loop at level " << LoopLevel << LOG_BEGIN);
	Ptr<Ctrl> ploop;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	bool quit = false;
	int64 loopno = ++EventLoopNo;
	ProcessEvents(&quit);
	while(loopno > EndSessionLoopNo && !quit && (ctrl ? ctrl->IsOpen() && ctrl->InLoop() : GetTopCtrls().GetCount()))
	{
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / GuiSleep");
		SyncCaret();
		GuiSleep(20);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / ProcessEvents");
		ProcessEvents(&quit);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / after ProcessEvents");
		LDUMP(loopno);
		LDUMP(fbEndSessionLoop);
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG(LOG_END << "Leaving event loop ");
}

void Ctrl::GuiSleep(int ms)
{
	GuiLock __;
	ASSERT(IsMainThread());
	LLOG("GuiSleep");
	int level = LeaveGuiMutexAll();
	SDL_WaitEventTimeout(NULL, ms);
	EnterGuiMutex(level);
}

END_UPP_NAMESPACE

#endif
