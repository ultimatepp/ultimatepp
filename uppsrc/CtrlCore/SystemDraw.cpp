#include "CtrlCore.h"

NAMESPACE_UPP

#define LTIMING(x)

void StdDrawImage(SystemDraw& w, int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	LTIMING("DrawImageOp");
	bool tonative = !w.IsNative();
	if(tonative) {
		w.BeginNative();
		w.Native(x, y);
		w.Native(cx, cy);
	}
	Size sz = Size(cx, cy);
	if((cx > 2000 || cy > 1500) && IsNull(color) && w.IsPrinter()) {
		int yy = 0;
		ImageRaster ir(img);
		RescaleImage rm;
		rm.Create(Size(cx, cy), ir, src);
		while(yy < cy) {
			int ccy = min(cy - yy, 16);
			ImageBuffer ib(cx, ccy);
			for(int q = 0; q < ccy; q++)
				rm.Get(ib[q]);
			DrawImageBandRLE(w, x, y + yy, ib, 16);
			yy += ccy;
		}
	}
	else
	if(src.GetSize() == sz)
		img.PaintImage(w, x, y, src, color);
	else {
		Image h = Rescale(img, Size(cx, cy), src);
		h.PaintImage(w, x, y, h.GetSize(), color);
	}
	if(tonative)
		w.EndNative();
}

ImageBuffer::ImageBuffer(ImageDraw& iw)
{
	Image m = iw;
	Set(m);
}

void ImageAnyDrawSystem(Draw *(*f)(Size sz), Image (*e)(Draw *w));

static Draw *sCD(Size sz)
{
	return new ImageDraw(sz);
}

static Image sED(Draw *w)
{
	ImageDraw *ip = dynamic_cast<ImageDraw *>(w);
	return ip ? (Image)(*ip) : Image();
}

void InstallSystemImage()
{
	Image::Data::InitSystemImage(&Image::Data::SysInitImp, &Image::Data::SysReleaseImp,
	                             &Image::Data::GetResCountImp, &Image::Data::PaintImp);
}

INITBLOCK {
	ImageAnyDrawSystem(sCD, sED);
	InstallSystemImage();
}

void SetSurface(SystemDraw& w, int x, int y, int cx, int cy, const RGBA *pixels);
void SetSurface(SystemDraw& w, const Rect& dest, const RGBA *pixels, Size psz, Point poff);

void SetSurface(Draw& w, const Rect& dest, const RGBA *pixels, Size srcsz, Point poff)
{
	SystemDraw *sw = dynamic_cast<SystemDraw *>(&w);
	if(sw && sw->CanSetSurface())
		SetSurface(*sw, dest, pixels, srcsz, poff);
	else {
		ImageBuffer ib(dest.GetWidth(), dest.GetHeight());
		for(int i = 0; i < ib.GetHeight(); i++) {
			int sl = poff.y + i;
			if(i >= 0 && i < srcsz.cy)
				Copy(ib[i], pixels + srcsz.cx * sl + poff.x,
				     minmax(srcsz.cx - poff.x, 0, min(dest.GetWidth(), srcsz.cx)));
		}
		w.DrawImage(dest.left, dest.top, ib);
	}
}

void SetSurface(Draw& w, int x, int y, int cx, int cy, const RGBA *pixels)
{
	SetSurface(w, RectC(x, y, cx, cy), pixels, Size(cx, cy), Point(0, 0));
}


SystemDraw& ScreenInfo();

bool BackDraw::IsPaintingOp(const Rect& r) const
{
	Rect rr = r + GetOffset();
	if(!rr.Intersects(size))
		return false;
	return painting ? painting->IsPainting(rr + painting_offset) : true;
}

BackDraw::BackDraw()
{
	painting = NULL;
	painting_offset = Point(0, 0);
}

BackDraw::~BackDraw()
{
	Destroy();
}

END_UPP_NAMESPACE
