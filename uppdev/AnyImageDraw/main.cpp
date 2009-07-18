#include "AnyImageDraw.h"

void ImageAnyDrawPainter(Draw *(*f)(Size sz), Image (*e)(Draw *w));
void ImageAnyDrawSystem(Draw *(*f)(Size sz), Image (*e)(Draw *w));

static Draw *sCP(Size sz)
{
	return new ImagePainter(sz);
}

static Image sEP(Draw *w)
{
	ImagePainter *ip = dynamic_cast<ImagePainter *>(w);
	return ip ? (Image)(*ip) : Image();
}

static Draw *sCD(Size sz)
{
	return new ImageDraw(sz);
}

static Image sED(Draw *w)
{
	ImageDraw *ip = dynamic_cast<ImageDraw *>(w);
	return ip ? (Image)(*ip) : Image();
}

INITBLOCK {
	ImageAnyDrawPainter(sCP, sEP);
	ImageAnyDrawSystem(sCD, sED);
}

void AnyImageDraw::Paint(Draw& w)
{
	ImageAnyDraw iw(200, 200);
	iw.DrawRect(0, 0, 200, 200, White());
	iw.DrawText(0, 0, "X", Roman(200));
	w.DrawImage(0, 0, iw);
}

AnyImageDraw::AnyImageDraw()
{
}

GUI_APP_MAIN
{
	AnyImageDraw().Run();
}
