#include "SDLFbLocal.h"

NAMESPACE_UPP

#define LLOG(x)       //LOG(x)

SDL_Surface *CreateScreen(int w, int h, int bpp, int flags)
{
	SDL_Surface * screen = SDL_SetVideoMode(w, h, bpp, flags);
	if(!screen)
	{
		Cout() << Format("Couldn't set display mode: %s\n", SDL_GetError());
		return NULL;
	}
	Cout() << Format("Screen is in %s mode\n", (screen->flags & SDL_FULLSCREEN) ? "fullscreen" : "windowed");
	return screen;
}

//GetModState ??
bool GetShift()       { uint8* ka = SDL_GetKeyState(NULL); return ka[SDLK_LSHIFT] || ka[SDLK_RSHIFT]; }
bool GetCtrl()        { uint8* ka = SDL_GetKeyState(NULL); return ka[SDLK_LCTRL]  || ka[SDLK_RCTRL]; }
bool GetAlt()         { uint8* ka = SDL_GetKeyState(NULL); return ka[SDLK_LALT]   || ka[SDLK_RALT]; }
bool GetCapsLock()    { uint8* ka = SDL_GetKeyState(NULL); return ka[SDLK_CAPSLOCK]; }
bool GetMouseLeft()   { return (SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)); }
bool GetMouseRight()  { return (SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)); }
bool GetMouseMiddle() { return (SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE)); }

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
	switch(event->type) {
		case SDL_ACTIVEEVENT: //SDL_ActiveEvent
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP: //SDL_KeyboardEvent
		{
			bool b = false;
			dword keycode = 0;
			if(event->type == SDL_KEYDOWN) {
				keycode = fbKEYtoK((dword)event->key.keysym.sym);
				if(keycode != K_SPACE) //dont send space on keydown
					b = Ctrl::DoKeyFB(keycode, 1);

				//send respective keyup things as char events as well
				keycode = (dword)event->key.keysym.unicode;
				if((keycode != 127 && keycode >= 32 && keycode < 255))
					b = Ctrl::DoKeyFB(keycode, 1);
			}
			else
			if(event->type == SDL_KEYUP)
			{
				keycode = fbKEYtoK((dword)event->key.keysym.sym) | K_KEYUP;
				b = Ctrl::DoKeyFB(keycode, 1);
			}
		}
			break;
		case SDL_MOUSEMOTION: //SDL_MouseMotionEvent
			Ctrl::DoMouseFB(Ctrl::MOUSEMOVE, Point(event->motion.x, event->motion.y));
			break;
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
					//case SDL_BUTTON_WHEELUP: Ctrl::DoMouseFB(Ctrl::MOUSEWHEELDOUBLE, p, +120); break;
					//case SDL_BUTTON_WHEELDOWN: Ctrl::DoMouseFB(Ctrl::MOUSEWHEELDOUBLE, p, -120); break;
				}
				isdblclick[bi] = 0; //reset, to go ahead sending repeats
			}
			else
			{
				lastbdowntime[bi] = ct;
				isdblclick[bi] = 0; //prepare for repeat
				switch(bi)
				{
					case SDL_BUTTON_LEFT: Ctrl::DoMouseFB(Ctrl::LEFTDOWN, p); break;
					case SDL_BUTTON_RIGHT: Ctrl::DoMouseFB(Ctrl::RIGHTDOWN, p); break;
					case SDL_BUTTON_MIDDLE: Ctrl::DoMouseFB(Ctrl::MIDDLEDOWN, p); break;
					case SDL_BUTTON_WHEELUP: Ctrl::DoMouseFB(Ctrl::MOUSEWHEEL, p, +120); break;
					case SDL_BUTTON_WHEELDOWN: Ctrl::DoMouseFB(Ctrl::MOUSEWHEEL, p, -120); break;
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
				case SDL_BUTTON_LEFT: Ctrl::DoMouseFB(Ctrl::LEFTUP, p); break;
				case SDL_BUTTON_RIGHT: Ctrl::DoMouseFB(Ctrl::RIGHTUP, p); break;
				case SDL_BUTTON_MIDDLE: Ctrl::DoMouseFB(Ctrl::MIDDLEUP, p); break;
				case SDL_BUTTON_WHEELUP: Ctrl::DoMouseFB(Ctrl::MOUSEWHEEL, p, +120); break;
				case SDL_BUTTON_WHEELDOWN: Ctrl::DoMouseFB(Ctrl::MOUSEWHEEL, p, -120); break;
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
		case SDL_VIDEORESIZE: //SDL_ResizeEvent
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
		case SDL_QUIT: //SDL_QuitEvent
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
