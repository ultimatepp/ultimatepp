#include "SDLFbLocal.h"

NAMESPACE_UPP

dword fbKEYtoK(dword chr);

SDL_Surface * screen = NULL;
bool fbEndSession = false;
int videoflags = 0;
int height = 0;
int width = 0;
int bpp = 0;

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

bool FBEndSession()
{
	return fbEndSession;
}

bool FBIsWaitingEvent()
{
	SDL_PumpEvents();
	SDL_Event events;
	int tc = SDL_PeepEvents(&events, 1, SDL_PEEKEVENT, SDL_EVENTMASK(SDL_ALLEVENTS));
	return (tc>0);
}

bool FBProcessEvent(bool *quit)
{
	SDL_Event event;
	if(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_ACTIVEEVENT: //SDL_ActiveEvent
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP: //SDL_KeyboardEvent
			{
				String msgdump;
				switch(event.type)
				{
				case SDL_KEYDOWN:    msgdump << "SDL_KEYDOWN"; break;
				case SDL_KEYUP:      msgdump << "SDL_KEYUP"; break;
				}
	//			msgdump << " wParam = 0x" << FormatIntHex(wParam, 8)
	//				<< ", lParam = 0x" << FormatIntHex(lParam, 8);
				msgdump << (int)event.key.keysym.sym;
				RLOG(msgdump);

				bool b = false;
				dword keycode = 0;
				if(event.type == SDL_KEYDOWN) {
					keycode = fbKEYtoK((dword)event.key.keysym.sym);
					if(keycode != K_SPACE) //dont send space on keydown
						b = Ctrl::DoKeyFB(keycode, 1);

					//send respective keyup things as char events as well
					keycode = (dword)event.key.keysym.unicode;
					if((keycode != 127 && keycode > 32 && keycode < 255) 
						|| (keycode == 32 && fbKEYtoK(SDLK_SPACE) == K_SPACE))
						b = Ctrl::DoKeyFB(keycode, 1);

				}
				else
				if(event.type == SDL_KEYUP)
				{
					keycode = fbKEYtoK((dword)event.key.keysym.sym) | K_KEYUP;
					b = Ctrl::DoKeyFB(keycode, 1);
				}
			}
				break;
			case SDL_MOUSEMOTION: //SDL_MouseMotionEvent
				Ctrl::DoMouseFB(Ctrl::MOUSEMOVE, Point(event.motion.x, event.motion.y));
				break;
			case SDL_MOUSEBUTTONDOWN: //SDL_MouseButtonEvent, FIXME DoubleClick
			{
				Point p(event.button.x, event.button.y);
				switch(event.button.button)
				{
					case SDL_BUTTON_LEFT: Ctrl::DoMouseFB(Ctrl::LEFTDOWN, p); break;
					case SDL_BUTTON_RIGHT: Ctrl::DoMouseFB(Ctrl::RIGHTDOWN, p); break;
					case SDL_BUTTON_MIDDLE: Ctrl::DoMouseFB(Ctrl::MIDDLEDOWN, p); break;
					case SDL_BUTTON_WHEELUP: Ctrl::DoMouseFB(Ctrl::MOUSEWHEEL, p, +120); break;
					case SDL_BUTTON_WHEELDOWN: Ctrl::DoMouseFB(Ctrl::MOUSEWHEEL, p, -120); break;
				}
			}
				break;
			case SDL_MOUSEBUTTONUP:
			{
				Point p(event.button.x, event.button.y);
				switch(event.button.button)
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
				width = event.resize.w;
				height = event.resize.h;
		
				SDL_FreeSurface(screen);
				screen = CreateScreen(width, height, bpp, videoflags);
				ASSERT(screen);
				Ctrl::SetFramebufferSize(Size(width, height));
			}
				break;
			case SDL_VIDEOEXPOSE: //SDL_ExposeEvent
				break;
			case SDL_QUIT: //SDL_QuitEvent
				if(quit) *quit = true;
				break;
			case SDL_USEREVENT: //SDL_UserEvent
				HandleUserEvents(&event);
				break;
			case SDL_SYSWMEVENT: //SDL_SysWMEvent
				break;
			
			default:
			break;
		}   // End switch
		return true;
	}   // End while
	return false;
}

void FBSleep(int ms)
{
	Sleep(ms); //sleep should be wakeable with input
}

void FBUpdate(const Rect& inv)
{
	//The invalidated areas accumulate in the update region until the region is processed when the next WM_PAINT message occurs
	SDL_LockSurface(screen);

	const ImageBuffer& framebuffer = Ctrl::GetFrameBuffer();

	memcpy(screen->pixels, (const char*)~framebuffer, framebuffer.GetLength() * sizeof(RGBA));
	SDL_UnlockSurface(screen);
	SDL_Flip(screen);
}

void FBInit()
{
	GuiLock __;
	
	Ctrl::InitFB();

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Cout() << Format("Couldn't initialize SDL: %s\n", SDL_GetError());
		return;
	}

	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL/2);

	const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	//ASSERT(vi->hw_available);

	width = vi->current_w;
	height = vi->current_h;
	//width = 600;
	//height = 400;
	bpp = vi->vfmt->BitsPerPixel;
	ASSERT(bpp == 32);
	
	//FIXME adjustable
	videoflags = SDL_HWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF | SDL_RESIZABLE;// | SDL_NOFRAME | SDL_FULLSCREEN;

	screen = CreateScreen(width, height, bpp, videoflags);
	ASSERT(screen);
	Ctrl::SetFramebufferSize(Size(width, height));

//	Csizeinit();
}

void FBDeInit()
{
	SDL_FreeSurface(screen);
	SDL_Quit();
}

END_UPP_NAMESPACE
