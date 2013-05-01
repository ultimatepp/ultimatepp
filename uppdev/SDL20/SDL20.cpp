#include <Core/Core.h>
#include <Draw/Draw.h>

using namespace Upp;

#include "SDL.h"

#define IMAGECLASS TestImg
#define IMAGEFILE <SDL20/test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <SDL20/test.iml>
#include <Draw/iml_source.h>

SDL_Texture *TextureFromImage(SDL_Renderer *renderer, const Image& m)
{
	Size isz = m.GetSize();
    void *pixels;
    int   pitch;
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
	                                         SDL_TEXTUREACCESS_STREAMING, isz.cx, isz.cy);
	if(SDL_LockTexture(texture, NULL, &pixels, &pitch) >= 0) {
		const RGBA *s = ~m;
		for(int y = 0; y < isz.cy; y++) {
			Copy((RGBA *)((Uint8*)pixels + y * pitch), s, isz.cx);
			s += isz.cx;
		}
		SDL_UnlockTexture(texture);
		SDL_SetTextureColorMod(texture, 200, 100, 50);
		return texture;
    }

	SDL_DestroyTexture(texture);
    return NULL;
}

void DrawImage(SDL_Renderer *renderer, int x, int y, const Image& m)
{
	Size isz = m.GetSize();
	SDL_Texture *tex = TextureFromImage(renderer, m);
	SDL_Rect t;
	t.x = x;
	t.y = y;
	t.w = isz.cx;
	t.h = isz.cy;
	SDL_RenderCopy(renderer, tex, NULL, &t);
	SDL_DestroyTexture(tex);
}

Image DownSample3x(const Image& src)
{
	Size tsz = src.GetSize() / 3;
	ImageBuffer ib(tsz);
	int w = src.GetSize().cx;
	int w2 = 2 * w;
	for(int y = 0; y < tsz.cy; y++) {
		RGBA *t = ib[y];
		RGBA *e = t + tsz.cx;
		const RGBA *s = src[3 * y];
		while(t < e) {
			int r, g, b, a;
			const RGBA *q;
			r = g = b = a = 0;
#define S__SUM(delta) q = s + delta; r += q->r; g += q->g; b += q->b; a += q->a;
			S__SUM(0) S__SUM(1) S__SUM(2)
			S__SUM(w + 0) S__SUM(w + 1) S__SUM(w + 2)
			S__SUM(w2 + 0) S__SUM(w2 + 1) S__SUM(w2 + 2)
#undef  S__SUM
			t->a = a / 9;
			t->r = r / 9;
			t->g = g / 9;
			t->b = b / 9;
			t++;
			s += 3;
		}
	}
	return ib;
}

int main(int argc, char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 1600, 900, SDL_WINDOW_SHOWN);
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	int i = 0;
	for(;;) {
		SDL_RenderClear(ren);
		DrawImage(ren, 300, 600, DownSample3x(UPP::Rotate(Magnify(TestImg::pinkie(), 3, 3), 500 * i)));
		DrawImage(ren, 100, 800, DownSample3x(UPP::Rotate(Magnify(TestImg::pinkie(), 3, 3), 140 * i)));
		DrawImage(ren, 800, 600, DownSample3x(UPP::Rotate(Magnify(TestImg::pinkie(), 3, 3), 70 * i)));
		DrawImage(ren, 40, 60, DownSample3x(UPP::Rotate(Magnify(TestImg::pinkie(), 3, 3), 60 * i)));
		i++;
		SDL_RenderPresent(ren);
	
		SDL_Delay(20);
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	SDL_Quit();
 	return 0;
}
