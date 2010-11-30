#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <SDL/SDLCtrl.h>
#include <SDLCtrl_demo/demo1.h>

const char * scroll[]=
{
	"ULTIMATE++",
	"WHEN PROGRAMMING",
	"IS FUN",
	"THIS IS A SIMPLE",
	"SDL DEMO",
	"CODED BY UNO",
	"ADAPTED TO SDLCTRL",
	"BY KOLDO",
	"ENJOY THE SCROLL"
};

const ScreenFont fntdef[50] = {
	0,0,32,51, 35,0,65,51, 71,0,101,52, 107,0,137,51, 143,0,167,51, 171,0,192,51,
	197,0,227,52, 233,0,264,51, 270,0,283,51, 288,0,305,51, 311,0,343,51,
	346,0,367,51, 371,0,412,51, 0,58,30,109, 36,58,66,110, 72,58,100,109,
	105,58,135,115, 141,58,170,109, 175,58,205,110, 208,58,237,109,
	241,58,271,110, 274,58,308,109, 310,58,361,109, 362,58,393,109,
	394,58,424,109, 426,58,449,109,
	/* 0 */
	0,120,29,172, 33,120,54,171, 60,120,86,171, 93,120,122,172,
	126,120,157,171, 162,120,190,172, 196,120,226,172, 230,120,254,171,
	258,120,287,172, 293,120,323,172,
	/* ! */
	0,177,15,231, 18,177,66,233, 69,177,107,231, 111,177,140,236,
	146,177,188,232, 191,177,220,205, 223,177,259,232, 261,177,277,194,
	281,177,297,231, 301,177,317,231, 322,177,339,215, 343,177,373,221,
	378,177,392,231, 396,177,410,231
};

const int fntascii[96] = {
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

SDLExample::SDLExample() { 
	done = true;
	demoInitialized = false;
	SetBpp(8);
	SetVideoFlags(GetVideoFlags() | SDL_HWPALETTE);
}

void SDLExample::PutLetter(int x, int y, int n, int col, double ampl) {
	int dx = fntdef[n].x1 - fntdef[n].x0;
	int dy = fntdef[n].y1 - fntdef[n].y0;

	int sx = fntdef[n].x0;
	int sy = fntdef[n].y0;

	int w = surf.GetWidth();
	int h = surf.GetHeight();

	if(x + dx < 0 || y + dy < 0 ||  x > w || y > h)
		return;

	if(x < 0) {
		sx -= x;
		dx += x;
		x = 0;
	}
	if(y < 0) {
		sy -= y;
		dy += y;
		y = 0;
	}

	if(x + dx > w)
		dx = w - x;
	if(y + dy > h)
		dy = h - y;

	for(int j = 0; j < dx; j++) {
		int ads = y + int(sintab[x + j] * ampl);
		for(int i = 0; i < dy; i++) {
			byte c = fntbmp.GetPixelByte(sx + j, sy + i);
			if(c == 1)
				surf.DrawPixel(x + j, ads + i, col);
			else if(c == 3)
				surf.DrawPixel(x + j, ads + i, 0);
		}
	}
}

void SDLExample::WriteStr(int x, int y, const char *str, int col, double ampl) {
	if (surf.GetWidth() == 0 || surf.GetHeight() == 0)
		return;
	while(*str) {
		int n = fntascii[*str];
		if(*str != 32){
			PutLetter(x, y, n, col, ampl);
			x += fntdef[n].x1 - fntdef[n].x0 + 2;
		} else
			x += 20;
		str++;
	}
}

int SDLExample::LengthStr(const char *str) {
	int l = 0, n;

	while(*str) {
		n = fntascii[*str];
		if(*str != 32)
			l += fntdef[n].x1 - fntdef[n].x0 + 2;
		else
			l += 20;

		str++;
	}
	return l;
}

void SDLExample::DrawPoints(ScreenPoint *p) {
	for(int i = 0; i < MAXPOINT; i++)
		surf.DrawPixel(p[i].x, p[i].y, 255);
}

void SDLExample::MovePoints(ScreenPoint *p, int width, int height) {
	int w = width - 1;
	int h = height - 1;

	for(int i = 0; i < MAXPOINT; i++) {
		p[i].x += p[i].sx;
		p[i].y += p[i].sy;

		if(p[i].x > w) {
			p[i].sx = -p[i].sx;
			p[i].x = w;
		}
		if(p[i].x < 0) {
			p[i].sx = -p[i].sx;
			p[i].x = 0;
		}
		if(p[i].y > h) {
			p[i].sy = -p[i].sy;
			p[i].y = h;
		}
		if(p[i].y < 0) {
			p[i].sy = -p[i].sy;
			p[i].y = 0;
		}
	}
}

// Included simpler options commented
void SDLExample::Blur(int width, int height) {
	byte *pos;
	for (int y = 0; y < height-1; ++y) {
		pos = surf.GetPixelPos(0, y);
		for (int x = 0; x < width; ++x) {
			int pixel = (*pos + *(pos+1) +
						// Also possible surf.GetPixelByte(x, y) + surf.GetPixelByte(x+1, y) + 
						 surf.GetPixelByte(x-1, y+1) + surf.GetPixelByte(x+2, y+1)) >> 2;
			*(pos++) = pixel;		// Also possible surf.DrawPixel(x, y, pixel);
		}
	}
	pos = surf.GetPixelPos(0, height-1);
	for (int x = 0; x < width; ++x) 
		*(pos++) = 0;				// Also possible surf.DrawPixel(x, height-1, 0);
}

void SDLExample::SetupPalette(SDL_Surface *screen) {
	SDL_Color pal[256];

	double d = 20;
	double dx = 63.0 / 256.0;

	for(int i = 0; i < 254; i++) {
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

void SDLExample::Layout() {
	SDLCtrl::Layout();
	if (!IsReady())
		return;
	
	Rect r = GetRect();
	for(int i = 0; i < r.GetWidth(); i++)
		sintab[i] = sin(i * M_PI / 180.0);

	for(int i = 0; i < MAXPOINT; i++) {
		points[i].x   = rand() % (r.GetWidth() - 1);
		points[i].y   = rand() % (r.GetHeight() - 1);
		points[i].sx  = (rand() & 1) + 1;
		points[i].sy  = (rand() & 1) + 1;
		points[i].col = (rand() % 255);
	}
	
	if (!demoInitialized)
		return;
	
	int width = GetWidth();
	int height = GetHeight();
	
	surf.Resize(width, height);
	
	SetupPalette(surf.GetSurface());
	SetupPalette(GetSurface());
	
	j = 0;
	k = 0;
	len0 = LengthStr(scroll[j]);
	len1 = len0;
	xmax = (width - len1) / 2;
	x0 = width;
	x1 = -len0;	
}

void SDLExample::Demo() {
	if(!fntbmp.LoadBMP(GetDataFile("font.bmp"))) {
		Exclamation(Format("Error loading font.bmp : %s\n", SDL_GetError()));
		return;
	}
	Rect r = GetRect();
	int width = r.GetWidth();
	int height = r.GetHeight();

	if (!surf.CreateRGB(width, height, GetBpp()))
		return;

	SetupPalette(surf.GetSurface());
	SetupPalette(GetSurface());

	demoInitialized = true;
	done = false;
	
	j = 0;
	k = 0;
	len0 = LengthStr(scroll[j]);
	len1 = len0;
	xmax = (width - len1) / 2;
	x0 = width;
	x1 = -len0;

	SDL_Event event;

	while(!done) {
		Ctrl::ProcessEvents();
		if(SDL_PollEvent(&event))
			switch (event.type) {
				case SDL_MOUSEMOTION:
					r = GetScreenView();
					GetParent()->MouseMove(Point(r.left+event.motion.x, r.top+event.motion.y), 0);				
					break;
				case SDL_QUIT:
					done = true;
					break;
				default:
					break;
			} else {
				r = GetRect();
				width = GetWidth();
				height = GetHeight();
			
				surf.Lock();
				
				DrawPoints(points);
				Blur(width, height);
				MovePoints(points, width, height);

				if(x0 > -len0)
					WriteStr(x0 + 5, height - 185, scroll[k], 254, 50);

				if(x1 <= xmax)
					WriteStr(x1, height - 250, scroll[j], 255, 50);

				x0 -= 2;
				x1 += 2;

				if(x0 < -len0) {
					if(++k > MAXSCROLL - 1) 
						k = 0;
					len0 = LengthStr(scroll[k]);
					x0 = width;
				}
				if(x1 > xmax) {
					if(++j > MAXSCROLL - 1) 
						j = 0;
					len1 =LengthStr(scroll[j]);
					xmax = (width - len1) / 2;
					x1 = -len1;
				}
				surf.Unlock();
				
				Blit(surf);
			}
	}
}
