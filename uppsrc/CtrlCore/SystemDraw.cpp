#include "SystemDraw.h"

NAMESPACE_UPP

#define LTIMING(x)

void SystemDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	GuiLock __;
	LTIMING("DrawImageOp");
	bool tonative = !IsNative();
	if(tonative) {
		BeginNative();
		Native(x, y);
		Native(cx, cy);
	}
	Size sz = Size(cx, cy);
	if((cx > 2000 || cy > 1500) && IsNull(color) && IsPrinter()) {
		int yy = 0;
		ImageRaster ir(img);
		RescaleImage rm;
		rm.Create(Size(cx, cy), ir, src);
		while(yy < cy) {
			int ccy = min(cy - yy, 16);
			ImageBuffer ib(cx, ccy);
			for(int q = 0; q < ccy; q++)
				rm.Get(ib[q]);
			DrawImageBandRLE(*this, x, y + yy, ib, 16);
			yy += ccy;
		}
	}
	else
	if(src.GetSize() == sz)
		img.PaintImage(*this, x, y, src, color);
	else {
		Image h = Rescale(img, Size(cx, cy), src);
		h.PaintImage(*this, x, y, h.GetSize(), color);
	}
	if(tonative)
		EndNative();
}


Draw& ImageDraw::Alpha()
{
	if(!has_alpha) {
		alpha.DrawRect(size, GrayColor(0));
		has_alpha = true;
	}
	return alpha;
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

END_UPP_NAMESPACE
