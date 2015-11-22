#include "Local.h"

#ifdef GUI_SDL20

NAMESPACE_UPP

#define LLOG(x)       //LOG(x)

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
void HandleSDLEvent(SDL_Event* event)
{
	DLOG("HandleSDLEvent " << event->type);
	switch(event->type) {
//		case SDL_ACTIVEEVENT: //SDL_ActiveEvent
//			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP: //SDL_KeyboardEvent
		{
//			bool b = false;
			dword keycode = 0;
			if(event->type == SDL_KEYDOWN) {
				switch(event->key.keysym.sym)
				{
					case SDLK_LSHIFT: modkeys |= KM_LSHIFT; break;
					case SDLK_RSHIFT: modkeys |= KM_RSHIFT; break;	
					case SDLK_LCTRL: modkeys |= KM_LCTRL; break;
					case SDLK_RCTRL: modkeys |= KM_RCTRL; break;	
					case SDLK_LALT: modkeys |= KM_LALT; break;
					case SDLK_RALT: modkeys |= KM_RALT; break;
					default:;
				}
				
				keycode = fbKEYtoK((dword)event->key.keysym.sym);
				if(keycode != K_SPACE) //dont send space on keydown
					/*b = */Ctrl::DoKeyFB(keycode, 1);

				//send respective keyup things as char events as well
				keycode = (dword)event->key.keysym.unicode;
				if((keycode != 127 && keycode >= 32 && keycode < 255))
					/*b = */Ctrl::DoKeyFB(keycode, 1);
			}
			else
			if(event->type == SDL_KEYUP)
			{
				switch(event->key.keysym.sym)
				{
					case SDLK_LSHIFT: modkeys &= ~KM_LSHIFT; break;
					case SDLK_RSHIFT: modkeys &= ~KM_RSHIFT; break;	
					case SDLK_LCTRL: modkeys &= ~KM_LCTRL; break;
					case SDLK_RCTRL: modkeys &= ~KM_RCTRL; break;	
					case SDLK_LALT: modkeys &= ~KM_LALT; break;
					case SDLK_RALT: modkeys &= ~KM_RALT; break;	
					default:;
				}

				keycode = fbKEYtoK((dword)event->key.keysym.sym) | K_KEYUP;
				/*b = */Ctrl::DoKeyFB(keycode, 1);
			}
		}
			break;
		case SDL_MOUSEMOTION: //SDL_MouseMotionEvent
			Ctrl::DoMouseFB(Ctrl::MOUSEMOVE, Point(event->motion.x, event->motion.y));
			break;
		case SDL_MOUSEWHEEL: Ctrl::DoMouseFB(Ctrl::MOUSEWHEEL, GetMousePos(), sgn(event->wheel.y) * 120); break;
		case SDL_MOUSEBUTTONDOWN: //SDL_MouseButtonEvent, FIXME DoubleClick
		{
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
		case SDL_MOUSEBUTTONUP:
		{
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
		case SDL_JOYAXISMOTION: //SDL_JoyAxisEvent
			break;
		case SDL_JOYBALLMOTION: //SDL_JoyBallEvent
			break;
		case SDL_JOYHATMOTION: //SDL_JoyHatEvent
			break;
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP: //SDL_JoyButtonEvent
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
			break;
*/		case SDL_QUIT: //SDL_QuitEvent
			Ctrl::EndSession();
			break;
		case SDL_USEREVENT: //SDL_UserEvent
			HandleUserEvent(event);
			break;
		case SDL_SYSWMEVENT: //SDL_SysWMEvent
			break;
		
		default:
		break;
	}   // End switch
}

void HandleUserEvent(SDL_Event* event)
{
/*
	switch (event->user.code) {
		case RUN_GAME_LOOP:
			GameLoop();
		break;
		
		default:
		break;
	}
*/
}

END_UPP_NAMESPACE

#endif
