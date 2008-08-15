#include "MDraw.h"

void PixelPanic()
{
	Panic("Unable to initialize graphics!");
}

void PixelBlock::Init()
{
	RTIMING("Init");

	HBITMAP hbm;

	hbm = CreateCompatibleBitmap(ScreenInfo().GetHandle(), 1, 1);
	if (!hbm)
	    PixelPanic();

	Buffer<byte> bibuffer(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256);
	BITMAPINFOHEADER *hi = (BITMAPINFOHEADER *) ~bibuffer;;
	BITMAPINFO *bi = (BITMAPINFO *)hi;

	memset(bi, 0, sizeof(BITMAPINFOHEADER));
	hi->biSize = sizeof(BITMAPINFOHEADER);

	if(!GetDIBits(ScreenInfo().GetHandle(), hbm, 0, 1, NULL, bi, DIB_RGB_COLORS)
	   || !GetDIBits(ScreenInfo().GetHandle(), hbm, 0, 1, NULL, bi, DIB_RGB_COLORS))
		PixelPanic();

	DeleteObject(hbm);

	dword rm, gm, bm;
	rm = gm = bm = 0;

	if(hi->biCompression == BI_BITFIELDS) {
		dword *dw = (dword *)bi->bmiColors;
		rm = dw[0];
		gm = dw[1];
		bm = dw[2];
	}

	LOG(Sprintf("bpp: %2d, rm:%8x gm:%8x bm:%8x", hi->biBitCount, rm, gm, bm));

	display = RGBX;
	switch(hi->biBitCount) {
	case 8:
		display = PALETTE8;
		break;
	case 16:
		if(rm == 0xf800 && gm == 0x7e0 && bm == 0x1f)
			display = RGB565;
		if(rm == 0x7c00 && gm == 0x3e0 && bm == 0x1f)
			display = RGB555;
		if(bm == 0xf800 && gm == 0x7e0 && rm == 0x1f)
			display = BGR565;
		if(bm == 0x7c00 && gm == 0x3e0 && rm == 0x1f)
			display = BGR555;
		break;
	case 24:
		display = RGB;
		break;
	}
}

void PixelBlock::Put(Draw& w, int x, int y)
{
	if(!display)
		Init();

	if(hbmp) {
		RTIMING("Section");
		HDC dc = w.GetHandle();
		HDC dcMem = ::CreateCompatibleDC(dc);
		HBITMAP hbmOld = (HBITMAP) ::SelectObject(dcMem, hbmp);
		::BitBlt(dc, x, y, size.cx, size.cy, dcMem, 0, 0, SRCCOPY);
        ::SelectObject(dcMem, hbmOld);
        ::DeleteDC(dcMem);
	}
	else {
		RTIMING("Set");
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

		SetDIBitsToDevice(w.GetHandle(), x, y, size.cx, size.cy,
		                  0, 0, 0, size.cy,
		                  pixel, bi, DIB_RGB_COLORS);
	}
}
