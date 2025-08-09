#include "CtrlCore.h"

#ifdef GUI_WIN

#include <shellapi.h>

namespace Upp {

#define LTIMING(x)  // RTIMING(x)
#define LLOG(x)     // DLOG(x)

bool ImageFallBack
// = true
;

class BitmapInfo32__ {
	Buffer<byte> data;

public:
	operator BITMAPINFO *()        { return (BITMAPINFO *)~data; }
	operator BITMAPINFOHEADER *()  { return (BITMAPINFOHEADER *)~data; }
	BITMAPINFOHEADER *operator->() { return (BITMAPINFOHEADER *)~data; }

	BitmapInfo32__(int cx, int cy);
};

BitmapInfo32__::BitmapInfo32__(int cx, int cy)
{
	data.Alloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256);
	BITMAPINFOHEADER *hi = (BITMAPINFOHEADER *) ~data;;
	memset8(hi, 0, sizeof(BITMAPINFOHEADER));
	hi->biSize = sizeof(BITMAPINFOHEADER);
	hi->biPlanes = 1;
#ifdef PLATFORM_WINCE
	hi->biBitCount = 32;
	hi->biCompression = BI_BITFIELDS;
	dword *x = (dword *)(~data + sizeof(BITMAPINFOHEADER));
	x[2] = 0xff;
	x[1] = 0xff00;
	x[0] = 0xff0000;
#else
	hi->biBitCount = 32;
	hi->biCompression = BI_RGB;
#endif
	hi->biSizeImage = 0;
	hi->biClrUsed = 0;
	hi->biClrImportant = 0;
	hi->biWidth = cx;
	hi->biHeight = -cy;
}

HBITMAP CreateBitMask(const RGBA *data, Size sz, Size tsz, Size csz, RGBA *ct)
{
	LTIMING("CreateBitMask");
	GuiLock __;
	Fill(ct, RGBAZero(), tsz.cx * tsz.cy);
	int linelen = (tsz.cx + 15) >> 4 << 1;
	Buffer<byte>  mask(tsz.cy * linelen, 0xff);
	byte *m = mask;
	RGBA *ty = ct;
	const RGBA *sy = data;
	for(int y = 0; y < csz.cy; y++) {
		const RGBA *s = sy;
		RGBA *t = ty;
		for(int x = 0; x < csz.cx; x++) {
			if(s->a > 128) {
				*t = *s;
				m[x >> 3] &= ~(0x80 >> (x & 7));
			}
			else
				t->r = t->g = t->b = 0;
			t->a = 0;
			t++;
			s++;
		}
		m += linelen;
		sy += sz.cx;
		ty += tsz.cx;
	}
	return ::CreateBitmap(tsz.cx, tsz.cy, 1, 1, mask);
}

void SetSurface(HDC dc, const Rect& dest, const RGBA *pixels, Size srcsz, Point srcoff)
{
	LTIMING("SetSurface");
	GuiLock __;
	BitmapInfo32__ bi(srcsz.cx, srcsz.cy);
	::SetDIBitsToDevice(dc, dest.left, dest.top, dest.GetWidth(), dest.GetHeight(),
	                    srcoff.x, -srcoff.y - dest.Height() + srcsz.cy, 0, srcsz.cy, pixels, bi,
	                    DIB_RGB_COLORS);
}

void SetSurface(HDC dc, int x, int y, int cx, int cy, const RGBA *pixels)
{
	SetSurface(dc, RectC(x, y, cx, cy), pixels, Size(cx, cy), Point(0, 0));
}

void SetSurface(SystemDraw& w, int x, int y, int cx, int cy, const RGBA *pixels)
{
	SetSurface(w.GetHandle(), x, y, cx, cy, pixels);
}

void SetSurface(SystemDraw& w, const Rect& dest, const RGBA *pixels, Size psz, Point poff)
{
	SetSurface(w.GetHandle(), dest, pixels, psz, poff);
}

class DrawSurface : NoCopy {
	int          x, y;
	Size         size;
	RGBA        *pixels;
	HDC          dc, dcMem;
	HBITMAP      hbmp, hbmpOld;

	void  Init(SystemDraw& w, int x, int y, int cx, int cy);
	RGBA* Line(int i) const { ASSERT(i >= 0 && i < size.cy); return (RGBA *)pixels + size.cx * (size.cy - i - 1); }

public:
	operator RGBA *()                   { return pixels; }
	Size        GetSize() const         { return size; }
	RGBA       *operator[](int i)       { return Line(i); }
	const RGBA *operator[](int i) const { return Line(i); }
	int         GetLineDelta() const    { return -size.cx; }

	DrawSurface(SystemDraw& w, const Rect& r);
	DrawSurface(SystemDraw& w, int x, int y, int cx, int cy);
	~DrawSurface();
};

void DrawSurface::Init(SystemDraw& w, int _x, int _y, int cx, int cy)
{
	LTIMING("DrawSurface");
	GuiLock __;
	dc = w.GetHandle();
	size = Size(cx, cy);
	x = _x;
	y = _y;
	dcMem = ::CreateCompatibleDC(dc);
	BitmapInfo32__ bi(cx, cy);
	hbmp = CreateDIBSection(dc, bi, DIB_RGB_COLORS, (void **)&pixels, NULL, 0);
	hbmpOld = (HBITMAP) ::SelectObject(dcMem, hbmp);
	::BitBlt(dcMem, 0, 0, cx, cy, dc, x, y, SRCCOPY);
}

DrawSurface::DrawSurface(SystemDraw& w, const Rect& r)
{
	Init(w, r.left, r.top, r.Width(), r.Height());
}

DrawSurface::DrawSurface(SystemDraw& w, int x, int y, int cx, int cy)
{
	Init(w, x, y, cx, cy);
}

DrawSurface::~DrawSurface()
{
	GuiLock __;
	::BitBlt(dc, x, y, size.cx, size.cy, dcMem, 0, 0, SRCCOPY);
	::DeleteObject(::SelectObject(dcMem, hbmpOld));
	::DeleteDC(dcMem);
}

#ifndef PLATFORM_WINCE
typedef BOOL (WINAPI *tAlphaBlend)(HDC hdcDest, int nXOriginDest, int nYOriginDest,
                                   int nWidthDest, int nHeightDest,
                                   HDC hdcSrc, int nXOriginSrc, int nYOriginSrc,
                                   int nWidthSrc, int nHeightSrc, BLENDFUNCTION blendFunction);

static tAlphaBlend fnAlphaBlend()
{
	GuiLock __;
	static tAlphaBlend pSet;
	static bool inited = false;
	if(!inited) {
		inited = true;
		if(HMODULE hDLL = LoadLibrary("msimg32.dll"))
			pSet = (tAlphaBlend) GetProcAddress(hDLL, "AlphaBlend");
	}
	return pSet;
}
#endif

struct ImageSysData {
	Image       img;
	HBITMAP     hbmp;
	HBITMAP     hmask;
	HBITMAP     himg;
	RGBA       *section;
	
	void Init(const Image& img);
	void CreateHBMP(HDC dc, const RGBA *data);
	void Paint(SystemDraw& w, int x, int y, const Rect& src, Color c);
	
	~ImageSysData();
};

void ImageSysData::Init(const Image& _img)
{
	img = _img;
	hbmp = hmask = himg = NULL;
	LLOG("ImageSysData::Init " << img.GetSerialId() << " " << img.GetSize());
}

ImageSysData::~ImageSysData()
{
	SysImageReleased(img);
	if(hbmp) {
		GuiLock __;
		DeleteObject(hbmp);
	}
	if(hmask) {
		GuiLock __;
		DeleteObject(hmask);
	}
	if(himg) {
		GuiLock __;
		DeleteObject(himg);
	}
	hbmp = hmask = himg = NULL;
	img.Clear();
}

void ImageSysData::CreateHBMP(HDC dc, const RGBA *data)
{
	LTIMING("CreateHBMP");
	LLOG("Creating BMP for " << img.GetSerialId() << ' ' << img.GetSize());
	Size sz = img.GetSize();
	BitmapInfo32__ bi(sz.cx, sz.cy);
	HDC dcMem = ::CreateCompatibleDC(dc);
	RGBA *pixels;
	HBITMAP hbmp32 = CreateDIBSection(dcMem, bi, DIB_RGB_COLORS, (void **)&pixels, NULL, 0);
	HDC hbmpOld = (HDC) ::SelectObject(dcMem, hbmp32);
	Copy(pixels, data, (int)img.GetLength());
	HDC dcMem2 = ::CreateCompatibleDC(dc);
	hbmp = ::CreateCompatibleBitmap(dc, sz.cx, sz.cy);
	HBITMAP o2 = (HBITMAP)::SelectObject(dcMem2, hbmp);
	::BitBlt(dcMem2, 0, 0, sz.cx, sz.cy, dcMem, 0, 0, SRCCOPY);
	::SelectObject(dcMem2, o2);
	::DeleteDC(dcMem2);
	::SelectObject(dcMem, hbmpOld);
	::DeleteObject(hbmp32);
	::DeleteDC(dcMem);
}

void ImageSysData::Paint(SystemDraw& w, int x, int y, const Rect& src, Color c)
{
	HDC dc = w.GetHandle();
	Size sz = img.GetSize();
	int  len = sz.cx * sz.cy;
	Rect sr = src & sz;
	Size ssz = sr.Size();
	if(sr.IsEmpty())
		return;

	if(fnAlphaBlend() && IsNull(c) && !ImageFallBack &&
	   !(w.IsPrinter() && (GetDeviceCaps(dc, SHADEBLENDCAPS) & (SB_PIXEL_ALPHA|SB_PREMULT_ALPHA)) !=
	                     (SB_PIXEL_ALPHA|SB_PREMULT_ALPHA))) {
		if(!himg) {
			LTIMING("Image Alpha create");
			BitmapInfo32__ bi(sz.cx, sz.cy);
			himg = CreateDIBSection(ScreenHDC(), bi, DIB_RGB_COLORS, (void **)&section, NULL, 0);
			if(!himg) return; // Return on release.
			Copy(section, ~img, (int)img.GetLength());
		}
		LTIMING("Image Alpha blit");
		BLENDFUNCTION bf;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 255;
		bf.AlphaFormat = AC_SRC_ALPHA;
		HDC dcMem = w.GetCompatibleDC();
		HBITMAP o = (HBITMAP)::SelectObject(dcMem, himg);
		fnAlphaBlend()(dc, x, y, ssz.cx, ssz.cy, dcMem, sr.left, sr.top, ssz.cx, ssz.cy, bf);
		::SelectObject(dcMem, o);
		SysImageRealized(img);
	}
	else {
		LTIMING("Image Alpha sw");
		DrawSurface sf(w, x, y, ssz.cx, ssz.cy);
		RGBA *t = sf;
		if(w.IsPrinter()) // We have got here because printer does not support alpha blending
			Fill(t, White(), ssz.cx * ssz.cy);
		for(int i = sr.top; i < sr.bottom; i++) {
			if(IsNull(c))
				AlphaBlendOpaque(t, img[i] + sr.left, ssz.cx);
			else
				AlphaBlendOpaque(t, img[i] + sr.left, ssz.cx, c);
			t += ssz.cx;
		}
	}
}

struct ImageSysDataMaker : LRUCache<ImageSysData, int64>::Maker {
	Image img;

	virtual int64  Key() const                      { return img.GetSerialId(); }
	virtual int    Make(ImageSysData& object) const { object.Init(img); return (int)img.GetLength(); }
};

void SystemDraw::SysDrawImageOp(int x, int y, const Image& img, const Rect& src, Color color)
{
	LLOG("SysDrawImageOp " << img.GetSerialId() << ' ' << img.GetSize());

	GuiLock __;
	if(img.GetLength() == 0)
		return;

	Size sz = img.GetSize();
	int kind = img.GetKindNoScan();
	if(kind == IMAGE_OPAQUE && !IsNull(color)) {
		DrawRect(x, y, sz.cx, sz.cy, color);
		return;
	}
	if(kind == IMAGE_OPAQUE && (!IsPrinter() || (GetDeviceCaps(GetHandle(), RASTERCAPS) & RC_DIBTODEV)) && src == img.GetSize()) {
		LTIMING("Image Opaque direct set");
		SetSurface(*this, x, y, sz.cx, sz.cy, ~img);
		return;
	}

	ImageSysDataMaker m;
	static LRUCache<ImageSysData, int64> cache;
	static int Rsz;
	Rsz += (int)img.GetLength();
	if(Rsz > 200 * 200) { // we do not want to do this for each small image painted...
		Rsz = 0;
		cache.Remove([](const ImageSysData& object) { return object.img.GetRefCount() == 1; });
	}
	LLOG("SysImage cache pixels " << cache.GetSize() << ", count " << cache.GetCount());
	sz = Ctrl::GetVirtualScreenArea().GetSize();
	m.img = IsPrinter() && GetDeviceCaps(GetHandle(), NUMCOLORS) == 2 ? Dither(img, 360) : img; // If printer does not support color, dither
	cache.Get(m).Paint(*this, x, y, src, color);
	cache.Shrink(4 * sz.cx * sz.cy, 1000);
}

void ImageDraw::Section::Init(int cx, int cy)
{
	GuiLock __;
	dc = ::CreateCompatibleDC(ScreenHDC());
	BitmapInfo32__ bi(cx, cy);
	hbmp = CreateDIBSection(dc, bi, DIB_RGB_COLORS, (void **)&pixels, NULL, 0);
	hbmpOld = (HBITMAP) ::SelectObject(dc, hbmp);
}

ImageDraw::Section::~Section()
{
	GuiLock __;
	::DeleteObject(::SelectObject(dc, hbmpOld));
	::DeleteDC(dc);
}

void ImageDraw::Init()
{
	GuiLock __;
	rgb.Init(size.cx, size.cy);
	a.Init(size.cx, size.cy);
	Attach(rgb.dc);
	InitClip(size);
	alpha.Attach(a.dc);
	alpha.InitClip(size);
	has_alpha = false;
}

Draw& ImageDraw::Alpha()
{
	if(!has_alpha) {
		alpha.DrawRect(size, GrayColor(0));
		has_alpha = true;
	}
	return alpha;
}

Image ImageDraw::Get(bool pm) const
{
	ImageBuffer b(size);
	int n = size.cx * size.cy;
	memcpy(~b, rgb.pixels, n * sizeof(RGBA));
	const RGBA *s = a.pixels;
	const RGBA *e = a.pixels + n;
	RGBA *t = b;
	if(has_alpha) {
		while(s < e) {
			t->a = s->r;
			t++;
			s++;
		}
		if(pm)
			Premultiply(b);
		b.SetKind(IMAGE_ALPHA);
	}
	else {
		while(s < e) {
			t->a = 255;
			t++;
			s++;
		}
		b.SetKind(IMAGE_OPAQUE);
	}
	return Image(b);
}

ImageDraw::operator Image() const
{
	return Get(true);
}

Image ImageDraw::GetStraight() const
{
	return Get(false);
}

ImageDraw::ImageDraw(Size sz)
{
	size = sz;
	Init();
}

ImageDraw::ImageDraw(int cx, int cy)
{
	size = Size(cx, cy);
	Init();
}

ImageDraw::~ImageDraw()
{
	Detach();
	alpha.Detach();
}

#ifdef PLATFORM_WINCE

Image Image::Arrow() { return Null; }
Image Image::Wait() { return Null; }
Image Image::IBeam() { return Null; }
Image Image::No() { return Null; }
Image Image::SizeAll() { return Null; }
Image Image::SizeHorz() { return Null; }
Image Image::SizeVert() { return Null; }
Image Image::SizeTopLeft() { return Null; }
Image Image::SizeTop() { return Null; }
Image Image::SizeTopRight() { return Null; }
Image Image::SizeLeft() { return Null; }
Image Image::SizeRight() { return Null; }
Image Image::SizeBottomLeft() { return Null; }
Image Image::SizeBottom() { return Null; }
Image Image::SizeBottomRight() { return Null; }

#else

static Image sWin32Icon(HICON icon, bool cursor)
{
	GuiLock __;
	ICONINFO iconinfo;
	if(!icon || !GetIconInfo(icon, &iconinfo))
		return Image();
	BITMAP bm;
	::GetObject((HGDIOBJ)iconinfo.hbmMask, sizeof(BITMAP), (LPVOID)&bm);
	HDC dcMem = ::CreateCompatibleDC(NULL);
	Size sz(bm.bmWidth, bm.bmHeight);
	BitmapInfo32__ bi(sz.cx, sz.cy);
	int len = sz.cx * sz.cy;
	Buffer<RGBA> mask(len);
	::SelectObject(dcMem, iconinfo.hbmColor);
	::GetDIBits(dcMem, iconinfo.hbmMask, 0, sz.cy, ~mask, bi, DIB_RGB_COLORS);
	ImageBuffer b(sz.cx, iconinfo.hbmColor ? sz.cy : sz.cy / 2);
	b.SetHotSpot(Point(iconinfo.xHotspot, iconinfo.yHotspot));
	if(iconinfo.hbmColor) {
		::SelectObject(dcMem, iconinfo.hbmColor);
		::GetDIBits(dcMem, iconinfo.hbmColor, 0, sz.cy, ~b, bi, DIB_RGB_COLORS);
		RGBA *s = ~b;
		RGBA *e = s + len;
		RGBA *m = mask;
		while(s < e) {
			if(s->a != 255 && s->a != 0) {
				Premultiply(b);
				goto alpha;
			}
			s++;
		}
		s = ~b;
		while(s < e) {
			s->a = m->r ? 0 : 255;
			s++;
			m++;
		}
	}
	else {
		len /= 2;
		RGBA *s = ~b;
		RGBA *e = s + len;
		RGBA *c = mask + len;
		RGBA *m = mask;
		while(s < e) {
			s->a = (m->r & ~c->r) ? 0 : 255;
			s->r = s->g = s->b = (c->r & ~m->r) ? 255 : 0;
			s++;
			m++;
			c++;
		}
	}
alpha:
	::DeleteDC(dcMem);
	::DeleteObject(iconinfo.hbmColor);
	::DeleteObject(iconinfo.hbmMask);
	Image img(b);
	::DestroyIcon(icon);
	return img;
}

Image SystemDraw::Win32IconCursor(LPCSTR id, int iconsize, bool cursor)
{
	HICON icon;
	if(cursor)
		icon = (HICON)LoadCursor(0, id);
	else
		if(iconsize)
			icon = (HICON)LoadImage(GetModuleHandle(NULL), id,
			                        IMAGE_ICON, iconsize, iconsize, LR_DEFAULTCOLOR);
		else {
			static HRESULT (WINAPI *LoadIconWithScaleDown)(HINSTANCE hinst, PCWSTR pszName, int cx, int cy, HICON *phico);
			ONCELOCK {
				DllFn(LoadIconWithScaleDown, "Comctl32.dll", "LoadIconWithScaleDown");
			}

			icon = NULL;
			if(LoadIconWithScaleDown)
				LoadIconWithScaleDown(0, (PCWSTR) id, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), &icon);
			else
				icon = LoadIcon(0, id);
		}
	Image img = sWin32Icon(icon, cursor);
	if(cursor)
		img.SetAuxData(reinterpret_cast<uint64>(id));
	return img;
}

Image Win32DllIcon(const char *dll, int ii, bool large)
{
	HICON icon;
	if(ExtractIconEx(dll, ii, large ? &icon : NULL, large ? NULL : &icon, 1) == 1)
		return sWin32Icon(icon, false);
	return Null;
}

Image Win32Icon(LPCSTR id, int iconsize)
{
	return SystemDraw::Win32IconCursor(id, iconsize, false);
}

Image Win32Icon(int id, int iconsize)
{
	return Win32Icon(MAKEINTRESOURCE(id), iconsize);
}

Image Win32Cursor(LPCSTR id)
{
	return SystemDraw::Win32IconCursor(id, 0, true);
}

Image Win32Cursor(int id)
{
	return Win32Cursor(MAKEINTRESOURCE(id));
}

HICON SystemDraw::IconWin32(const Image& img_, bool cursor)
{
	GuiLock __;
	Image img = img_;
	if(img.IsEmpty())
		return NULL;
	if(cursor) {
		LPCSTR id = reinterpret_cast<LPCSTR>(img.GetAuxData());
		if(id)
			return (HICON)LoadCursor(0, id);
	}
	img = Unmultiply(img); // it seems like Win32 wants them unmultiplied...
	Size sz = img.GetSize();
	ICONINFO iconinfo;
	iconinfo.fIcon = !cursor;
	Point p = img.GetHotSpot();
	iconinfo.xHotspot = p.x;
	iconinfo.yHotspot = p.y;
	Size tsz = sz;
	Size csz = Size(min(tsz.cx, sz.cx), min(tsz.cy, sz.cy));
	if(!ImageFallBack) {
		RGBA *pixels;
		BitmapInfo32__ bi(tsz.cx, tsz.cy);
		HDC dcMem = ::CreateCompatibleDC(NULL);
		iconinfo.hbmColor = ::CreateDIBSection(dcMem, bi, DIB_RGB_COLORS, (void **)&pixels, NULL, 0);
		iconinfo.hbmMask = ::CreateBitmap(tsz.cx, tsz.cy, 1, 1, NULL);
		Fill(pixels, RGBAZero(), tsz.cx * tsz.cy);
		for(int y = 0; y < csz.cy; y++)
			memcpy(pixels + y * tsz.cx, img[y], csz.cx * sizeof(RGBA));
		::DeleteDC(dcMem);
	}
	else {
		Buffer<RGBA>  h(tsz.cx * tsz.cy);
		HDC dc = ::GetDC(NULL);
		BitmapInfo32__ bi(tsz.cx, tsz.cy);
		iconinfo.hbmMask = CreateBitMask(~img, sz, tsz, csz, h);
		iconinfo.hbmColor = ::CreateDIBitmap(dc, bi, CBM_INIT, h, bi, DIB_RGB_COLORS);
		::ReleaseDC(NULL, dc);
	}

	HICON icon = ::CreateIconIndirect(&iconinfo);
	::DeleteObject(iconinfo.hbmColor);
	::DeleteObject(iconinfo.hbmMask);
	return icon;
}

#define WCURSOR_(x)\
{ Image m; INTERLOCKED { static Image img = Win32Cursor(x); m = img; } return m; }

Image Image::Arrow() WCURSOR_(IDC_ARROW)
Image Image::Wait() WCURSOR_(IDC_WAIT)
Image Image::IBeam() WCURSOR_(IDC_IBEAM)
Image Image::No() WCURSOR_(IDC_NO)
Image Image::SizeAll() WCURSOR_(IDC_SIZEALL)
Image Image::SizeHorz()	WCURSOR_(IDC_SIZEWE)
Image Image::SizeVert() WCURSOR_(IDC_SIZENS)
Image Image::SizeTopLeft() WCURSOR_(IDC_SIZENWSE)
Image Image::SizeTop() WCURSOR_(IDC_SIZENS)
Image Image::SizeTopRight() WCURSOR_(IDC_SIZENESW)
Image Image::SizeLeft() WCURSOR_(IDC_SIZEWE)
Image Image::SizeRight() WCURSOR_(IDC_SIZEWE)
Image Image::SizeBottomLeft() WCURSOR_(IDC_SIZENESW)
Image Image::SizeBottom() WCURSOR_(IDC_SIZENS)
Image Image::SizeBottomRight()  WCURSOR_(IDC_SIZENWSE)
Image Image::Cross() WCURSOR_(IDC_CROSS)
Image Image::Hand() WCURSOR_(IDC_HAND)

#endif

}

#endif
