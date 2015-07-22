#ifndef _SDLCtrl_h_
#define _SDLCtrl_h_

#include "SDLWrapper.h"

inline bool Odd_(int val)	  	{return val%2;}

class SDLSurface {
public:
	SDLSurface();
	SDLSurface(int width, int height, int bpp = 0); 
	SDLSurface(SDL_Surface *_surface, bool _del = false);
	~SDLSurface();
	bool Resize(int width, int height);
	bool LoadBMP(const char *filename);
	bool CreateRGB(int width, int height, int bpp);
	void Lock();
	void Unlock();
	bool Blit(SDLSurface &surf, Rect source = Null, Rect dest = Null);
	void DrawImage(Image img, int x, int y, Color transparent);
	void DrawLine1(int x1, int y1, int x2, int y2, Color color);
	void DrawLine(int x1, int y1, int x2, int y2, int thick, Color color);
	void DrawRect(int x, int y, int width, int height, int thick, Color color);
	void FillRect(int x, int y, int width, int height, Color color);
	void DrawCircle1(int x, int y, int radius, Color color);
	void DrawCircle(int x, int y, int radius, int thick, Color color);
	void FillCircle(int x, int y, int radius, Color color);
	void DrawPixel(int x, int y, Color color);
	inline void DrawPixel(int x, int y, Uint32 scolor) {
		byte *pixelpos = GetPixelPos(x, y);
		if (!pixelpos)
			return;
		if (surface->format->BytesPerPixel == 1)
			*pixelpos = (byte)scolor;
		else
			memcpy(pixelpos, &scolor, surface->format->BytesPerPixel);
	}
	Color GetPixel(int x, int y);
	byte GetPixelByte(int x, int y);
	inline byte *GetPixelPos(int x, int y) {
		if (x < 0 || y < 0|| x >= surface->w || y >= surface->h)
			return 0;
		return (byte *)surface->pixels + surface->pitch*y + surface->format->BytesPerPixel*x;
	}
	Uint32 GetColor(Color color);
	int GetBpp()				{return surface->format->BitsPerPixel;};	
	int GetWidth()				{return surface->w;};
	int GetHeight()				{return surface->h;};
	const char *GetError() 		{return SDL_GetError();};
	bool IsReady()				{return surface;};
	SDL_Surface *GetSurface() 	{return surface;};
	
protected:
	SDL_Surface *surface;
	bool delIn;
};

class SDLCtrl : public Ctrl {
typedef SDLCtrl CLASSNAME;	
	class SDLCtrlIn : public DHCtrl, public SDLSurface {
	typedef SDLCtrlIn CLASSNAME;	
	public:
		SDLCtrlIn();
		~SDLCtrlIn();
		
		virtual void Layout();	
		//virtual void Paint(Draw& w);		
		bool CreateScreen();
			
	#ifdef PLATFORM_POSIX
		XDisplay *display;
	#else
		HWND hwndSDL;
	#endif
		int videoflags;
		int bpp;
	
		bool init;
			
	#ifdef PLATFORM_WIN32
		virtual void State(int reason);
	#endif
		SDL_Cursor *cursor;
		String strError;
		String GetError() 				{return strError;};
		void SetError(String str);
		void ResetError()				{strError = "";};
	};
	SDLCtrlIn ctrlIn;
	
public:
	SDLCtrl() {
		Add(ctrlIn.SizePos());
	}
	SDLCtrl &SetBpp(int bpp) 		{ctrlIn.bpp = bpp; return *this;};
	int GetBpp() 					{return ctrlIn.GetBpp();};	
	SDLCtrl &SetVideoFlags(int vf) 	{ctrlIn.videoflags = vf; return *this;};
	int GetVideoFlags()				{return ctrlIn.videoflags;};
	bool IsReady()					{return ctrlIn.IsReady();};
	int GetWidth()					{return ctrlIn.GetWidth();};
	int GetHeight()					{return ctrlIn.GetHeight();};	
	String GetError() 				{return ctrlIn.GetError();};
	void SetError(String str)		{ctrlIn.SetError(str);};
	void ResetError()				{ctrlIn.ResetError();};
	SDL_Surface *GetSurface() 		{return ctrlIn.GetSurface();};
	bool Blit(SDLSurface &surf, Rect source = Null, Rect dest = Null) {
		return ctrlIn.Blit(surf, source, dest);};
};

#endif



