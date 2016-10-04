#include <CtrlLib/CtrlLib.h>

struct App : TopWindow {
	virtual void Paint(Draw& draw);
};

struct RGBA {
	byte b, g, r, a;
};


#define Image NewImage

class ImageBuffer {
	int          kind;
	Size         size;
	Buffer<RGBA> data;

public:
	RGBA *operator[](int i)             { ASSERT(i >= 0 && i < size.cy); return ~data + size.cx * (size.cy - i - 1); }
	const RGBA *operator[](int i) const { ASSERT(i >= 0 && i < size.cy); return ~data + size.cx * (size.cy - i - 1); }
	int   GetLineDelta()() const        {

	ImageBuffer(Size sz);
	ImageBuffer(pick_ ImageBuffer& b);
	ImageBuffer(Image& img);
};

enum ImageKind {
	IMAGE_UNKNOWN,
	IMAGE_EMPTY,
	IMAGE_ALPHA,
	IMAGE_MASK,
	IMAGE_OPAQUE,
};

struct Image {
	struct ImageData {
		Atomic refcount;
		void   Retain() { AtomicInc(refcount); }
		void   Relase() { if(AtomicDec(refcount) == 0) delete this; }

		int         kind;
		bool        dibsection;
		HBITMAP     hbmp;
		HBITMAP     mask;
		RGBA       *pixel;
		int         linedelta;
		ImageBuffer data;
		int         storecount;


		ImageData()   { kind = UNKNOWN; AtomicWrite(refcount, 1); }
		~ImageData()  {
	};



	void Paint(HDC

	Image(Size sz);
}

ImageBuffer px(img);
RGBA *x = px[20];
x += px.GetLineDelta();



typedef BOOL (WINAPI *tAlphaBlend)(
  HDC hdcDest,                 // handle to destination DC
  int nXOriginDest,            // x-coord of upper-left corner
  int nYOriginDest,            // y-coord of upper-left corner
  int nWidthDest,              // destination width
  int nHeightDest,             // destination height
  HDC hdcSrc,                  // handle to source DC
  int nXOriginSrc,             // x-coord of upper-left corner
  int nYOriginSrc,             // y-coord of upper-left corner
  int nWidthSrc,               // source width
  int nHeightSrc,              // source height
  BLENDFUNCTION blendFunction  // alpha-blending function
);

static tAlphaBlend AlphaBlend()
{
	static tAlphaBlend pSet;
	static bool inited = false;
	if(!inited) {
		inited = true;
		if(HMODULE hDLL = LoadLibrary("msimg32.dll"))
			pSet = (tAlphaBlend) GetProcAddress(hDLL, "AlphaBlend");
	}
	return pSet;
}

void App::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White);

	RGBA *pixel;

	Buffer<byte> bibuffer(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256);
	BITMAPINFOHEADER *hi = (BITMAPINFOHEADER *) ~bibuffer;;
	BITMAPINFO *bi = (BITMAPINFO *)hi;

	memset(bi, 0, sizeof(BITMAPINFOHEADER));
	hi->biSize = sizeof(BITMAPINFOHEADER);
	hi->biPlanes = 1;
	hi->biBitCount = 32;
	hi->biCompression = BI_RGB;
	hi->biSizeImage = 0;
	hi->biClrUsed = 0;
	hi->biClrImportant = 0;

	hi->biWidth = 100;
	hi->biHeight = 100;

	HBITMAP hbmp = CreateDIBSection(ScreenInfo().GetHandle(), bi, DIB_RGB_COLORS, (void **)&pixel, NULL, 0);

	HDC dc = w.GetHandle();
	HDC dcMem = ::CreateCompatibleDC(dc);
	HBITMAP hbmOld = (HBITMAP) ::SelectObject(dcMem, hbmp);

	for(int x = 0; x < 100; x++)
		for(int y = 0; y < 100; y++) {
			RGBA& b = *(pixel + 100 * x + y);
			b.r = 0;
			b.b = 100;
			b.g = 255;
			b.a = x + y;
		}

	if(false) {
		BLENDFUNCTION bf;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 255;
		bf.AlphaFormat = AC_SRC_ALPHA;
		::AlphaBlend()(dc, 0, 0, 100, 100, dcMem, 0, 0, 100, 100, bf);
		::AlphaBlend()(dc, 80, 80, 100, 100, dcMem, 0, 0, 100, 100, bf);
	}
	else {
		pos = Point(x, y);
		size = Size(cx, cy);
		Buffer<byte> bibuffer(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256);
		BITMAPINFOHEADER *hi = (BITMAPINFOHEADER *) ~bibuffer;;
		BITMAPINFO *bi = (BITMAPINFO *)hi;
		memset(bi, 0, sizeof(BITMAPINFOHEADER));
		hi->biSize = sizeof(BITMAPINFOHEADER);
		hi->biPlanes = 1;
		hi->biBitCount = 32;
		hi->biCompression = BI_RGB;
		hi->biSizeImage = 0;
		hi->biClrUsed = 0;
		hi->biClrImportant = 0;
		hi->biWidth = size.cx;
		hi->biHeight = size.cy;
		hbmp = CreateDIBSection(ScreenInfo().GetHandle(), bi, DIB_RGB_COLORS, (void **)&pixel, NULL, 0);
		dcMem = ::CreateCompatibleDC(hdc);
		hbmOld = (HBITMAP) ::SelectObject(dcMem, hbmp);
		::BitBlt(dcMem, 0, 0, size.cx, size.cy, hdc, x, y, SRCCOPY);



		::BitBlt(hdc, pos.x, pos.y, size.cx, size.cy, dcMem, 0, 0, SRCCOPY);
	    ::SelectObject(dcMem, hbmOld);
	    ::DeleteDC(dcMem);
		::DeleteObject(hbmp);
	}

	::SelectObject(dcMem, hbmOld);
	::DeleteDC(dcMem);
}

struct RasterData {
	int
	int     uploaded;
	int     kind;

	HBITMAP server;
	HBITMAP server_mask;
};

GUI_APP_MAIN
{
	App().Run();
}