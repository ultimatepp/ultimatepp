#ifndef _PaintGL_PaintGL_h
#define _PaintGL_PaintGL_h

#include <Draw/Draw.h>

#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>

class PaintGL {
	Size    size;
	HBITMAP hbmp, ohbmp;
	HDC     hdc;
	HGLRC   hrc;

	void Free();
	void Init(Size sz);

public:
	void Paint(Draw& w, const Rect& r, Callback gl);

	PaintGL();
	~PaintGL();
};


#endif
