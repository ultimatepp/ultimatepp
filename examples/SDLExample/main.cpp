#include <Core/Core.h>

using namespace Upp;

#ifdef COMPILER_MSC
#include <SDL.h>
#endif
#ifdef COMPILER_GCC
#include <SDL/SDL.h>
#endif

const int maxpoint  = 1000;
const int maxscroll = 7;
const int width  = 640;
const int height = 480;
const int bpp    = 8;

struct ScreenPoint
{
	int x, y;
	int sx, sy;
	int col;
};

struct ScreenFont
{
int x0, y0;
int x1, y1;
};

double sintab[width];

SDL_Surface *fntbmp;

const char * scroll[]=
{
	"ULTIMATE++",
	"WHEN PROGRAMMING",
	"IS FUN",
	"THIS IS A SIMPLE",
	"SDL DEMO",
	"CODED BY UNO",
	"ENJOY THE SCROLL"
};

const ScreenFont fntdef[] =
{
	{ 0,0,32,51 }, { 35,0,65,51 }, { 71,0,101,52 }, { 107,0,137,51 }, { 143,0,167,51 }, { 171,0,192,51, },
	{ 197,0,227,52 }, { 233,0,264,51 }, { 270,0,283,51 }, { 288,0,305,51 }, { 311,0,343,51, },
	{ 346,0,367,51 }, { 371,0,412,51 }, { 0,58,30,109 }, { 36,58,66,110 }, { 72,58,100,109, },
	{ 105,58,135,115 }, { 141,58,170,109 }, { 175,58,205,110 }, { 208,58,237,109, },
	{ 241,58,271,110 }, { 274,58,308,109 }, { 310,58,361,109 }, { 362,58,393,109, },
	{ 394,58,424,109 }, { 426,58,449,109, },
	{ /* 0 */ },
	{ 0,120,29,172 }, { 33,120,54,171 }, { 60,120,86,171 }, { 93,120,122,172, },
	{ 126,120,157,171 }, { 162,120,190,172 }, { 196,120,226,172 }, { 230,120,254,171, },
	{ 258,120,287,172 }, { 293,120,323,172, },
	{ /* ! */ },
	{ 0,177,15,231 }, { 18,177,66,233 }, { 69,177,107,231 }, { 111,177,140,236, },
	{ 146,177,188,232 }, { 191,177,220,205 }, { 223,177,259,232 }, { 261,177,277,194, },
	{ 281,177,297,231 }, { 301,177,317,231 }, { 322,177,339,215 }, { 343,177,373,221, },
	{ 378,177,392,231 }, { 396,177,410,231 },
};

const int fntascii[96] =
{
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	36,0,38,39,40,42,0,44,45,43,47,0,46,0,0,
	26,27,28,29,30,31,32,33,34,35,
	0,0,0,0,0,0,37,
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
	17,18,19,20,21,22,23,24,25,
	48,0,49,41
};

void PutLetter(SDL_Surface * screen, int x, int y, int n, int col, double ampl)
{
	int dx = fntdef[n].x1 - fntdef[n].x0;
	int dy = fntdef[n].y1 - fntdef[n].y0;

	int sx = fntdef[n].x0;
	int sy = fntdef[n].y0;

	int w = screen->w;
	int h = screen->h;

	if(x + dx < 0 || y + dy < 0 ||  x > w || y > h)
		return;

	if(x < 0)
	{
		sx -= x;
		dx += x;
		x = 0;
	}
	if(y < 0)
	{
		sy -= y;
		dy += y;
		y = 0;
	}

	if(x + dx > w)
	dx = w - x;
	if(y + dy > h)
	dy = h - y;

	Uint8 * b = (Uint8 *) screen->pixels;
	Uint8 * f = (Uint8 *) fntbmp->pixels;

	for(int j = 0; j < dx; j++)
	{
		int ads = (y + (int) (sintab[x + j] * ampl)) * screen->w + x;
		int adf = sy * fntbmp->w + sx;
		for(int i = 0; i < dy; i++)
		{
			char c = f[adf + j];
			if(c == 1)
				b[ads + j] = col;
			else if(c == 3)
				b[ads + j] = 0;
			ads += screen->w;
			adf += fntbmp->w;
		}
	}
}

void WriteStr(SDL_Surface * screen, int x, int y, const char *str, int col, double ampl)
{
	while(*str)
	{
		int n = fntascii[(byte)*str];
		if(*str != 32)
		{
			PutLetter(screen, x, y, n, col, ampl);
			x += fntdef[n].x1 - fntdef[n].x0 + 2;
		}
		else
			x += 20;
		str++;
	}
}

int LengthStr(const char *str)
{
	int l = 0, n;

	while(*str)
	{
		n = fntascii[(byte)*str];
		if(*str != 32)
			l += fntdef[n].x1 - fntdef[n].x0 + 2;
		else
			l += 20;

		str++;
	}
	return l;
}

void DrawPoints(SDL_Surface *screen, ScreenPoint *p)
{
	Uint8 * b = (Uint8 *) screen->pixels;

	for(int i = 0; i < maxpoint; i++)
		b[p[i].y * screen->w + p[i].x] = 255;
}


void MovePoints(ScreenPoint *p)
{
	int w = width - 1;
	int h = height - 1;

	for(int i = 0; i < maxpoint; i++)
	{
		p[i].x += p[i].sx;
		p[i].y += p[i].sy;

		if(p[i].x > w)
		{
			p[i].sx = -p[i].sx;
			p[i].x = w;
		}
		if(p[i].x < 0)
		{
			p[i].sx = -p[i].sx;
			p[i].x = 0;
		}
		if(p[i].y > h)
		{
			p[i].sy = -p[i].sy;
			p[i].y = h;
		}
		if(p[i].y < 0)
		{
			p[i].sy = -p[i].sy;
			p[i].y = 0;
		}
	}
}

void Blur(SDL_Surface *screen)
{
	Uint8 * b = (Uint8 *) screen->pixels;

	int i = 0;
	for(; i < width * (height - 1); i++)
		b[i] = (b[i] + b[i + 1] + b[i + width - 1] + b[i + width + 2]) >> 2;

	for(; i < width * height; i++)
		b[i] = 0;

}

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

void SetupPalette(SDL_Surface * screen)
{
	SDL_Color pal[256];

	double d = 20;
	double dx = 63.0 / 256.0;

	for(int i = 0; i < 254; i++)
	{
		pal[i].r = (Uint8) d;
		pal[i].g = (Uint8) d;
		pal[i].b = 0;

		d += dx;
	}
	pal[254].r = 0;
	pal[254].g = 150;
	pal[254].b = 255;

	pal[255].r = 255;
	pal[255].g = 255;
	pal[255].b = 0;

	SDL_SetColors(screen, pal, 0, 256);
	SDL_Flip(screen);
	SDL_SetColors(screen, pal, 0, 256);
}

CONSOLE_APP_MAIN
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Cout() << Format("Couldn't initialize SDL: %s\n", SDL_GetError());
		return;
	}

	ScreenPoint points[maxpoint];

	for(int i = 0; i < width; i++)
		sintab[i] = sin(i * M_PI / 180.0);

	for(int i = 0; i < maxpoint; i++)
	{
		points[i].x   = rand() % (width - 1);
		points[i].y   = rand() % (height - 1);
		points[i].sx  = (rand() & 1) + 1;
		points[i].sy  = (rand() & 1) + 1;
		points[i].col = (rand() % 255);
	}

	fntbmp = SDL_LoadBMP(GetDataFile("font.bmp"));
	if(!fntbmp)
	{
		Cout() << Format("Error loading font.bmp : %s\n", SDL_GetError());
		return;
	}

	int videoflags = SDL_HWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF | SDL_FULLSCREEN;

	SDL_Surface * screen = CreateScreen(width, height, bpp, videoflags);
	if(!screen)
		return;

	SDL_Surface * surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, bpp, 0, 0, 0, 0);
	if(!surface)
		return;

	SetupPalette(surface);
	SetupPalette(screen);

	int j = 0;
	int k = 0;
	int len0 = LengthStr(scroll[j]);
	int len1 = len0;
	int xmax = (width - len1) / 2;
	int x0 = width;
	int x1 = -len0;

	SDL_Event event;
	bool done = false;

	while(!done)
	{
		if(SDL_PollEvent(&event))
			switch (event.type)
			{
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_LALT || event.key.keysym.sym == SDLK_TAB)
						break;

					if(event.key.keysym.sym == SDLK_LALT && event.key.keysym.sym == SDLK_RETURN)
					{
						videoflags ^= SDL_FULLSCREEN;
						screen = CreateScreen(screen->w, screen->h, screen->format->BitsPerPixel, videoflags);
						if(!screen)
						{
							Cout() << "Couldn't toggle fullscreen mode\n";
							done = true;
						}
						SetupPalette(surface);
						SetupPalette(screen);
						break;
					}
				case SDL_QUIT:
					done = true;
					break;
				default:
					break;
			}
			else
			{
				SDL_LockSurface(surface);

				DrawPoints(surface, points);
				Blur(surface);
				MovePoints(points);

				if(x0 > -len0)
					WriteStr(surface, x0 + 5, height - 195, scroll[k], 254, 50);

				if(x1 <= xmax)
					WriteStr(surface, x1, height - 260, scroll[j], 255, 50);

				x0 -= 1;
				x1 += 1;

				if(x0 < -len0)
				{
					if(++k > maxscroll - 1) k = 0;
					len0 = LengthStr(scroll[k]);
					x0 = width;
				}

				if(x1 > xmax)
				{
					if(++j > maxscroll - 1) j = 0;
					len1 = LengthStr(scroll[j]);
					xmax = (width - len1) / 2;
					x1 = -len1;
				}

				SDL_UnlockSurface(surface);
				SDL_BlitSurface(surface, NULL, screen, NULL);
				SDL_Flip(screen);
			}
	}
	SDL_FreeSurface(surface);
	SDL_FreeSurface(screen);
	SDL_FreeSurface(fntbmp);
	SDL_Quit();
}
