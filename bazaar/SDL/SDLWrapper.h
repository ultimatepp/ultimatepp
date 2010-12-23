#ifndef _SDL_SDLWrapper_h_
#define _SDL_SDLWrapper_h_

#if defined(_WIN32)
	#include <SDL.h> 
	#include <SDL_thread.h> 
	#include <SDL_syswm.h>
#else
	#include <SDL/SDL.h>
	#include <SDL/SDL_thread.h>
	#include <SDL/SDL_syswm.h>
#endif

#undef main

#define INT64_C(val) val##LL
#define UINT64_C(val) val##ULL 

#endif
