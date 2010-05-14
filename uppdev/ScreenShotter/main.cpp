#include <CtrlLib/CtrlLib.h>

using namespace Upp;

Image ScreenShot(int x, int y, int cx, int cy)
{
	HDC dcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);

	RGBA *pixels;

	Buffer<byte> data;
	data.Alloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256);
	BITMAPINFOHEADER *hi = (BITMAPINFOHEADER *) ~data;;
	memset(hi, 0, sizeof(BITMAPINFOHEADER));
	hi->biSize = sizeof(BITMAPINFOHEADER);
	hi->biPlanes = 1;
	hi->biBitCount = 32;
	hi->biCompression = BI_RGB;
	hi->biSizeImage = 0;
	hi->biClrUsed = 0;
	hi->biClrImportant = 0;
	hi->biWidth = cx;
	hi->biHeight = -cy;

	HBITMAP hbmp = CreateDIBSection(dcScreen, (BITMAPINFO *)hi, DIB_RGB_COLORS, (void **)&pixels, NULL, 0);
	HDC dcMem = ::CreateCompatibleDC(dcScreen);
	HBITMAP hbmpOld = (HBITMAP) ::SelectObject(dcMem, hbmp);
	HDC hdcCompatible = CreateCompatibleDC(dcScreen);
	::BitBlt(dcMem, 0, 0, cx, cy, dcScreen, x, y, SRCCOPY);
	
	ImageBuffer ib(cx, cy);
	memcpy(~ib, pixels, cx * cy * sizeof(RGBA));
	
	::DeleteObject(::SelectObject(dcMem, hbmpOld));
	::DeleteDC(dcMem);
	::DeleteDC(dcScreen);
	
	return ib;
};

GUI_APP_MAIN
{
	Image m = ScreenShot(100, 100, 300, 300);
	PNGEncoder().SaveFile("u:/test.png", m);
}
