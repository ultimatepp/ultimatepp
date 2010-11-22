#ifndef _SDLCtrl_h_
#define _SDLCtrl_h_

#include <SDL/SDLWrapper.h>

inline bool Odd_(int val)	  	{return val%2;}

class SDLSurface {
public:
	SDLSurface();
	SDLSurface(int width, int height, int bpp = 0); 
	SDLSurface(SDL_Surface *_surface, bool _del = false);
	~SDLSurface();
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
	void DrawPixel(int x, int y, Uint32 scolor);
	Color GetPixel(int x, int y);
	byte *GetPixelPos(int x, int y);
	Uint32 GetColor(Color color);
	int GetBpp()			{return surface->format->BitsPerPixel;};	
	int GetWidth()			{return surface->w;};
	int GetHeight()			{return surface->h;};
	const char *GetError() 	{return SDL_GetError();};
	
protected:
	SDL_Surface *surface;
	bool del;
};

class SDLCtrl : public DHCtrl, public SDLSurface {
typedef SDLCtrl CLASSNAME;	
public:
	SDLCtrl();
	~SDLCtrl();

	SDLCtrl &SetBpp(int _bpp) {bpp = _bpp; return *this;};
	SDLCtrl &SetVideoflags(int _vf) 	{videoflags = _vf; return *this;};
	String GetError() 					{
		const char *err = SDLSurface::GetError();
		if (*err)
			return err;
		else
			return strError;};
	void SetError(String str);
	void ResetError() 					{strError = "";};
	bool IsReady()						{return surface;};
	int GetWidth()						{return surface->w;};
	int GetHeight()						{return surface->h;};
	
protected:
	virtual void Layout();	
	//virtual void Paint(Draw& w);		
	bool CreateScreen();
		
#ifdef PLATFORM_POSIX
	XDisplay *display;
#else
	long hwndSDL;
#endif
	int videoflags;
	int bpp;

	bool init;
		
private:
#ifdef PLATFORM_WIN32
	virtual void State(int reason);
#endif
	SDL_Cursor *cursor;
	String strError;
};

#endif



