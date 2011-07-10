#include <CtrlLib/CtrlLib.h>

#ifdef GUI_WINGL

//#include <shellapi.h>

NAMESPACE_UPP

#define LTIMING(x) // RTIMING(x)

void SetSurface(SystemDraw& w, int x, int y, int cx, int cy, const RGBA *pixels)
{
	GuiLock __;
}

void SetSurface(SystemDraw& w, const Rect& dest, const RGBA *pixels, Size psz, Point poff)
{
	GuiLock __;
}

struct Image::Data::SystemData {
};

void Image::Data::SysInitImp()
{
	SystemData& sd = Sys();
}

void Image::Data::SysReleaseImp()
{
	SystemData& sd = Sys();
}

Image::Data::SystemData& Image::Data::Sys() const
{
	ASSERT(sizeof(system_buffer) >= sizeof(SystemData));
	return *(SystemData *)system_buffer;
}

int  Image::Data::GetResCountImp() const
{
	SystemData& sd = Sys();
	return 0;
}

void Image::Data::PaintImp(SystemDraw& w, int x, int y, const Rect& src, Color c)
{
	GuiLock __;
	SystemData& sd = Sys();
}

Image ImageDraw::Get(bool pm) const
{
	ImageBuffer result(image.GetSize());
	const RGBA *e = image.End();
	const RGBA *p = ~image;
	RGBA *t = ~result;
	if(has_alpha) {
		const RGBA *a = ~alpha;
		while(p < e) {
			*t = *p++;
			(t++)->a = (a++)->r;
		}
		if(pm)
			Premultiply(result);
		result.SetKind(IMAGE_ALPHA);
	}
	else {
		while(p < e) {
			*t = *p++;
			(t++)->a = 255;
		}
	}
	return result;
}

Draw& ImageDraw::Alpha()
{
	has_alpha = true;
	return alpha_painter;
}


ImageDraw::ImageDraw(Size sz)
:	ImageDraw__(sz.cx, sz.cy),
	BufferPainter(image),
	alpha_painter(alpha)
{
	has_alpha = false;
}

ImageDraw::ImageDraw(int cx, int cy)
:	ImageDraw__(cx, cy),
	BufferPainter(image),
	alpha_painter(alpha)
{
	has_alpha = false;
}
/*

void    Image::SetCursorCheat(LPCSTR id)
{
	data->Sys().cursor_cheat = id;
}

LPCSTR  Image::GetCursorCheat() const
{
	return data ? data->Sys().cursor_cheat : NULL;
}

Image::Data::SystemData& Image::Data::Sys() const
{
	ASSERT(sizeof(system_buffer) >= sizeof(SystemData));
	return *(SystemData *)system_buffer;
}

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
	memset(hi, 0, sizeof(BITMAPINFOHEADER));
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

Image Win32IconCursor(LPCSTR id, int iconsize, bool cursor)
{
	HICON icon;
	if(cursor)
		icon = (HICON)LoadCursor(0, id);
	else
		if(iconsize)
			icon = (HICON)LoadImage(GetModuleHandle(NULL), id,
			                        IMAGE_ICON, iconsize, iconsize, LR_DEFAULTCOLOR);
		else
			icon = LoadIcon(0, id);
	Image img = sWin32Icon(icon, cursor);
	if(cursor)
		img.SetCursorCheat(id);
	return img;
}

Image Win32Icon(LPCSTR id, int iconsize)
{
	return Win32IconCursor(id, iconsize, false);
}

Image Win32Icon(int id, int iconsize)
{
	return Win32Icon(MAKEINTRESOURCE(id), iconsize);
}

Image Win32Cursor(LPCSTR id)
{
	return Win32IconCursor(id, 0, true);
}

Image Win32Cursor(int id)
{
	return Win32Cursor(MAKEINTRESOURCE(id));
}

HBITMAP CreateBitMask(const RGBA *data, Size sz, Size tsz, Size csz, RGBA *ct)
{
	GuiLock __;
	memset(ct, 0, tsz.cx * tsz.cy * sizeof(RGBA));
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

HICON IconWin32(const Image& img, bool cursor)
{
	GuiLock __;
	if(img.IsEmpty())
		return NULL;
	if(cursor) {
		LPCSTR id = img.GetCursorCheat();
		if(id)
			return (HICON)LoadCursor(0, id);
	}
	Size sz = img.GetSize();
	ICONINFO iconinfo;
	iconinfo.fIcon = !cursor;
	Point p = img.GetHotSpot();
	iconinfo.xHotspot = p.x;
	iconinfo.yHotspot = p.y;
	static Size cursor_size(GetSystemMetrics(SM_CXCURSOR), GetSystemMetrics(SM_CYCURSOR));
	Size tsz = sz;
	if(!IsWin2K() && cursor)
		tsz = cursor_size;
	Size csz = Size(min(tsz.cx, sz.cx), min(tsz.cy, sz.cy));
 if(IsWinXP() && !ImageFallBack) {
		RGBA *pixels;
		BitmapInfo32__ bi(tsz.cx, tsz.cy);
		HDC dcMem = ::CreateCompatibleDC(NULL);
		iconinfo.hbmColor = ::CreateDIBSection(dcMem, bi, DIB_RGB_COLORS, (void **)&pixels, NULL, 0);
		iconinfo.hbmMask = ::CreateBitmap(tsz.cx, tsz.cy, 1, 1, NULL);
		memset(pixels, 0, tsz.cx * tsz.cy * sizeof(RGBA));
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
Image Image::Hand() WCURSOR_(IDC_HAND)*/


#define IMAGECLASS WinGlImg
#define IMAGEFILE <WinGl/WinGl.iml>
#include <Draw/iml_source.h>

Image Image::Arrow() { return WinGlImg::arrow(); }
Image Image::Wait() { return WinGlImg::wait(); }
Image Image::IBeam() { return WinGlImg::ibeam(); }
Image Image::No() { return WinGlImg::no(); }
Image Image::SizeAll() { return WinGlImg::sizeall(); }
Image Image::SizeHorz() { return WinGlImg::sizehorz(); }
Image Image::SizeVert() { return WinGlImg::sizevert(); }
Image Image::SizeTopLeft() { return WinGlImg::sizetopleft(); }
Image Image::SizeTop() { return WinGlImg::sizetop(); }
Image Image::SizeTopRight() { return WinGlImg::sizetopright(); }
Image Image::SizeLeft() { return WinGlImg::sizeleft(); }
Image Image::SizeRight() { return WinGlImg::sizeright(); }
Image Image::SizeBottomLeft() { return WinGlImg::sizebottomleft(); }
Image Image::SizeBottom() { return WinGlImg::sizebottom(); }
Image Image::SizeBottomRight() { return WinGlImg::sizebottomright(); }

END_UPP_NAMESPACE

#endif
