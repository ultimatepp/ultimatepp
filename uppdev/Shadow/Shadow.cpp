#include <CtrlLib/CtrlLib.h>

struct Shadow : public Ctrl {
	void Paint(HDC hdc);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
		HWND hwnd = GetHWND();
		if(message == WM_CREATE)
			SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
		if(message == WM_PAINT) {
			if(IsVisible() && hwnd) {
				PAINTSTRUCT ps;
				HDC dc = BeginPaint(hwnd, &ps);
				Paint(dc);
				EndPaint(hwnd, &ps);
			}
			return 0L;
		}
		return Ctrl::WindowProc(message, wParam, lParam);
	}
};

struct SRGBA {
	byte r, g, b, a;
};

struct DrawDirect {
	SRGBA  *pixel;
	Point   pos;
	Size    size;
	HBITMAP hbmp, hbmOld;
	HDC     hdc, dcMem;

	operator SRGBA*()        { return pixel; }
	SRGBA *operator[](int i) { return pixel + size.cx * i; }

	DrawDirect(HDC hdc, int x, int y, int cx, int cy);
	~DrawDirect();
};

DrawDirect::DrawDirect(HDC _hdc, int x, int y, int cx, int cy)
{
	hdc = _hdc;
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
}

DrawDirect::~DrawDirect()
{
	::BitBlt(hdc, pos.x, pos.y, size.cx, size.cy, dcMem, 0, 0, SRCCOPY);
    ::SelectObject(dcMem, hbmOld);
    ::DeleteDC(dcMem);
	::DeleteObject(hbmp);
}

int  CompShadow(int level)
{
	return min(level * 50 + 70, 256);
}

inline void SetShadow(SRGBA *p, int black)
{
	p->r = (black * p->r) >> 8;
	p->g = (black * p->g) >> 8;
	p->b = (black * p->b) >> 8;
}

void Shadow::Paint(HDC hdc)
{
	Size  size = GetSize();
	if(size.cx > size.cy) {
		if(size.cx > 10) {
			DrawDirect dd(hdc, 0, 0, size.cx, 3);
			for(int y = 0; y < 3; y++) {
				SRGBA *p = dd[y];
				for(int i = 0; i < 3; i++)
					SetShadow(p++, CompShadow(5 - i - y));
				int b = CompShadow(3 - y);
				for(int i = size.cx - 6; i--;)
					SetShadow(p++, b);
				for(int i = 0; i < 3; i++)
					SetShadow(p++, CompShadow(3 + i - y));
			}
		}
	}
	else {
		if(size.cy > 10) {
			DrawDirect dd(hdc, 0, 0, 3, size.cy);
			for(int x = 0; x < 3; x++) {
				SRGBA *p = dd.pixel + x;
				int b = CompShadow(x + 1);
				for(int i = size.cy - 3; i--;) {
					SetShadow(p, b);
					p += size.cx;
				}
				for(int i = 0; i < 3; i++) {
					SetShadow(p, CompShadow(i + x + 1));
					p += size.cx;
				}
			}
		}
	}
}

GUI_APP_MAIN
{
	Shadow w, w1;
	w.SetRect(808, 200, 95, 3);
	w.PopUp();
	w1.SetRect(900, 108, 3, 92);
	w1.PopUp();
	StaticRect test;
	test.SetFrame(BlackFrame());
	test.SetRect(800, 100, 100, 100);
	test.PopUp();
	TopWindow ww;
	ww.SetRect(400, 400, 100, 100);
	ww.OpenMain();
	ww.Run();
}