#include "SysInfo_in.h"

#include <plugin/png/png.h>
#include <plugin/jpg/jpg.h>
#include <plugin/bmp/bmp.h>

NAMESPACE_UPP


bool Window_SaveCapture(int64 windowId, String fileName, int left, int top, int width, int height)
{
	Image img = Window_SaveCapture(windowId, left, top, width, height);
	if (IsNull(img))
		return false;
	
	if (GetFileExt(fileName) == ".png") {
		PNGEncoder encoder;
		return encoder.SaveFile(fileName, img);
	} else if (GetFileExt(fileName) == ".jpg") {	
		JPGEncoder encoder(90);
		return encoder.SaveFile(fileName, img);		
	} else if (GetFileExt(fileName) == ".bmp") {
		BMPEncoder encoder;
		return encoder.SaveFile(fileName, img);
	} else
		return false;
}

#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)

#if defined(__MINGW32__) 
	#define labs(x)	labs((Upp::int64)(x))
#elif defined(_MSC_VER)
	#define labs(x)	abs(x)
#endif


Rect GetDesktopRect() {
	HWND wH = GetDesktopWindow();
	 
	RECT rc;
	if (!GetWindowRect(wH, &rc))
		return Null;

	Rect ret(rc.left, rc.top, rc.right, rc.bottom);
	return ret;
}

Image Window_SaveCapture(int64 windowId, int left, int top, int width, int height) {
	HWND wH = reinterpret_cast<HWND>(windowId);
	if (wH == 0)
		wH = GetDesktopWindow();
	 
	RECT rc;
	if (!GetWindowRect(wH, &rc))
		return Null;

	if (left == -1)
		left = rc.left;
	if (top == -1)
		top = rc.top;
	if (width == -1)
		width	= rc.right - rc.left;
	if (height == -1)
		height	= rc.bottom - rc.top;

	HDC hDC = NULL;
	HDC memDC = NULL;
	HBITMAP hb = NULL;
	HBITMAP oldBM = NULL;
	Image img = Null;
	ImageBuffer b;
	
	if ((hDC = GetDC(0)) == NULL)
		goto end;
	if ((memDC = CreateCompatibleDC(hDC)) == NULL) 
		goto end;
	if ((hb = CreateCompatibleBitmap(hDC, width, height)) == NULL) 
		goto end;	
	if ((oldBM = (HBITMAP)SelectObject(memDC, hb)) == NULL)
		goto end;			
	if (!BitBlt(memDC, 0, 0, width, height , hDC, left, top, SRCCOPY))
		goto end;			
	
  	BITMAPINFO bmpInfo;
    ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    if (!GetDIBits(hDC, hb, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS))
        goto end;
   	if(bmpInfo.bmiHeader.biSizeImage <= 0)
     	bmpInfo.bmiHeader.biSizeImage = bmpInfo.bmiHeader.biWidth*labs(bmpInfo.bmiHeader.biHeight)*(bmpInfo.bmiHeader.biBitCount+7)/8;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biHeight = -height;

	b.Create(width, height);
	if (!GetDIBits(hDC, hb, 0, height, ~b, (BITMAPINFO *)&bmpInfo.bmiHeader, DIB_RGB_COLORS))
		goto end;
	
	img = b;
	
end:
	SelectObject(hDC, oldBM);
	DeleteObject(hb);
	DeleteDC(memDC);
	ReleaseDC(0, hDC);
	
	return img;
}

class ScreenGrab {
private:	
	enum {GRAB_MODE_DESKTOP, GRAB_MODE_WINDOW, GRAB_MODE_RECT};
	static HDC lDCDest;
	static HGDIOBJ lDib;
	static void *lDibPtr;
	static HGDIOBJ lBmpOld;
	static BITMAPINFO tBmpInfo;
	static PAVIFILE lAVIPtrFile;
	static PAVISTREAM lAVIPtrStrm;
	static AVISTREAMINFO tAVIHdr;
	static AVIFILEINFO tAVIFile;
	long lAVICnt;
	double frameRate;
	long numFrames; 		// number of frames in video stream
	long firstFrame; 		// position of the first video frame
	String fileName;
	bool viewMouse;
	bool opened;
	int grabMode;
	uint64 hwnd;
	int left;
	int top; 
	int width;
	int height;

	bool AVIOpen(bool create = true);
	bool AVIWrite();
	void AVIClose();
	HICON GetCursorHandle();
	bool DIBCreate(HWND lHandleSource, long lWidth, long lHeight, long lPosX, long lPosY);
	void DIBClean();
	bool ScreenshotMemory();

public:
	ScreenGrab(String fileName, double secsFrame = 1, bool viewMouse = true);
	~ScreenGrab();
	
	bool IniGrabDesktop();
	bool IniGrabWindow(uint64 handle);
	bool IniGrabDesktopRectangle(int left, int top, int width, int height);
	bool Grab(unsigned duration);
	bool GrabSnapshot();
	void Close();
};

bool Record_Desktop(String fileName, int duration, double secsFrame, bool viewMouse)
{
	ScreenGrab grab(fileName, secsFrame, viewMouse);
	if (!grab.IniGrabDesktop())
		return false;
	if (!grab.Grab(duration))
		return false;
	grab.Close();
	return true;
}
bool Record_DesktopRectangle(String fileName, int duration, int left, int top, int width, int height, double secsFrame, bool viewMouse)
{
	ScreenGrab grab(fileName, secsFrame, viewMouse);
	if (!grab.IniGrabDesktopRectangle(left, top, width, height))
		return false;
	if (!grab.Grab(duration))
		return false;
	grab.Close();
	return true;
}

bool Record_Window(String fileName, int duration, int64 handle, double secsFrame, bool viewMouse)
{
	ScreenGrab grab(fileName, secsFrame, viewMouse);
	if (!grab.IniGrabWindow(handle))
		return false;
	if (!grab.Grab(duration))
		return false;
	grab.Close();
	return true;
}

bool ScreenGrab::AVIOpen(bool create)
{
    long lRet, mode, res;
    
    AVIFileInit();
    if (create)
        mode = OF_CREATE | OF_WRITE;
    else
        mode = OF_SHARE_DENY_WRITE;

    lRet = AVIFileOpen(&lAVIPtrFile, fileName, mode, 0);
    if (lRet == AVIERR_OK && create) {
      	tAVIHdr.fccType = streamtypeVIDEO;
        tAVIHdr.fccHandler = 0;
       	tAVIHdr.dwScale = 100;		
       	tAVIHdr.dwRate = (DWORD)(tAVIHdr.dwScale*frameRate);	
       	//tAVIHdr.dwQuality = -1;
       	tAVIHdr.dwSuggestedBufferSize = tBmpInfo.bmiHeader.biSizeImage;
        SetRect(&(tAVIHdr.rcFrame), 0, 0, tBmpInfo.bmiHeader.biWidth, tBmpInfo.bmiHeader.biHeight);
        lRet = AVIFileCreateStream(lAVIPtrFile, &lAVIPtrStrm, &tAVIHdr);
        if (lRet == AVIERR_OK) {
            lRet = AVIStreamSetFormat(lAVIPtrStrm, 0, &(tBmpInfo.bmiHeader), sizeof(tBmpInfo.bmiHeader));
            if (lRet == AVIERR_OK) 
                lAVICnt = 0;
        }
    } else {
        res = AVIFileGetStream(lAVIPtrFile, &lAVIPtrStrm, streamtypeVIDEO, 0);
        if (res != AVIERR_OK)
            return false;
        firstFrame = AVIStreamStart(lAVIPtrStrm);
        if (firstFrame != -1) 
			return false;
        numFrames = AVIStreamLength(lAVIPtrStrm); 
        if (numFrames == -1)
        	return false;
        res = AVIFileInfo(lAVIPtrFile, &tAVIFile, sizeof(tAVIFile));
        if (res != AVIERR_OK)
        	return false;
        res = AVIStreamInfo(lAVIPtrStrm, &tAVIHdr, sizeof(tAVIHdr));
        if (res != AVIERR_OK) 
			return false;
    }
    return true;
}

bool ScreenGrab::AVIWrite()
{
    HRESULT  lRet;
    
    lRet = AVIStreamWrite(lAVIPtrStrm, lAVICnt, 1, lDibPtr, tBmpInfo.bmiHeader.biSizeImage, AVIIF_KEYFRAME, NULL, NULL);
    if (lRet == AVIERR_OK) {
        lAVICnt++;
        return true;
    } else
        return false;
}

void ScreenGrab::AVIClose()
{
    if (lAVIPtrStrm != 0) 
    	AVIStreamClose(lAVIPtrStrm);
    if (lAVIPtrFile != 0) 
		AVIFileClose(lAVIPtrFile);
    AVIFileExit();
}

HICON ScreenGrab::GetCursorHandle()
{
    HWND lHandle;
    POINT lpPos;
    long lThreadID;
    long lCurrentThreadID;
 
  	GetCursorPos(&lpPos);
    lHandle = WindowFromPoint(lpPos);
    lThreadID = GetWindowThreadProcessId(lHandle, 0);
    lCurrentThreadID = GetWindowThreadProcessId(reinterpret_cast<HWND>(GetWindowIdFromProcessId(GetProcessId())), 0); 
    HICON ret;
    if (lThreadID != lCurrentThreadID) {
        if (AttachThreadInput(lCurrentThreadID, lThreadID, true)) {
            ret = GetCursor();
            AttachThreadInput(lCurrentThreadID, lThreadID, false);
        }
    } else
        ret = GetCursor();
   
    return ret;
}

bool ScreenGrab::DIBCreate(HWND lHandleSource, long lWidth, long lHeight, long lPosX, long lPosY) 
{
    HDC lDCSource, lDCSourceDesktop;
    POINT lpCursorPos;
    
    lDCSource = GetWindowDC(lHandleSource);
    lDCSourceDesktop = GetWindowDC(0);
    
    bool ret = false;
    if (lDCSource != 0 && lDCSourceDesktop != 0) {
        lDCDest = CreateCompatibleDC(lDCSource);
        if (lDCDest != 0) {
            tBmpInfo.bmiHeader.biSize = sizeof(tBmpInfo.bmiHeader);
          	tBmpInfo.bmiHeader.biWidth = lWidth;
          	tBmpInfo.bmiHeader.biHeight = lHeight;
			tBmpInfo.bmiHeader.biPlanes = 1;
			tBmpInfo.bmiHeader.biBitCount = 24;
			tBmpInfo.bmiHeader.biCompression = 0;
			tBmpInfo.bmiHeader.biSizeImage = ((tBmpInfo.bmiHeader.biWidth * 3 + 3) & 0xFFFFFFFC) * tBmpInfo.bmiHeader.biHeight;
            lDib = CreateDIBSection(lDCDest, &tBmpInfo, 0, &lDibPtr, 0, 0);
            if (lDib != 0) {
                lBmpOld = SelectObject(lDCDest, lDib);
                BitBlt(lDCDest, 0, 0, lWidth, lHeight, lDCSourceDesktop, lPosX, lPosY, 0xCC0020);
                if (viewMouse) {
                    GetCursorPos(&lpCursorPos);
                    DrawIcon(lDCDest, lpCursorPos.x - lPosX, lpCursorPos.y - lPosY, GetCursorHandle());
                }
                ret = true;
            }
        }
    }
    ReleaseDC(lHandleSource, lDCSource);
    ReleaseDC(0, lDCSourceDesktop);
    return ret;
}

void ScreenGrab::DIBClean()
{
    SelectObject(lDCDest, lBmpOld);
    DeleteDC(lDCDest);
    DeleteObject(lDib);
}

bool ScreenGrab::ScreenshotMemory() 
{
    HWND lHandle;
    RECT lpRect;
    long lWidth, lHeight;
    long lPosX, lPosY;
    
	switch (grabMode) {
	case GRAB_MODE_DESKTOP:
    	lHandle = GetDesktopWindow();
    	GetWindowRect(lHandle, &lpRect);
    	lWidth = lpRect.right - lpRect.left;
    	lHeight = lpRect.bottom - lpRect.top;
    	lPosX = lpRect.left;
    	lPosY = lpRect.top;
    	break;
	case GRAB_MODE_WINDOW:
    	lHandle = reinterpret_cast<HWND>(hwnd);
    	GetWindowRect(lHandle, &lpRect);
    	lWidth = lpRect.right - lpRect.left;
    	lHeight = lpRect.bottom - lpRect.top;
    	lPosX = lpRect.left;
    	lPosY = lpRect.top;
    	break;
	case GRAB_MODE_RECT:
    	lHandle = GetDesktopWindow();
    	GetWindowRect(lHandle, &lpRect);
    	lWidth = width;
    	lHeight = height;
    	lPosX = left;
    	lPosY = top;
    	break;
    default:
    	throw Exc(t_("Unknown grab mode"));
    	return false;
	}
    if (DIBCreate(lHandle, lWidth, lHeight, lPosX, lPosY))
        return true;
  	else
    	return false;
}

HDC ScreenGrab::lDCDest;
HGDIOBJ ScreenGrab::lDib;
void *ScreenGrab::lDibPtr;
HGDIOBJ ScreenGrab::lBmpOld;
PAVIFILE ScreenGrab::lAVIPtrFile;
PAVISTREAM ScreenGrab::lAVIPtrStrm;
BITMAPINFO ScreenGrab::tBmpInfo;
AVISTREAMINFO ScreenGrab::tAVIHdr;
AVIFILEINFO ScreenGrab::tAVIFile;

ScreenGrab::ScreenGrab(String _fileName, double secsFrame, bool _viewMouse)
{
	opened = false;
	fileName = _fileName;
	viewMouse = _viewMouse;
	frameRate = 1./secsFrame;
}

ScreenGrab::~ScreenGrab()
{
	Close();
}

void ScreenGrab::Close()
{
	if (!opened)
		return;
    AVIClose();
    DIBClean();
    opened = false;
}

bool ScreenGrab::IniGrabDesktop()
{
	opened = true;
	grabMode = GRAB_MODE_DESKTOP;
	if (!ScreenshotMemory())
    	return false;
  	if (!AVIOpen())
  		return false;
	return true;
}

bool ScreenGrab::IniGrabWindow(uint64 handle)
{
	opened = true;
	grabMode = GRAB_MODE_WINDOW;
	hwnd = handle;
	if (!ScreenshotMemory())
    	return false;
  	if (!AVIOpen())
  		return false;
	return true;
}

bool ScreenGrab::IniGrabDesktopRectangle(int _left, int _top, int _width, int _height)
{
	opened = true;	
	grabMode = GRAB_MODE_RECT;
	left = _left;
	top = _top;
	width = _width;
	height = _height;
	if (!ScreenshotMemory())
    	return false;
  	if (!AVIOpen())
  		return false; 	
	return true;	
}
	
bool ScreenGrab::Grab(unsigned duration)
{
	if (!opened)
		return false;
    TimeStop timer;
    timer.Reset();
    while (timer.Elapsed() < duration*1000) {
        if (!ScreenshotMemory())
        	return false;
        if (!AVIWrite())
            return false;
        while (timer.Elapsed() < (lAVICnt*1000.)/frameRate)
	        Sleep(10);//DoEvents();
    }
    return true;
}

bool ScreenGrab::GrabSnapshot()
{
	if (!opened)
		return false;
  	if (!ScreenshotMemory())
       	return false;
   	if (!AVIWrite())
      	return false;
    return true;
}

#endif

#ifdef PLATFORM_POSIX

Window GetToplevelParent(_XDisplay *display, Window window) {
	Window parent;
	Window root;
	Window *children;
	unsigned int numChildren;
	
	while (true) {
		if (0 == XQueryTree(display, window, &root, &parent, &children, &numChildren)) 
		 	return -1;
		if (children)  
		 	XFree(children);

		if (window == root || parent == root) 
		 	return window;
		else 
		 	window = parent;
	}
}

Rect GetDesktopRect() {
	SetSysInfoX11ErrorHandler();
	
	_XDisplay *dpy = XOpenDisplay (NULL);
	if (!dpy) {
		SetX11ErrorHandler();
		return Null;
	}
	int screen = DefaultScreen(dpy);
	int64 windowId = RootWindow(dpy, screen);
	
	XWindowAttributes rc;
  	if (!XGetWindowAttributes(dpy, windowId, &rc)) {
	  	XCloseDisplay(dpy);
		SetX11ErrorHandler();
		return Null;	
  	}
  	Rect ret(rc.x, rc.y, rc.x + rc.width, rc.y + rc.height);
	
	XCloseDisplay(dpy);
	SetX11ErrorHandler();
	
	return ret;
}

Image Window_SaveCapture(int64 windowId, int left, int top, int width, int height)
{
	SetSysInfoX11ErrorHandler();
	
	_XDisplay *dpy = XOpenDisplay (NULL);
	if (!dpy) {
		SetX11ErrorHandler();
		return Null;
	}
	int screen = DefaultScreen(dpy);
	if (windowId == 0)
		windowId = RootWindow(dpy, screen);
	else {
		windowId = GetToplevelParent(dpy, windowId);
		if (windowId < 0) {
			SetX11ErrorHandler();
			return Null;
		}
	}
	
	XWindowAttributes rc;
  	if (!XGetWindowAttributes(dpy, windowId, &rc)) {
	  	XCloseDisplay(dpy);
		SetX11ErrorHandler();
		return Null;	
  	}
  	
  	if (left == -1)
		left = rc.x;
	if (top == -1)
		top = rc.y;
	if (width == -1)
		width	= rc.width;
	if (height == -1)
		height	= rc.height;
	
	XImage *image = XGetImage(dpy, windowId, left, top, width, height, XAllPlanes(), ZPixmap);
	if (image == NULL) {
		XCloseDisplay(dpy);
		SetX11ErrorHandler();
		return Null;
	}
	
	ImageBuffer ib(width, height);
	
	unsigned long red_mask = image->red_mask;
	unsigned long green_mask = image->green_mask;
	unsigned long blue_mask = image->blue_mask;
	
	for (int y = 0; y < height; y++) {
		RGBA *row = ib[y];
		for (int x = 0; x < width ; x++) {
		 	unsigned long pixel = XGetPixel(image, x, y);	
			unsigned char blue  = pixel & blue_mask;
			unsigned char green = (pixel & green_mask) >> 8;
			unsigned char red   = (pixel & red_mask) >> 16;
			(row + x)->r = red;
			(row + x)->g = green;
			(row + x)->b = blue;
		}
	}
	XCloseDisplay(dpy);
	SetX11ErrorHandler();
	
	Image img;
	img = ib;
	return img;
}

#endif

bool Snap_Desktop(String fileName)
{
	return Window_SaveCapture(0, fileName);
}

bool Snap_DesktopRectangle(String fileName, int left, int top, int width, int height)
{
	return Window_SaveCapture(0, fileName, left, top, width, height);
}

bool Snap_Window(String fileName, int64 handle)
{
	return Window_SaveCapture(handle, fileName);
}

Image Snap_Desktop()
{
	return Window_SaveCapture(0);
}

Image Snap_DesktopRectangle(int left, int top, int width, int height)
{
	return Window_SaveCapture(0, left, top, width, height);
}

Image Snap_Window(int64 handle)
{
	return Window_SaveCapture(handle);
}

END_UPP_NAMESPACE