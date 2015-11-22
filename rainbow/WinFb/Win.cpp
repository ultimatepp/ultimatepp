#include "WinFb.h"

#ifdef flagWINFB

NAMESPACE_UPP

#define LLOG(x) //DLOG(x)

HWND fbHWND;

bool FBIsWaitingEvent()
{
	MSG msg;
	return PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
}

bool FBProcessEvent(bool *quit)
{
	MSG msg;
	if(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		if(msg.message == WM_QUIT && quit)
			*quit = true;
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
		return true;
	}
	return false;
}

void FBQuitSession()
{
	::PostQuitMessage(0);
}

void FBSleep(int ms)
{
	TimeStop tm;
	MsgWaitForMultipleObjects(0, NULL, FALSE, ms, QS_ALLINPUT);
	LLOG("@ FBSleep " << tm.Elapsed());
}

void FBInit(HINSTANCE hInstance)
{
	GuiLock __;
	
	Ctrl::InitFB();
	
	WNDCLASSW  wc;
	Zero(wc);
	wc.style         = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)fbWindowProc;
	wc.hInstance     = hInstance;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)NULL;
	wc.lpszClassName = L"UPP-FB-CLASS";
	RegisterClassW(&wc);
	fbHWND = CreateWindowW(L"UPP-FB-CLASS", L"",
	                       WS_OVERLAPPED|WS_VISIBLE|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_SYSMENU,
	                       CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
	                       NULL, NULL, hInstance, NULL);
	SetTimer(fbHWND, 1, 10, NULL);

//	Csizeinit();
}

void fbUpdate(HDC hdc, const Rect& r_)
{
	const ImageBuffer& framebuffer = Ctrl::GetFrameBuffer();
	Size sz = framebuffer.GetSize();
	Rect r = sz;
//	Rect r = r_;
	LLOG("fbUpdate " << r);
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
	hi->biWidth = sz.cx;
	hi->biHeight = -sz.cy;
	::SetDIBitsToDevice(hdc, r.left, r.top, r.GetWidth(), r.GetHeight(),
	                    r.left, -r.top - r.GetHeight() + sz.cy, 0, sz.cy,
	                    ~framebuffer, (BITMAPINFO *)~data, DIB_RGB_COLORS);
}

void FBInitUpdate()
{

}

void FBUpdate(const Rect& r)
{
	LLOG("FBUpdate " << r);
	if(fbHWND) {
		HDC hdc = GetDC(fbHWND);
		fbUpdate(hdc, r);
	#if 0
		::PatBlt(hdc, r.left, r.top, r.GetWidth(), r.GetHeight(), DSTINVERT);
		Sleep(40);
		::PatBlt(hdc, r.left, r.top, r.GetWidth(), r.GetHeight(), DSTINVERT);
	#endif
		ReleaseDC(fbHWND, hdc);
	}
//	::InvalidateRect(fbHWND, NULL, false);
}

void FBFlush()
{
	::UpdateWindow(fbHWND);
	GdiFlush();
}

END_UPP_NAMESPACE

#endif
