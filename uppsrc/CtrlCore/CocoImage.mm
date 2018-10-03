#include "CocoMM.h"

#ifdef PLATFORM_COCOA

#define LLOG(x)

namespace Upp {

CGImageRef createCGImage(const Image& img)
{
	CGDataProvider *dataProvider = CGDataProviderCreateWithData(NULL, ~img, img.GetLength() * sizeof(RGBA), NULL);
	static CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB(); // TODO: This is probably wrong...
	Upp::Size isz = img.GetSize();
    CGImageRef cg_img = CGImageCreate(isz.cx, isz.cy, 8, 32, isz.cx * sizeof(RGBA),
                                      colorSpace, kCGImageAlphaPremultipliedFirst,
                                      dataProvider, 0, false, kCGRenderingIntentDefault);
	CGDataProviderRelease(dataProvider);
	return cg_img;
}

struct ImageSysData {
	Image            img;
	CGImageRef       cgimg = NULL;
	
	void Init(const Image& img);
	~ImageSysData();
};

void ImageSysData::Init(const Image& m)
{
	img = m;
	cgimg = createCGImage(img);
	SysImageRealized(img);
}

ImageSysData::~ImageSysData()
{
	SysImageReleased(img);
    CGImageRelease(cgimg);
}

struct ImageSysDataMaker : LRUCache<ImageSysData, int64>::Maker {
	Image img;

	virtual int64  Key() const                      { return img.GetSerialId(); }
	virtual int    Make(ImageSysData& object) const { object.Init(img); return img.GetLength(); }
};

static LRUCache<ImageSysData, int64> cg_image_cache;

void SystemDraw::SysDrawImageOp(int x, int y, const Image& img, Color color)
{
	GuiLock __;

	if(img.GetLength() == 0)
		return;

	ImageSysDataMaker m;
	LLOG("SysImage cache pixels " << cache.GetSize() << ", count " << cache.GetCount());
	m.img = IsNull(color) ? img : CachedSetColorKeepAlpha(img, color); // TODO: Can setcolor be optimized out? By masks e.g.?
	ImageSysData& sd = cg_image_cache.Get(m);
	Size isz = img.GetSize();
	CGContextSaveGState(cgHandle);
	Point off = GetOffset();
	CGContextTranslateCTM(cgHandle, x + off.x, y + off.y);
	CGContextScaleCTM(cgHandle, 1.0, -1.0);
	CGContextDrawImage(cgHandle, CGRectMake(0, -isz.cy, isz.cx, isz.cy), sd.cgimg);
    CGContextRestoreGState(cgHandle);
	cg_image_cache.Shrink(4 * 1024 * 768, 1000); // Cache must be after Paint because of PaintOnly!
}

// TODO: https://stackoverflow.com/questions/10733228/native-osx-lion-resize-cursor-for-custom-nswindow-or-nsview

enum {
	MM_Arrow = 1,
	MM_Wait,
	MM_IBeam,
	MM_No,
	MM_SizeAll,
	MM_SizeHorz,
	MM_SizeVert,
	MM_SizeTopLeft,
	MM_SizeTop,
	MM_SizeTopRight,
	MM_SizeLeft,
	MM_SizeRight,
	MM_SizeBottomLeft,
	MM_SizeBottom,
	MM_SizeBottomRight,
	MM_Cross,
	MM_Hand,
};


#define FCURSOR_(x) { static Image h; ONCELOCK { h = CreateImage(Size(1, 1), Black); h.SetAuxData(x); } return h; }

Image Image::Arrow() FCURSOR_(MM_Arrow)
Image Image::Wait() FCURSOR_(MM_Wait)
Image Image::IBeam() FCURSOR_(MM_IBeam)
Image Image::No() FCURSOR_(MM_No)
Image Image::SizeAll() FCURSOR_(MM_SizeAll)
Image Image::SizeHorz() FCURSOR_(MM_SizeHorz)
Image Image::SizeVert() FCURSOR_(MM_SizeVert)
Image Image::SizeTopLeft() FCURSOR_(MM_SizeTopLeft)
Image Image::SizeTop() FCURSOR_(MM_SizeTop)
Image Image::SizeTopRight() FCURSOR_(MM_SizeTopRight)
Image Image::SizeLeft() FCURSOR_(MM_SizeLeft)
Image Image::SizeRight() FCURSOR_(MM_SizeRight)
Image Image::SizeBottomLeft() FCURSOR_(MM_SizeBottomLeft)
Image Image::SizeBottom() FCURSOR_(MM_SizeBottom)
Image Image::SizeBottomRight() FCURSOR_(MM_SizeBottomRight)
Image Image::Cross() FCURSOR_(MM_Cross)
Image Image::Hand() FCURSOR_(MM_Hand)

// TODO: Missing kinds (sizers mostly)
// https://stackoverflow.com/questions/10733228/native-osx-lion-resize-cursor-for-custom-nswindow-or-nsview/21786835#21786835

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundeclared-selector"

#define XCURSOR(id) \
	if([NSCursor respondsToSelector:@selector(id)]) return [NSCursor performSelector:@selector(id)];

NSCursor *GetNSCursor(int kind)
{
	switch(kind) {
	case MM_SizeAll:
		XCURSOR(_moveCursor);
		break;
	case MM_SizeHorz:
		XCURSOR(_windowResizeEastWestCursor);
		break;
	case MM_SizeVert:
		XCURSOR(_windowResizeNorthSouthCursor);
		break;
	case MM_SizeTopLeft:
		XCURSOR(_windowResizeNorthWestSouthEastCursor);
		break;
	case MM_SizeTop:
		XCURSOR(_windowResizeNorthCursor);
		break;
	case MM_SizeTopRight:
		XCURSOR(_windowResizeNorthEastSouthWestCursor);
		break;
	case MM_SizeLeft:
		XCURSOR(_windowResizeWestCursor);
		break;
	case MM_SizeRight:
		XCURSOR(_windowResizeEastCursor);
		break;
	case MM_SizeBottomLeft:
		XCURSOR(_windowResizeNorthEastSouthWestCursor);
		break;
	case MM_SizeBottom:
		XCURSOR(_windowResizeSouthCursor);
		break;
	case MM_SizeBottomRight:
		XCURSOR(_windowResizeNorthWestSouthEastCursor);
		break;
	case MM_IBeam: return [NSCursor IBeamCursor];
	case MM_Cross: return [NSCursor crosshairCursor];
	case MM_Hand:  return [NSCursor pointingHandCursor];
	case MM_No:    return [NSCursor operationNotAllowedCursor];
	}
	return [NSCursor arrowCursor];
}

#pragma clang diagnostic pop

void Ctrl::SetNSAppImage(const Image& img)
{
	ImageSysDataMaker m;
	LLOG("SysImage cache pixels " << cache.GetSize() << ", count " << cache.GetCount());
	m.img = img;
	ImageSysData& sd = cg_image_cache.Get(m);
	static CGImageRef cgimg;
	static NSImage   *nsimg;
	if(sd.cgimg != cgimg) {
		cgimg = sd.cgimg;
		if(nsimg)
			[nsimg release];
		Point p = img.GetHotSpot();
		Size isz = img.GetSize();
		double scale = 1.0 / DPI(1);
		NSSize size;
		size.width = scale * isz.cx;
		size.height = scale * isz.cy;
		NSPoint hot;
		hot.x = scale * p.x;
		hot.y = scale * p.y;
		nsimg = [[NSImage alloc] initWithCGImage:cgimg size:size];
	}
	[NSApp setApplicationIconImage:nsimg];
}

void  Ctrl::SetMouseCursor(const Image& img)
{
	if(GetDragAndDropSource())
		return;
	int64 h = img.GetAuxData();
	if(h) {
		[GetNSCursor(h) set];
		return;
	}
	ImageSysDataMaker m;
	LLOG("SysImage cache pixels " << cache.GetSize() << ", count " << cache.GetCount());
	m.img = img;
	ImageSysData& sd = cg_image_cache.Get(m);
	static CGImageRef cgimg;
	static NSCursor  *cursor;
	if(sd.cgimg != cgimg) {
		cgimg = sd.cgimg;
		if(cursor)
			[cursor release];
		Point p = img.GetHotSpot();
		Size isz = img.GetSize();
		double scale = 1.0 / DPI(1);
		NSSize size;
		size.width = scale * isz.cx;
		size.height = scale * isz.cy;
		NSPoint hot;
		hot.x = scale * p.x;
		hot.y = scale * p.y;
		NSImage *nsimg = [[NSImage alloc] initWithCGImage:cgimg size:size];
		cursor = [[NSCursor alloc] initWithImage:nsimg hotSpot:hot];
		[nsimg release];
	}
	[cursor set];
}

void ImageDraw::Init(int cx, int cy)
{
	ib.Create(cx, cy);

	static CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();

	SystemDraw::Init(CGBitmapContextCreateWithData(~ib, cx, cy, 8, cx * sizeof(RGBA),
	                                               colorSpace, kCGImageAlphaPremultipliedFirst,
	                                               NULL, NULL), NULL);
	CGContextTranslateCTM(cgHandle, 0, cy);
	if(IsUHDMode()) {
		CGContextScaleCTM(cgHandle, 2, -2);
		CGContextTranslateCTM(cgHandle, 0, -cy / 2.0);
	}
	else
		CGContextScaleCTM(cgHandle, 1, -1);
}

ImageDraw::ImageDraw(Size sz)
{
	Init(sz.cx, sz.cy);
}

ImageDraw::ImageDraw(int cx, int cy)
{
	Init(cx, cy);
}

Draw& ImageDraw::Alpha()
{
	if(!alpha)
		alpha.Create(ib.GetSize());
	return *alpha;
}

ImageDraw::~ImageDraw()
{
	CGContextRelease(cgHandle);
	handle = NULL; // avoid releasing invalid handle in ~SystemDraw
}

Image ImageDraw::GetStraight()
{
	if(alpha) {
		RGBA *e = ib.End();
		RGBA *t = ib;
		RGBA *s = alpha->ib;
		while(t < e) {
			t->a = s->r;
			s++;
			t++;
		}
	}
	return Image(ib);
}

ImageDraw::operator Image()
{
	if(alpha)
		return Premultiply(GetStraight());
	return Image(ib);
}

};

#endif
