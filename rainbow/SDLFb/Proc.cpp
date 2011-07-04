#include "SDLFbLocal.h"

NAMESPACE_UPP

#define LLOG(x)       LOG(x)

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

void HandleUserEvents(SDL_Event* event)
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
