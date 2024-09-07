#include "CtrlCore.h"

#ifdef GUI_X11

namespace Upp {

#define LLOG(x)        //DLOG(x)

#define LTIMING(x)     //TIMING(x)

static dword sKbdState;
static dword sModState;

void ClearKbdState_()
{
	GuiLock __;
	sKbdState = 0;
}

Point GetMousePos()
{
	GuiLock __;
	LTIMING("GetMousePos");
	return Ctrl::mousePos;
}

void Ctrl::SyncMousePos()
{
	GuiLock __;
	LTIMING("XQueryPointer");
	int x, y, xx, yy;
	Window dm1, dm2;
	Ctrl::mousePos = Null;
	if(XQueryPointer(Xdisplay, Xroot, &dm1, &dm2, &x, &y, &xx, &yy, &sKbdState))
		Ctrl::mousePos = Point(x, y);
}

bool GetShift() { GuiLock __; return sKbdState & ShiftMask; }
bool GetCtrl() { GuiLock __; return sKbdState & ControlMask; }
bool GetAlt() { GuiLock __; return sKbdState & Mod1Mask; }
bool GetCapsLock() { GuiLock __; return sKbdState & LockMask; }
bool GetMouseLeft() { GuiLock __; return sModState & Button1Mask; }
bool GetMouseRight() { GuiLock __; return sModState & (Ctrl::Xbuttons >= 3 ? Button3Mask : Button2Mask); }
bool GetMouseMiddle() { GuiLock __; return sModState & (Ctrl::Xbuttons >= 3 ? Button2Mask : 0); }

int   Ctrl::Xbuttontime;
Point Ctrl::Xbuttonpos;

dword Ctrl::KEYtoK(dword key)
{
	if(key != K_CTRL_KEY && key != K_SHIFT_KEY && key != K_ALT_KEY) {
		if(GetCtrl()) key |= K_CTRL;
		if(GetAlt()) key |= K_ALT;
		if(GetShift()) key |= K_SHIFT;
	}
	return key;
}

void Ctrl::SetLastActive(XWindow *w, Ctrl *la)
{
	GuiLock __; 
	while(w) {
		LLOG("  to " << UPP::Name(w->ctrl));
		w->last_active = la;
		w = w->owner ? w->owner->GetXWindow() : NULL;
	}
}

void Ctrl::EventProc(XWindow& w, XEvent *event)
{
	GuiLock __; 
	eventid++;
	Ptr<Ctrl> _this = this;
	bool pressed = false;
	int  count = 1;
	switch(event->type) {
	case NoExpose:
		LLOG("NoExpose serial " << event->xnoexpose.serial);
		break;
	case GraphicsExpose:
		LLOG("GraphicsExpose serial " << event->xgraphicsexpose.serial);
	case Expose: {
			XExposeEvent& e = event->xexpose;
			w.exposed = true;
			LLOG("Expose " << RectC(e.x, e.y, e.width, e.height));
			Invalidate(w, RectC(e.x, e.y, e.width, e.height));
		}
		return;
	case ConfigureNotify: {
			XConfigureEvent& e = event->xconfigure;
			int x, y;
			Window dummy;
// 01/12/2007 - mdelfede
// added support for windowed controls
//			if(top)
//				XTranslateCoordinates(Xdisplay, top->window, Xroot, 0, 0, &x, &y, &dummy);
			Window xwin = GetWindow();
			if(xwin) {
				Window DestW = (GetParent() ? GetParentWindow() : Xroot);
				XTranslateCoordinates(Xdisplay, xwin, DestW, 0, 0, &x, &y, &dummy);
				Rect rect = RectC(x, y, e.width, e.height);
				LLOG("CongigureNotify " << rect);
				if(GetRect() != rect)
					SetWndRect(rect);
			}
			// Synchronizes native windows (NOT the main one)
			SyncNativeWindows();
// 01/12/2007 - END

		}
		return;
	default:
		if(!IsEnabled()) return;
	}
	LTIMING("XUserInput");
	switch(event->type) {
	case FocusIn:
		if(w.xic)
			XSetICFocus(w.xic);
		break;
	case FocusOut:
		if(w.xic)
			XUnsetICFocus(w.xic);
		break;
	case KeyPress:
		pressed = true;
		LLOG("event type:" << event->type << " state:" << event->xkey.state <<
		     "keycode:" << event->xkey.keycode);
		for(;;) {
			XEvent ev1[1], ev2[1];
			bool hasev2 = false;
			if(!IsWaitingEvent()) break;
			do
				XNextEvent(Xdisplay, ev1);
			while(ev1->type == NoExpose && IsWaitingEvent());
			LLOG("ev1 type:" << ev1->type << " state:" << ev1->xkey.state <<
			     "keycode:" << ev1->xkey.keycode);
			if(ev1->type == KeyPress)
				*ev2 = *ev1;
			else {
				if(ev1->type != KeyRelease ||
				   ev1->xkey.state != event->xkey.state ||
				   ev1->xkey.keycode != event->xkey.keycode ||
				   !IsWaitingEvent()) {
					XPutBackEvent(Xdisplay, ev1);
					break;
				}
				do
					XNextEvent(Xdisplay, ev2);
				while(ev2->type == NoExpose && IsWaitingEvent());
				LLOG("ev2 type:" << ev2->type << " state:" << ev2->xkey.state <<
				     "keycode:" << ev2->xkey.keycode);
				hasev2 = true;
			}
			if(ev2->type != KeyPress ||
			   ev2->xkey.state != event->xkey.state ||
			   ev2->xkey.keycode != event->xkey.keycode) {
				if(hasev2)
					XPutBackEvent(Xdisplay, ev2);
				XPutBackEvent(Xdisplay, ev1);
				break;
			}
			else {
				XFilterEvent(ev1, None);
				if(hasev2)
					XFilterEvent(ev2, None);
			}
			count++;
		}
	case KeyRelease: {
			mousePos = Point(event->xkey.x_root, event->xkey.y_root);
			char buff[128];
			Xeventtime = event->xkey.time;
			LLOG("Key Xeventtime: " << Xeventtime << " count:" << count);
			KeySym keysym;
			int    chr = 0;
			WString wtext;
			if(pressed && w.xic) {
				Status status;
				int len = Xutf8LookupString(w.xic, &event->xkey, buff, sizeof(buff), &keysym, &status);
				buff[len] = 0;
				if(status == XLookupChars || status == XLookupBoth) {
					chr = ToUtf32(buff, len)[0];
					if(status == XLookupChars)
						wtext = ToUtf32(buff, len);
				}
				else
				if(status != XLookupKeySym && status != XLookupBoth)
				    keysym = 0;
			}
			else {
				int len = XLookupString(&event->xkey, buff, sizeof(buff), &keysym, NULL);
				buff[len] = 0;
				chr = ToUtf32(buff, len)[0];
				if(len > 1)
					wtext = ToUtf32(buff, len);
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
			static struct {
				KeySym keysym;
				dword key;
			}
			keypadtab[] = {
				{ XK_ISO_Left_Tab, K_TAB|K_SHIFT },
				{ XK_BackSpace,    K_BACKSPACE },
				{ XK_Tab,          K_TAB       },
				{ XK_Return,       K_ENTER     },
				{ XK_KP_Enter,     K_ENTER     },
				{ XK_Escape,       K_ESCAPE    },
				{ XK_space,        K_SPACE     },
				{ XK_KP_Space,     K_SPACE     },
				{ XK_KP_Tab,       K_TAB       },
				{ XK_KP_Enter,     K_ENTER     },
				{ XK_KP_F1,        K_F1        },
				{ XK_KP_F2,        K_F2        },
				{ XK_KP_F3,        K_F3        },
				{ XK_KP_F4,        K_F4        },
				{ XK_KP_Home,      K_HOME      },
				{ XK_KP_Left,      K_LEFT      },
				{ XK_KP_Up,        K_UP        },
				{ XK_KP_Right,     K_RIGHT     },
				{ XK_KP_Down,      K_DOWN      },
				{ XK_KP_Page_Up,   K_PAGEUP    },
				{ XK_KP_Page_Down, K_PAGEDOWN  },
				{ XK_KP_End,       K_END       },
				{ XK_KP_Begin,     K_HOME      },
				{ XK_KP_Insert,    K_INSERT    },
				{ XK_KP_Delete,    K_DELETE    },
			},
			numpadtab[] = {
				{ XK_KP_0,         K_NUMPAD0   },
				{ XK_KP_1,         K_NUMPAD1   },
				{ XK_KP_2,         K_NUMPAD2   },
				{ XK_KP_3,         K_NUMPAD3   },
				{ XK_KP_4,         K_NUMPAD4   },
				{ XK_KP_5,         K_NUMPAD5   },
				{ XK_KP_6,         K_NUMPAD6   },
				{ XK_KP_7,         K_NUMPAD7   },
				{ XK_KP_8,         K_NUMPAD8   },
				{ XK_KP_9,         K_NUMPAD9   },
				{ XK_KP_Multiply,  K_MULTIPLY  },
				{ XK_KP_Add,       K_ADD       },
				{ XK_KP_Separator, K_SEPARATOR },
				{ XK_KP_Subtract,  K_SUBTRACT  },
				{ XK_KP_Decimal,   K_DECIMAL   },
				{ XK_KP_Divide,    K_DIVIDE    },
			};
			for(int i = 0; i < __countof(keypadtab); i++) {
				if(keypadtab[i].keysym == keysym) {
					DispatchKey(KEYtoK(keypadtab[i].key|up), count);
					return;
				}
			}
			if(GetCtrl() || GetAlt()) // Send Numpad keycodes.
				for(int i = 0; i < __countof(numpadtab); i++) {
				if(numpadtab[i].keysym == keysym) {
					DispatchKey(KEYtoK(numpadtab[i].key|up), count);
					return;
				}
			}
			if(GetShift() && chr == 0) {
				static dword k[] = { 41, 33, 64, 35, 36, 37, 94, 38, 42, 40 };
				for(int i = 0; i < 10; i++)
					if(keysym == k[i]) {
						DispatchKey(KEYtoK(i + K_0)|up, count);
						return;
					}
			}
			if(GetCtrl() || GetAlt()) { // fix Ctrl+Shift+1 etc...
				keysym = decode((int)event->xkey.keycode,
								0x0a, 0x31,
								0x0b, 0x32,
								0x0c, 0x33,
								0x0d, 0x34,
								0x0e, 0x35,
								0x0f, 0x36,
								0x10, 0x37,
								0x11, 0x38,
								0x12, 0x39,
								0x13, 0x30, keysym);
			}
			// DLOG("keysym: " << keysym << " " << (char)keysym);
			if(keysym >= '0' && keysym <= '9' && (chr == 0 || GetCtrl() || GetAlt())) {
				DispatchKey(KEYtoK(keysym - '0' + K_0)|up, count);
				return;
			}
			if(chr >= 1 && chr < 32) {
				DispatchKey(KEYtoK(chr - 1 + K_CTRL_A)|up, count);
				return;
			}
			if(keysym >= 0xff80 && keysym <= 0xffb9 && chr) {
				DispatchKey(KEYtoK(chr)|up, count);
				return;
			}
			if(keysym >= 0xff00 && chr < 128 ||
			   (GetCtrl() || GetAlt()) && keysym >= 0x20 && keysym < 0x7f) {
				if(keysym >= 'a' && keysym <= 'z')
					keysym = keysym - 'a' + 'A';
				DispatchKey(KEYtoK(keysym|K_DELTA)|up, count);
				return;
			}

			if((chr == 32 || chr == 9 || chr == 13) && !pressed)
				DispatchKey(chr|K_KEYUP, count);
			if(chr && pressed) {
				DispatchKey(chr, count);
				for(int ii = 1; ii < wtext.GetLength(); ii++)
					DispatchKey(wtext[ii], count);
			}
		}
		break;
	case ButtonPress: {
			if(!HasWndFocus() && !popup)
				SetWndFocus();
			ClickActivateWnd();
			mousePos = Point(event->xbutton.x_root, event->xbutton.y_root);
			ReleaseGrab();
			XButtonEvent& e = event->xbutton;
			sModState = e.state;
			Xeventtime = e.time;
			if(ignoreclick) break;
			Point p = Point(e.x, e.y);
			dword action = DOWN;
			if((dword)e.time - (dword)Xbuttontime < 800) {
				action = DOUBLE;
				Xbuttontime = Xeventtime - 0x80000000;
			}
			else {
				Xbuttontime = e.time;
				Xbuttonpos = mousePos;
			}
			switch(e.button) {
			case Button1:
				sModState |= Button1Mask;
				DispatchMouse(LEFT|action, p, 0);
				break;
			case Button2:
				sModState |= Button2Mask;
				if(Xbuttons < 3)
					DispatchMouse(RIGHT|action, p, 0);
				else
					DispatchMouse(MIDDLE|action, p, 0);
				break;
			case Button3:
				sModState |= Button3Mask;
				DispatchMouse(RIGHT|action, p, 0);
				break;
			}
			if(_this) PostInput();
		}
		break;
	case ButtonRelease: {
			mousePos = Point(event->xbutton.x_root, event->xbutton.y_root);
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
			if(ignoreclick)
				EndIgnore();
			else
				switch(e.button) {
				case Button1:
					DispatchMouse(LEFTUP, p, 0);
					break;
				case Button2:
					if(Xbuttons < 3)
						DispatchMouse(RIGHTUP, p, 0);
					else
						DispatchMouse(MIDDLEUP, p, 0);
					break;
				case Button3:
					DispatchMouse(RIGHTUP, p, 0);
					break;
				case Button4:
					DispatchMouse(MOUSEWHEEL, p, 120);
					break;
				case Button5:
					DispatchMouse(MOUSEWHEEL, p, -120);
					break;
				}
			if(_this) PostInput();
		}
		break;
	case MotionNotify:
		Window xwin = GetWindow();
		if(xwin) {
			while(XCheckWindowEvent(Xdisplay, xwin, PointerMotionMask, event));
			EndIgnore();
			mousePos = Point(event->xmotion.x_root, event->xmotion.y_root);
			Xeventtime = event->xmotion.time;
			Point p = mousePos - Xbuttonpos;
			if(max(abs(p.x), abs(p.y)) > 4)
				Xbuttontime = Xeventtime - 0x80000000;
			sModState = event->xmotion.state;
			DispatchMouse(MOUSEMOVE, Point(event->xmotion.x, event->xmotion.y));
			DoCursorShape();
		}
		break;
	}
	DropEvent(w, event);
}

}

#endif
