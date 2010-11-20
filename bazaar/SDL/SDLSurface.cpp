#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include "SDLCtrl.h"

SDLSurface::SDLSurface() {
	surface = 0;
	del = false;
}

SDLSurface::SDLSurface(SDL_Surface *_surface, bool _del) {
	surface = _surface;
	del = _del;
}

SDLSurface::~SDLSurface() {
	if (del) 
		SDL_FreeSurface(surface);
}

inline Uint32 SDLSurface::GetColor(Color color) {
	return SDL_MapRGB(surface->format, color.GetR(), color.GetG(), color.GetB());
}

void SDLSurface::Lock() {
	SDL_LockSurface(surface);
}

void SDLSurface::Unlock() {
	SDL_UnlockSurface(surface);
}

inline byte *SDLSurface::GetPixelPos(int x, int y) {
	return (byte *)surface->pixels + surface->pitch*y + surface->format->BytesPerPixel*x;
}

void SDLSurface::DrawPixel(int x, int y, Color color) {
	Uint32 scolor = GetColor(color);
	memcpy(GetPixelPos(x, y), &scolor, surface->format->BytesPerPixel);
}

inline void SDLSurface::DrawPixel(int x, int y, Uint32 scolor) {
	memcpy(GetPixelPos(x, y), &scolor, surface->format->BytesPerPixel);
}

Color SDLSurface::GetPixel(int x, int y) {
	Uint32 scolor;
	memcpy(&scolor, GetPixelPos(x, y), surface->format->BytesPerPixel);
	Uint8 r, g, b;
	SDL_GetRGB(scolor, surface->format, &r, &g, &b);
	return Color(r, g, b);
}

void SDLSurface::DrawImage(Image img, int x0, int y0, Color transparent) {
	for(int y = 0; y < img.GetHeight(); y++) {
		for(int x = 0; x < img.GetWidth(); x++) {
			if (img[y][x] != transparent) 
				DrawPixel(x + x0, y + y0, img[y][x]);
		}
	} 
}

// Bresenham's line algorithm
void SDLSurface::DrawLine1(int x0, int y0, int x1, int y1, Color color) {
	Uint32 pixel = GetColor(color);
	bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
       	Swap(x0, y0);
      	Swap(x1, y1);
    }
   	if (x0 > x1) {
      	Swap(x0, x1);
      	Swap(y0, y1);
   	}
  	int deltax = x1 - x0;
    int deltay = abs(y1 - y0);
    int error = deltax/2;
    int ystep;
    int y = y0;
    if (y0 < y1)
    	ystep = 1;
    else 
    	ystep = -1;
    for (int x = x0; x < x1; ++x) {
     	if (steep)
     		DrawPixel(y, x, pixel); 
     	else 
     		DrawPixel(x, y, pixel);
       	error = error - deltay;
       	if (error < 0) {
          	y = y + ystep;
            error = error + deltax;
       	}
    }
}

void SDLSurface::DrawLine(int x0, int y0, int x1, int y1, int thick, Color color) {
	if (x0 == x1) {
		if (y1 < y0)
			Swap(y0, y1);
		int t = thick/2;
		FillRect(x0-t, y0, thick, y1-y0, color);	
	} else if (y0 == y1) {
		if (x1 < x0)
			Swap(x0, x1);
		int t = thick/2;
		FillRect(x0, y0-t, x1-x0, thick, color);	
	} else
		DrawLine1(x0, y0, x1, y1, color); // Pending
}

void SDLSurface::DrawRect(int x, int y, int width, int height, int thick, Color color) {
	int t = thick/2;
	FillRect(x+t, y-t, width-thick, thick, color);
	FillRect(x+t, y-t+height, width-thick, thick, color);
	FillRect(x-t, y-t, thick, height+thick, color);
	FillRect(x-t+width, y-t, thick, height+thick, color);
}

void SDLSurface::FillRect(int x, int y, int width, int height, Color color) {
	SDL_Rect rect = {x, y, width, height};
	Uint32 pixel = GetColor(color);
	SDL_FillRect(surface, &rect, pixel);
}

void SDLSurface::DrawCircle1(int x0, int y0, int radius, Color color) {
 	Uint32 pixel = GetColor(color);
 	
 	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;
 
	DrawPixel(x0, y0 + radius, pixel);
	DrawPixel(x0, y0 - radius, pixel);
	DrawPixel(x0 + radius, y0, pixel);
	DrawPixel(x0 - radius, y0, pixel); 
  	while(x < y) {
		ddF_x = 2*x + 1;
		ddF_y = -2*y;
		f = x*x + y*y - radius*radius + 2*x - y + 1;
		if(f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
    	}
		x++;
		ddF_x += 2;
		f += ddF_x;    
		DrawPixel(x0 + x, y0 + y, pixel);
		DrawPixel(x0 - x, y0 + y, pixel);
		DrawPixel(x0 + x, y0 - y, pixel);
		DrawPixel(x0 - x, y0 - y, pixel);
		DrawPixel(x0 + y, y0 + x, pixel);
		DrawPixel(x0 - y, y0 + x, pixel);
		DrawPixel(x0 + y, y0 - x, pixel);
		DrawPixel(x0 - y, y0 - x, pixel);
	}
}

void SDLSurface::DrawCircle(int x0, int y0, int radius, int thick, Color color) {
	if (thick == 1)
		DrawCircle1(x0, y0, radius, color);
	else {
		int max = thick/2;
		for (int i = 0; i < max; ++i) {
			DrawCircle1(x0, y0, radius + i, color);
			if (Odd(i))
				DrawCircle1(x0, y0, radius - i, color);
		}
	}
}

void SDLSurface::FillCircle(int cx, int cy, int radius, Color color) {
	Uint32 pixel = GetColor(color); 
   	int bpp = surface->format->BytesPerPixel;
    double r = double(radius);
 
    for (double dy = 1; dy <= r; dy += 1.0) {
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        int x = int(cx - dx);
        Uint8 *target_pixel_a = GetPixelPos(x, int(cy + r - dy));
        Uint8 *target_pixel_b = GetPixelPos(x, int(cy - r + dy));;
        for (; x <= cx + dx; x++) {
            memcpy(target_pixel_a, &pixel, bpp);
            memcpy(target_pixel_b, &pixel, bpp);
            target_pixel_a += bpp;
            target_pixel_b += bpp;
        }
    }
}

