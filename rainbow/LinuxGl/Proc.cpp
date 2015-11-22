#ifdef flagLINUXGL

#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

#define LLOG(x)       //LOG(x)

static dword sKbdState;
static dword sModState;

bool GetShift() { GuiLock __; return sKbdState & ShiftMask; }
bool GetCtrl() { GuiLock __; return sKbdState & ControlMask; }
bool GetAlt() { GuiLock __; return sKbdState & Mod1Mask; }
bool GetCapsLock() { GuiLock __; return sKbdState & LockMask; }
bool GetMouseLeft() { GuiLock __; return sModState & Button1Mask; }
bool GetMouseRight() { GuiLock __; return sModState & (Ctrl::Xbuttons >= 3 ? Button3Mask : Button2Mask); }
bool GetMouseMiddle() { GuiLock __; return sModState & (Ctrl::Xbuttons >= 3 ? Button2Mask : 0); }

dword KEYtoK(dword key)
{
	if(key != K_CTRL_KEY && key != K_SHIFT_KEY && key != K_ALT_KEY) {
		if(GetCtrl()) key |= K_CTRL;
		if(GetAlt()) key |= K_ALT;
		if(GetShift()) key |= K_SHIFT;
	}
	return key;
}

void glWindowProc(XWindow win, XEvent* event)
{
	GuiLock __;
	//Ptr<Ctrl> _this = this;
	bool pressed = false;
	int  count = 1;

	switch(event->type) {
	  	case Expose: {
	  		XWindowAttributes gwa;
			XGetWindowAttributes(dpy, win, &gwa);
			glViewport(0, 0, gwa.width, gwa.height);
			Size sz(gwa.width, gwa.height);
			screenFbo0.Resize(sz.cx, sz.cy);
			screenFbo1.Resize(sz.cx, sz.cy);
			Ctrl::SetWindowSize(sz);
	  	}
	  	break;
		case KeyPress:
			pressed = true;
			LLOG("event type:" << event->type << " state:" << event->xkey.state <<
			     "keycode:" << event->xkey.keycode);
			for(;;) {
				XEvent ev1[1], ev2[1];
				bool hasev2 = false;
				if(!GlIsWaitingEvent()) break;
				do
					XNextEvent(dpy, ev1);
				while(ev1->type == NoExpose && GlIsWaitingEvent());
				LLOG("ev1 type:" << ev1->type << " state:" << ev1->xkey.state <<
				     "keycode:" << ev1->xkey.keycode);
				if(ev1->type == KeyPress)
					*ev2 = *ev1;
				else {
					if(ev1->type != KeyRelease ||
					   ev1->xkey.state != event->xkey.state ||
					   ev1->xkey.keycode != event->xkey.keycode ||
					   !GlIsWaitingEvent()) {
					   	XPutBackEvent(dpy, ev1);
					   	break;
					}
					do
						XNextEvent(dpy, ev2);
					while(ev2->type == NoExpose && GlIsWaitingEvent());
					LLOG("ev2 type:" << ev2->type << " state:" << ev2->xkey.state <<
					     "keycode:" << ev2->xkey.keycode);
					hasev2 = true;
				}
				if(ev2->type != KeyPress ||
				   ev2->xkey.state != event->xkey.state ||
				   ev2->xkey.keycode != event->xkey.keycode) {
					if(hasev2)
						XPutBackEvent(dpy, ev2);
					XPutBackEvent(dpy, ev1);
					break;
				}
				else {
					XFilterEvent(ev1, None);
					if(hasev2)
						XFilterEvent(ev2, None);
				}
				count++;
			}
		break;
		case KeyRelease: {
	//			mousepos = Point(event->xkey.x_root, event->xkey.y_root);
				char buff[128];
				Xeventtime = event->xkey.time;
				LLOG("Key Xeventtime: " << Xeventtime << " count:" << count);
				KeySym keysym;
				int    chr = 0;
				WString wtext;
				if(pressed) {
//					Status status;
//					int len = Xutf8LookupString(NULL, &event->xkey, buff, sizeof(buff), &keysym, &status);
					int len = 0;
					buff[len] = 0;
					/*if(status == XLookupChars || status == XLookupBoth) {
						chr = FromUtf8(buff, len)[0];
						if(status == XLookupChars)
							wtext = FromUtf8(buff, len);
					}
					else
					if(status != XLookupKeySym && status != XLookupBoth)
					    keysym = 0;*/
				}
				else {
					int len = XLookupString(&event->xkey, buff, sizeof(buff), &keysym, NULL);
					buff[len] = 0;
					chr = FromUtf8(buff, len)[0];
					if(len > 1)
						wtext = FromUtf8(buff, len);
				}
				if(keysym == XK_Control_L || keysym == XK_Control_R) {
					keysym = XK_Control_L;
					if(pressed)
						sKbdState |= ControlMask;
					else
						sKbdState &= ~ControlMask;
				}
				if(keysym == XK_Shift_L || keysym == XK_Shift_R) {
					keysym = XK_Shift_L;
					if(pressed)
						sKbdState |= ShiftMask;
					else
						sKbdState &= ~ShiftMask;
				}
				if(keysym == XK_Meta_L || keysym == XK_Meta_R || keysym == XK_Alt_L ||
				   keysym == XK_Alt_R || keysym == XK_Super_L || keysym == XK_Super_R ||
				   keysym == XK_Hyper_L || keysym == XK_Hyper_R || keysym == XK_ISO_Prev_Group) {
					keysym = XK_Meta_L;
					if(pressed)
						sKbdState |= Mod1Mask;
					else
						sKbdState &= ~Mod1Mask;
				}
				LLOG("KeySym:" << FormatIntHex(keysym) << " " << (char)keysym << " " << count);
				dword up = pressed ? 0 : K_KEYUP;
				static struct { KeySym keysym; dword key; } tab[] = {
					{ XK_ISO_Left_Tab, K_TAB|K_SHIFT },
					{ XK_BackSpace, K_BACKSPACE },
					{ XK_Tab, K_TAB },
					{ XK_Return, K_ENTER },
					{ XK_KP_Enter, K_ENTER },
					{ XK_Escape, K_ESCAPE },
					{ XK_space, K_SPACE },
	
					{ XK_KP_Space, K_SPACE },
					{ XK_KP_Tab, K_TAB },
					{ XK_KP_Enter, K_ENTER },
					{ XK_KP_F1, K_F1 },
					{ XK_KP_F2, K_F2 },
					{ XK_KP_F3, K_F3 },
					{ XK_KP_F4, K_F4 },
					{ XK_KP_Home, K_HOME },
					{ XK_KP_Left, K_LEFT },
					{ XK_KP_Up, K_UP },
					{ XK_KP_Right, K_RIGHT },
					{ XK_KP_Down, K_DOWN },
					{ XK_KP_Page_Up, K_PAGEUP },
					{ XK_KP_Page_Down, K_PAGEDOWN },
					{ XK_KP_End, K_END },
					{ XK_KP_Begin, K_HOME },
					{ XK_KP_Insert, K_INSERT },
					{ XK_KP_Delete, K_DELETE },
				};
				for(int i = 0; i < __countof(tab); i++)
					if(tab[i].keysym == keysym) {
						Ctrl::DoKeyGl(KEYtoK(tab[i].key)|up, count);
						return;
					}
				if(GetShift() && chr == 0) {
					static dword k[] = { 41, 33, 64, 35, 36, 37, 94, 38, 42, 40 };
					for(int i = 0; i < 10; i++)
						if(keysym == k[i]) {
							Ctrl::DoKeyGl(KEYtoK(i + K_0)|up, count);
							return;
						}
				}
				if(keysym >= 48 && keysym <= 57 && chr == 0) {
					Ctrl::DoKeyGl(KEYtoK(keysym - 48 + K_0)|up, count);
					return;
				}
				if(chr >= 1 && chr < 32) {
					Ctrl::DoKeyGl(KEYtoK(chr - 1 + K_CTRL_A)|up, count);
					return;
				}
				if(keysym >= 0xff80 && keysym <= 0xffb9 && chr) {
					Ctrl::DoKeyGl(KEYtoK(chr)|up, count);
					return;
				}
				if(keysym >= 0xff00 && chr < 128 ||
				   (GetCtrl() || GetAlt()) && keysym >= 0x20 && keysym < 0x7f) {
					if(keysym >= 'a' && keysym <= 'z')
						keysym = keysym - 'a' + 'A';
					Ctrl::DoKeyGl(KEYtoK(keysym|K_DELTA)|up, count);
					return;
				}
	
				if((chr == 32 || chr == 9 || chr == 13) && !pressed)
					Ctrl::DoKeyGl(chr|K_KEYUP, count);
				if(chr && pressed) {
					Ctrl::DoKeyGl(chr, count);
					for(int ii = 1; ii < wtext.GetLength(); ii++)
						Ctrl::DoKeyGl(wtext[ii], count);
				}
			}
			break;
		case ButtonPress: {
				//if(!HasWndFocus() && !popup)
				//	SetWndFocus();
				//ClickActivateWnd();
				Point mousePos = Point(event->xbutton.x_root, event->xbutton.y_root);
				//ReleaseGrab();
				XButtonEvent& e = event->xbutton;
				sModState = e.state;
				Xeventtime = e.time;
				//if(Ctrl::ignoreclick) break;
				Point p = Point(e.x, e.y);
				dword action = Ctrl::DOWN;
				if((dword)e.time - (dword)Xbuttontime < 800) {
					action = Ctrl::DOUBLE;
					Xbuttontime = Xeventtime - 0x80000000;
				}
				else {
					Xbuttontime = e.time;
					Xbuttonpos = mousePos;
				}
				switch(e.button) {
				case Button1:
					sModState |= Button1Mask;
					Ctrl::DoMouseGl(Ctrl::LEFT|action, p, 0);
					break;
				case Button2:
					sModState |= Button2Mask;
					if(Xbuttons < 3)
						Ctrl::DoMouseGl(Ctrl::RIGHT|action, p, 0);
					else
						Ctrl::DoMouseGl(Ctrl::MIDDLE|action, p, 0);
					break;
				case Button3:
					sModState |= Button3Mask;
					Ctrl::DoMouseGl(Ctrl::RIGHT|action, p, 0);
					break;
				}
				//if(_this) PostInput();				
			}
			break;
		case ButtonRelease: {
				Point mousePos = Point(event->xbutton.x_root, event->xbutton.y_root);
				XButtonEvent& e = event->xbutton;
				sModState = e.state;
				Xeventtime = e.time;
				Point p = Point(e.x, e.y);
				switch(e.button) {
				case Button1:
					sModState &= ~Button1Mask;
					break;
				case Button2:
					sModState &= ~Button2Mask;
					break;
				case Button3:
					sModState &= ~Button3Mask;
					break;
				}
//				if(ignoreclick)
//					EndIgnore();
//				else
					switch(e.button) {
					case Button1:
						Ctrl::DoMouseGl(Ctrl::LEFTUP, p, 0);
						break;
					case Button2:
						if(Xbuttons < 3)
							Ctrl::DoMouseGl(Ctrl::RIGHTUP, p, 0);
						else
							Ctrl::DoMouseGl(Ctrl::MIDDLEUP, p, 0);
						break;
					case Button3:
						Ctrl::DoMouseGl(Ctrl::RIGHTUP, p, 0);
						break;
					case Button4:
						Ctrl::DoMouseGl(Ctrl::MOUSEWHEEL, p, 120);
						break;
					case Button5:
						Ctrl::DoMouseGl(Ctrl::MOUSEWHEEL, p, -120);
						break;
					}
				//if(_this) PostInput();
				
			}
			break;
		case MotionNotify:
			while(XCheckWindowEvent(dpy, win, PointerMotionMask, event));
	//		Ctrl::EndIgnore();
			Point mousePos = Point(event->xmotion.x_root, event->xmotion.y_root);
			Xeventtime = event->xmotion.time;
			Point p = mousePos - Xbuttonpos;
			if(max(abs(p.x), abs(p.y)) > 4)
				Xbuttontime = Xeventtime - 0x80000000;
			sModState = event->xmotion.state;
			Ctrl::DoMouseGl(Ctrl::MOUSEMOVE, Point(event->xmotion.x, event->xmotion.y), 0);
	//		DoCursorShape();
			break;
	}
  	
/*
	case WM_SIZE:
	{
		//ActivateGlContext();
		Ctrl::SetWindowSize(Size(LOWORD(lParam), HIWORD(lParam)));
		Size sz = Ctrl::GetScreenSize();
		screenFbo0.Resize(sz.cx, sz.cy);
		screenFbo1.Resize(sz.cx, sz.cy);
		return 0L;
	}
	case WM_CLOSE:
		Ctrl::EndSession();
		return 0L;
	}*/
	//DropEvent(win, event);
}

END_UPP_NAMESPACE

#endif
