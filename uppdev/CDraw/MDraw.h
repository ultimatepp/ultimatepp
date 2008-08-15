#ifndef _MDraw_MDraw_h
#define _MDraw_MDraw_h

#include <CtrlLib/CtrlLib.h>

#include <emmintrin.h>


class PixelBlock {
	enum {
		RGBX = 1, RGB, RGB565, BGR565, RGB555, BGR555, PALETTE8
	};
	static int   display;

	dword        *buffer;
	dword        *pixel;
	Size          size;

#ifdef PLATFORM_WIN32
	HBITMAP       hbmp;
#endif

	void   Create0(int cx, int cy, bool section = true);

public:
	static void Init();

	Size   GetSize()                 { return size; }

	void   Create(int cx, int cy);
	void   Clear();

	void   Put(Draw& w, int x, int y);

	operator dword*()                { return pixel; }

	int    LineDelta();
	dword *PointAdr(int x, int y);

	void   DrawRect(int x, int y, int cx, int cy, Color c);

	PixelBlock();
	PixelBlock(int cx, int cy, bool section);
	~PixelBlock();
};


class MDraw : public TopWindow {
public:
	int  posy;

	bool gdi;
	int  am;
	int  recti;
	int  chari;
	int  va;
	bool sec;

	BiVector<int> m;
	int    frames;
	double time;

	virtual void Paint(Draw& w);
	virtual void MouseMove(Point p, dword);
	virtual bool Key(dword key, int);
	void ClearRefresh();

	typedef MDraw CLASSNAME;
	MDraw();
};

void DrawRectNOP(PixelBlock& w, int x, int y, int cx, int cy, Color c);
void DrawNOP(PixelBlock& w, int x, int y, Color c);
void DrawRectBASE(PixelBlock& w, int x, int y, int cx, int cy, Color c);
void DrawBASE(PixelBlock& w, int x, int y, Color c);
void DrawBASE2(PixelBlock& w, int x, int y, Color c);
void AADrawBASE(PixelBlock& w, int x, int y, Color c);
void AADrawBASEPF(PixelBlock& w, int x, int y, Color c);

void DrawRectF(PixelBlock& w, int x, int y, int cx, int cy, Color c);
void DrawMMX1(PixelBlock& w, int x, int y, Color color);

void DrawRectS(PixelBlock& w, int x, int y, int cx, int cy, Color c);

void DrawAAP(PixelBlock& w, int x, int y, Color c);
void DrawAAPMMX(PixelBlock& w, int x, int y, Color c);

#endif
