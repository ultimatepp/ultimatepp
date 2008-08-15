#include "MDraw.h"

int    PixelBlock::display;

void   PixelBlock::Create0(int cx, int cy, bool section)
{
	if(!display)
		Init();

	hbmp = NULL;
	size.cx = cx;
	size.cy = cy;

#ifdef PLATFORM_WIN32
	if(section) {
		TIMING("Create section");
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

		return;
	}
#endif

	buffer.Alloc(cx * cy);
	pixel = buffer;
}


void   PixelBlock::Create(int cx, int cy)
{
	Clear();
	Create0(cx, cy);
}

void   PixelBlock::Clear()
{
	if(hbmp)
		::DeleteObject(hbmp);
	else
		buffer.Clear();
	pixel = NULL;
	hbmp = NULL;
	size.cx = size.cy = 0;
}

void   PixelBlock::DrawRect(int x, int y, int cx, int cy, Color c)
{
	RTIMING("DrawRect");
	dword color = c.GetRaw();
	dword *l = pixel + y * size.cx + x;
	while(cy--) {
		memsetd(l, color, cx);
//		memset(l, color, 2 * cx);
		l += size.cx;
	}
}

int    PixelBlock::LineDelta()
{
	return size.cx;
}

dword *PixelBlock::PointAdr(int x, int y)
{
	return pixel + y * size.cx + x;
}

PixelBlock::PixelBlock()
{
	size.cx = size.cy = 0;
	hbmp = NULL;
	pixel = NULL;
}

PixelBlock::PixelBlock(int cx, int cy, bool section)
{
	Create0(cx, cy, section);
}

PixelBlock::~PixelBlock()
{
	Clear();
}
