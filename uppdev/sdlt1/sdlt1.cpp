#include "sdlt1.h"

SDL_Surface * OpenScreen(const int width, const int height, const int bpp, const int flags)
{
	SDL_Surface * screen = SDL_SetVideoMode(width, height, bpp, flags);
	if(!screen)
	{
		printf("Couldn't set display mode: %s\n", SDL_GetError());
		return NULL;
	}
	return screen;
}

void Paint(SDL_Surface * surface)
{
	/* Put your painting code here */
}

int main(int argc, char **argv)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;

	int videoflags = SDL_HWSURFACE | SDL_HWACCEL | SDL_FULLSCREEN | SDL_DOUBLEBUF;
	SDL_Surface * screen = OpenScreen(1024, 768, 32, videoflags);
	if(!screen)
		return 1;

	SDL_Event event;
	bool done = false;

	while(!done)
	{
		if(SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_LALT || event.key.keysym.sym == SDLK_TAB)
						break;
					if(event.key.keysym.sym == SDLK_RETURN)
					{
						videoflags ^= SDL_FULLSCREEN;
						screen = OpenScreen(screen->w, screen->h, screen->format->BitsPerPixel, videoflags);
						if(!screen)
							done = true;
						break;
					}
				case SDL_QUIT:
					done = true;
					break;
				default:
					break;
			}
		}
		else
		{
			Paint(screen);
			SDL_Flip(screen);
		}
	}
	SDL_FreeSurface(screen);
	SDL_Quit();
	return 0;
}
