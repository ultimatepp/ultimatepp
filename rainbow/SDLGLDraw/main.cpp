#include "SDLGLDraw.h"

#define IMAGECLASS TestImg
#define IMAGEFILE <SDLGLDraw/test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <SDLGLDraw/test.iml>
#include <Draw/iml_source.h>

void SDLGLDraw::Paint(Draw& w)
{
	w.DrawRect(GetSize(), LtGray);
	w.DrawRect(100, 100, 16, 16, Blue());
	w.DrawImage(100, 100, TestImg::test());

	w.DrawRect(140, 100, 16, 16, Blue());
	w.DrawImage(141, 101, TestImg::test(), Rect(1, 1, 15, 15));

	w.DrawRect(200, 100, 16, 16, Blue());
	w.DrawImage(200, 100, TestImg::test(), Cyan());

	w.DrawRect(240, 100, 16, 16, Blue());
	w.DrawImage(241, 101, TestImg::test(), Rect(1, 1, 15, 15), Cyan());
	
	w.DrawText(10, 10, "Text1", StdFont(), Red());
	w.DrawText(10, 50, "Text2", StdFont(), Black());

	w.DrawText(500, 500, 300, "Angled", StdFont(), Black());
}

SDLGLDraw::SDLGLDraw()
{
}

GUI_APP_MAIN
{
	SDLGLDraw().Run();
}
