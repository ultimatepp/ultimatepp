#ifndef _MDraw_MDraw_h
#define _MDraw_MDraw_h

#include <CtrlLib/CtrlLib.h>

class PixelBlock {
	enum {
		RGBX = 1, RGB, RGB565, BGR565, RGB555, BGR555, PALETTE8
	};
	static int   display;

	Buffer<dword> buffer;
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
	int pos;

	virtual void Paint(Draw& w);

	typedef MDraw CLASSNAME;
	MDraw() { pos = 0; }
};

#endif
