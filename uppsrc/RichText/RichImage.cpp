#include "RichText.h"
#include <Painter/Painter.h>

namespace Upp {

#ifdef NEWIMAGE

struct RichImage : public RichObjectType {
	String GetTypeName(const Value& v) const override;
	Size   GetPhysicalSize(const Value& data) const override;
	Size   GetPixelSize(const Value& data) const override;
	void   Paint(const Value& data, Draw& w, Size sz, const RichObjectPaintInfo& pi) const override;
	Image  ToImage(int64, const Value& data, Size sz, const RichObjectPaintInfo& pi) const override;

	bool   Accept(PasteClip& clip) override;
	Value  Read(PasteClip& clip) override;
	String GetClipFmts() const override;
	String GetClip(const Value& data, const String& fmt) const override;

	typedef RichImage CLASSNAME;
};

String RichImage::GetTypeName(const Value& v) const
{
	return "image";
}

// following function pointers are set in CtrlCore (or similar host platform interface package)
static String      (*sGetImageClip)(const Image& img, const String& fmt);
static bool        (*sAcceptImage)(PasteClip& clip);
static Image       (*sGetImage)(PasteClip& clip);
static const char *(*sClipFmtsImage)();

void InitRichImage(String      (*fGetImageClip)(const Image& img, const String& fmt),
                   bool        (*fAcceptImage)(PasteClip& clip),
                   Image       (*fGetImage)(PasteClip& clip),
                   const char *(*fClipFmtsImage)())
{
	sGetImageClip = fGetImageClip;
	sAcceptImage = fAcceptImage;
	sGetImage = fGetImage;
	sClipFmtsImage = fClipFmtsImage;
}

bool RichImage::Accept(PasteClip& clip)
{
	return sAcceptImage ? sAcceptImage(clip) : false;
}

Value RichImage::Read(PasteClip& clip)
{
	if(sGetImage)
		return StoreImageAsString(sGetImage(clip));
	return Null;
}

String RichImage::GetClipFmts() const
{
	if(sClipFmtsImage)
		return sClipFmtsImage();
	return Null;
}

String RichImage::GetClip(const Value& data, const String& fmt) const
{
	if(sGetImageClip)
		return sGetImageClip(LoadImageFromString(data), fmt);
	return Null;
}

Size   RichImage::GetPixelSize(const Value& data) const
{
	return GetImageStringSize(data);
}

Size   RichImage::GetPhysicalSize(const Value& data) const
{
	Size sz = GetImageStringDots(data);
	if(sz.cx == 0 || sz.cy == 0)
		sz = 600 * GetPixelSize(data) / 96;
	return sz;
}

void   RichImage::Paint(const Value& data, Draw& w, Size sz, const RichObjectPaintInfo&) const
{
	Image x = LoadImageFromString(data);
	w.DrawImage(0, 0, sz.cx, sz.cy, x);
}

Image  RichImage::ToImage(int64, const Value& data, Size sz, const RichObjectPaintInfo&) const
{
	return Rescale(LoadImageFromString(data), sz);
}

INITBLOCK {
	RichObject::Register("image", &Single<RichImage>());
};

RichObject CreateImageObject(const Image& img, int cx, int cy)
{
	RichObject o = RichObject("image", StoreImageAsString(img));
	if(cx || cy)
		o.SetSize(GetRatioSize(o.GetPixelSize(), cx, cy));
	return o;
}

struct RichPNG : public RichObjectType {
	String GetTypeName(const Value& v) const override;
	Value  Read(const String& s) const override;
	String Write(const Value& v) const override;
	Size   GetPhysicalSize(const Value& data) const override;
	Size   GetPixelSize(const Value& data) const override;
	void   Paint(const Value& data, Draw& w, Size sz, const RichObjectPaintInfo& pi) const override;
	Image  ToImage(int64, const Value& data, Size sz, const RichObjectPaintInfo& pi) const override;
};

String RichPNG::GetTypeName(const Value& v) const
{
	return IsString(v) ? "PNG" : "image";
}

Value RichPNG::Read(const String& s) const
{
	Image img = StreamRaster::LoadStringAny(s);
	if(img)
		return img;
	return s;
}

String RichPNG::Write(const Value& v) const
{
	if(IsString(v))
		return v;
	return StoreImageAsString(v);
}

Size RichPNG::GetPhysicalSize(const Value& data) const
{
	if(IsString(data))
		return Size(0, 0);
	return Image(data).GetDots();
}

Size RichPNG::GetPixelSize(const Value& data) const
{
	if(IsString(data))
		return Size(0, 0);
	return Image(data).GetDots();
}

void RichPNG::Paint(const Value& data, Draw& w, Size sz, const RichObjectPaintInfo& ) const
{
	if(IsString(data)) {
		w.DrawRect(sz, SColorFace());
		DrawFrame(w, sz, SColorText());
		w.DrawText(2, 2, "plugin/png missing!");
		return;
	}
	Image x = Image(data);
	Size outsz(min(sz.cx, 4 * x.GetWidth()), min(sz.cy, 4 * x.GetHeight()));
	w.DrawImage(0, 0, outsz.cx, outsz.cy, x);
}

Image RichPNG::ToImage(int64, const Value& data, Size sz, const RichObjectPaintInfo& pi) const
{
	if(IsString(data)) {
		ImageAnyDraw iw(sz);
		Paint(data, iw, sz, pi);
		return iw;
	}
	Image x = Image(data);
	Size outsz(min(sz.cx, 4 * x.GetWidth()), min(sz.cy, 4 * x.GetHeight()));
	return Rescale(x, outsz);
}

INITBLOCK {
	RichObject::Register("PNG", &Single<RichPNG>());
};

struct RichRawImage : public RichObjectType {
	String GetTypeName(const Value& v) const override;
	Value  Read(const String& s) const override;
	String Write(const Value& v) const override;
	Size   GetPhysicalSize(const Value& data) const override;
	Size   GetPixelSize(const Value& data) const override;
	void   Paint(const Value& data, Draw& w, Size sz, const RichObjectPaintInfo& pi) const override;
	Image  ToImage(int64, const Value& data, Size sz, const RichObjectPaintInfo& pi) const override;
};

String RichRawImage::GetTypeName(const Value& v) const
{
	return "rawimage";
}

Value RichRawImage::Read(const String& s) const
{
	return s;
}

String RichRawImage::Write(const Value& v) const
{
	return v;
}

Size RichRawImage::GetPhysicalSize(const Value& data) const
{
	String s = data;
	StringStream ss(s);
	One<StreamRaster> r = StreamRaster::OpenAny(ss);
	if(r)
		return r->GetInfo().dots;
	else
	if(IsString(data) && IsSVG(~data)) {
		Rectf f = GetSVGBoundingBox(~data);
		Zoom z = GetRichTextStdScreenZoom();
		return z.d * (Size)f.GetSize() / z.m;
	}
	return Size(0, 0);
}

Size RichRawImage::GetPixelSize(const Value& data) const
{
	String s = data;
	StringStream ss(s);
	One<StreamRaster> r = StreamRaster::OpenAny(ss);
	if(r)
		return r->GetSize();
	else
	if(IsString(data) && IsSVG(~data)) {
		Rectf f = GetSVGBoundingBox(~data);
		return (Size)f.GetSize();
	}
	return Size(0, 0);
}

void RichRawImage::Paint(const Value& data, Draw& w, Size sz, const RichObjectPaintInfo& pi) const
{
	String s = data;
	StringStream ss(s);
	One<StreamRaster> r = StreamRaster::OpenAny(ss);
	if(r) {
		Size isz = r->GetSize();
		if(GetIsJPGFn() && GetIsJPGFn()(~r) && GetPdfDrawJPEGFn())
			GetPdfDrawJPEGFn()(w, 0, 0, sz.cx, sz.cy, data); // make sure we export jpg to pdf without reprocessing
		else
		if(isz.cx * isz.cy > sz.cx * sz.cy) { // conserve memory by scaling down from source
			ImageEncoder m;
			Rescale(m, sz, *r, isz);
			w.DrawImage(0, 0, sz.cx, sz.cy, m);
		}
		else
			w.DrawImage(0, 0, sz.cx, sz.cy, r->GetImage()); // scale up by Draw to give e.g. PDF chance to store unscaled
	}
	else
	if(IsSVG(s))
		w.DrawImage(0, 0, RenderSVGImage(sz, s, pi.ink));
}

Image RichRawImage::ToImage(int64 serial_id, const Value& data, Size sz, const RichObjectPaintInfo& pi) const
{
	String s = data;
	StringStream ss(s);
	One<StreamRaster> r = StreamRaster::OpenAny(ss);
	if(r) {
		struct ImgFormatLoader : ImageMaker {
			int64  serial_id;
			String s;

			virtual String Key() const  { String x; RawCat(x, serial_id); return x; }
			virtual Image  Make() const {
				StringStream ss(s);
				One<StreamRaster> r = StreamRaster::OpenAny(ss);
				return r ? r->GetImage() : Image();
			}
		} loader;
		loader.serial_id = serial_id;
		loader.s = s;
		return Rescale(MakeImage(loader), sz);
	}
	else
	if(IsString(data) && IsSVG(~data))
		return RenderSVGImage(sz, ~data, pi.ink);
	return Null;
}

INITBLOCK {
	RichObject::Register("rawimage", &Single<RichRawImage>());
};

RichObject CreateRawImageObject(const String& s, int cx, int cy)
{
	RichObject o = RichObject("rawimage", s);
	o.InitSize(cx, cy);
	return o;
}

struct RichImlImage : public RichObjectType {
	String GetTypeName(const Value& v) const override;
	Size   GetPhysicalSize(const Value& data) const override;
	Size   GetPixelSize(const Value& data) const override;
	void   Paint(const Value& data, Draw& w, Size sz, const RichObjectPaintInfo& pi) const override;
	Image  ToImage(int64, const Value& data, Size sz, const RichObjectPaintInfo& pi) const override;
	bool   IsText() const override;
	
	Image Get(const Value& v) const;
};

Image RichImlImage::Get(const Value& v) const
{
	return GetImlImage((String)v);
}

String RichImlImage::GetTypeName(const Value& v) const
{
	return "iml";
}

bool RichImlImage::IsText() const
{
	return true;
}

Size RichImlImage::GetPhysicalSize(const Value& data) const
{
	return Get(data).GetSize();
}

Size RichImlImage::GetPixelSize(const Value& data) const
{
	return Get(data).GetSize();
}

void RichImlImage::Paint(const Value& data, Draw& w, Size sz, const RichObjectPaintInfo&) const
{
	w.DrawImage(0, 0, sz.cx, sz.cy, Get(data));
}

Image RichImlImage::ToImage(int64, const Value& data, Size sz, const RichObjectPaintInfo&) const
{
	return Rescale(Get(data), sz);
}

INITBLOCK {
	RichObject::Register("iml", &Single<RichImlImage>());
};

#endif

INITIALIZER(RichImage) {}

}
