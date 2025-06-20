#include "CtrlCore.h"

#ifdef GUI_WIN

namespace Upp {

#define LLOG(x) // LOG(x)
#define LTIMING(x) // RTIMING(x)

static COLORREF sLightGray;

Rect SystemDraw::GetVirtualScreenArea()
{
	GuiLock __;
	return RectC(GetSystemMetrics(SM_XVIRTUALSCREEN),
		GetSystemMetrics(SM_YVIRTUALSCREEN),
		GetSystemMetrics(SM_CXVIRTUALSCREEN),
		GetSystemMetrics(SM_CYVIRTUALSCREEN));
}

dword SystemDraw::GetInfo() const
{
	return DATABANDS|(native || !(style & DOTS) ? style|NATIVE : style);
}

Size SystemDraw::GetPageSize() const
{
	return native && Dots() ? nativeSize : pageSize;
}

Size SystemDraw::GetNativeDpi() const
{
	return nativeDpi;
}

SystemDraw& ScreenInfo()
{
	static ScreenDraw sd(true);
	return sd;
}

HDC ScreenHDC()
{
	static HDC hdc;
	ONCELOCK {
		hdc = CreateDC("DISPLAY", NULL, NULL, NULL);
	}
	return hdc;
}

COLORREF SystemDraw::GetColor(Color c) const {
	COLORREF color = c;
	return color;
}

void SystemDraw::InitColors()
{
}

void SystemDraw::SetColor(Color color)
{
	GuiLock __;
	LLOG("SetColor " << color);
	if(color != lastColor) {
		LLOG("Setting, lastColor:" << FormatIntHex(lastColor.GetRaw())
			<< " color:" << FormatIntHex(color.GetRaw()) <<
		     " GetColor:" << FormatIntHex(GetColor(color)) << " palette:" << palette);
		HBRUSH oldBrush = actBrush;
		HBRUSH h;
		if(!IsNull(color))
			h = (HBRUSH) SelectObject(handle, actBrush = CreateSolidBrush(GetColor(color)));
		else {
			HGDIOBJ empty = GetStockObject(HOLLOW_BRUSH);
			h = (HBRUSH) SelectObject(handle, empty);
			actBrush = NULL;
		}
		ASSERT(h);
		if(!orgBrush) orgBrush = h;
		if(oldBrush) DeleteObject(oldBrush);
		lastColor = color;
	}
}

void SystemDraw::SetDrawPen(int width, Color color) {
	GuiLock __;
	if(IsNull(width))
		width = PEN_NULL;
	if(width != lastPen || color != lastPenColor) {
		static int penstyle[] = {
			PS_NULL, PS_SOLID, PS_DASH,
		#ifndef PLATFORM_WINCE
			PS_DOT, PS_DASHDOT, PS_DASHDOTDOT
		#endif
		};
		HPEN oldPen = actPen;
		actPen = CreatePen(width < 0 ? penstyle[-width - 1] : PS_SOLID,
			               width < 0 ? 0 : width, GetColor(color));
		HPEN h = (HPEN) SelectObject(handle, actPen);
		if(!orgPen) orgPen = h;
		if(oldPen) DeleteObject(oldPen);
		lastPen = width;
		lastPenColor = color;
	}
}

void SystemDraw::SetOrg() {
	GuiLock __;
#ifdef PLATFORM_WINCE
	::SetViewportOrgEx(handle, actual_offset.x, actual_offset.y, 0);
#else
	LLOG("SystemDraw::SetOrg: clip = " << GetClip() << ", offset = " << actual_offset);
	::SetWindowOrgEx(handle, -actual_offset.x, -actual_offset.y, 0);
	LLOG("//SystemDraw::SetOrg: clip = " << GetClip());
#endif
}

#ifndef PLATFORM_WINCE
Point SystemDraw::LPtoDP(Point p) const {
	GuiLock __;
	::LPtoDP(handle, p, 1);
	return p;
}

Point SystemDraw::DPtoLP(Point p) const {
	GuiLock __;
	::DPtoLP(handle, p, 1);
	return p;
}

Rect  SystemDraw::LPtoDP(const Rect& r) const {
	GuiLock __;
	Rect w = r;
	::LPtoDP(handle, reinterpret_cast<POINT *>(&w), 2);
	return w;
}

Rect  SystemDraw::DPtoLP(const Rect& r) const {
	GuiLock __;
	Rect w = r;
	::LPtoDP(handle, reinterpret_cast<POINT *>(&w), 2);
	return w;
}
#endif

Size SystemDraw::GetSizeCaps(int i, int j) const {
	GuiLock __;
	return Size(GetDeviceCaps(handle, i), GetDeviceCaps(handle, j));
}

void SystemDraw::DotsMode()
{
	::SetMapMode(handle, MM_ANISOTROPIC);
	::SetViewportExtEx(handle, nativeDpi.cx, nativeDpi.cy, NULL);
	::SetViewportOrgEx(handle, 0, 0, NULL);
	::SetWindowExtEx(handle, 600, 600, NULL);
	::SetWindowOrgEx(handle, 0, 0, NULL);
}

void SystemDraw::BeginNative()
{
	if(GetPixelsPerInch() != nativeDpi && ++native == 1) {
		::SetMapMode(handle, MM_TEXT);
		actual_offset_bak = actual_offset;
		Native(actual_offset);
		SetOrg();
	}
}

void SystemDraw::EndNative()
{
	if(GetPixelsPerInch() == nativeDpi && --native == 0) {
		DotsMode();
		actual_offset = actual_offset_bak;
		SetOrg();
	}
}

int SystemDraw::GetCloffLevel() const
{
	return cloff.GetCount();
}

void SystemDraw::LoadCaps() {
	GuiLock __;
	color16 = false;
	palette = (GetDeviceCaps(handle, RASTERCAPS) & RC_PALETTE);
	if(palette)
		color16 = GetDeviceCaps(handle, SIZEPALETTE) != 256;
	nativeSize = pageSize = GetSizeCaps(HORZRES, VERTRES);
	nativeDpi = GetSizeCaps(LOGPIXELSX, LOGPIXELSY);
	is_mono = GetDeviceCaps(handle, BITSPIXEL) == 1 && GetDeviceCaps(handle, PLANES) == 1;
}

void SystemDraw::Cinit() {
	GuiLock __;
	lastColor = Color::FromCR(COLORREF(-5));
	lastPenColor = Color::FromCR(COLORREF(-5));
	lastTextColor = COLORREF(-1);
	lastPen = Null;
	actBrush = orgBrush = NULL;
	actPen = orgPen = NULL;
}

void SystemDraw::Init() {
	GuiLock __;
	drawingclip = Rect(-(INT_MAX >> 1), -(INT_MAX >> 1), +(INT_MAX >> 1), +(INT_MAX >> 1));
	Cinit();
	SetBkMode(handle, TRANSPARENT);
	::SetTextAlign(handle, TA_BASELINE);
#ifdef PLATFORM_WINCE
	actual_offset = Point(0, 0);
#else
	::GetViewportOrgEx(handle, actual_offset);
#endif
	LoadCaps();
}

void SystemDraw::InitClip(const Rect& clip)
{
	drawingclip = clip;
}

void SystemDraw::Reset() {
	GuiLock __;
	style = 0;
}

SystemDraw::SystemDraw() {
	GuiLock __;
	native = 0;
	InitColors();
	actual_offset = Point(0, 0);
	Reset();
	handle = NULL;
	dcMem = NULL;
}

SystemDraw::SystemDraw(HDC hdc) {
	GuiLock __;
	native = 0;
	InitColors();
	Reset();
	Attach(hdc);
}

void SystemDraw::Unselect0() {
	GuiLock __;
	if(orgPen) SelectObject(handle, orgPen);
	if(orgBrush) SelectObject(handle, orgBrush);
	if(actPen) DeleteObject(actPen);
	if(actBrush) DeleteObject(actBrush);
	Cinit();
}

void SystemDraw::Unselect() {
	GuiLock __;
	while(cloff.GetCount())
		End();
	Unselect0();
}

void  SystemDraw::Attach(HDC ahandle)
{
	handle = ahandle;
	dcMem = ::CreateCompatibleDC(handle);
	Init();
}

HDC   SystemDraw::Detach()
{
	Unselect();
	HDC h = handle;
	handle = NULL;
	::DeleteDC(dcMem);
	dcMem = NULL;
	return h;
}

SystemDraw::~SystemDraw() {
	GuiLock __;
	Detach();
}

HDC SystemDraw::BeginGdi() {
	GuiLock __;
	Begin();
	return handle;
}

void SystemDraw::EndGdi() {
	GuiLock __;
	Unselect0();
	End();
}

void BackDraw::Create(SystemDraw& w, int cx, int cy) {
	ASSERT(w.GetHandle());
	GuiLock __;
	Destroy();
	size.cx = cx;
	size.cy = cy;
	hbmp = ::CreateCompatibleBitmap(w.GetHandle(), cx, cy);
	handle = ::CreateCompatibleDC(w.GetHandle());
	dcMem = ::CreateCompatibleDC(handle);
	ASSERT(hbmp);
	ASSERT(handle);
	hbmpold = (HBITMAP) ::SelectObject(handle, hbmp);
	Init();
	InitClip(size);
}

void BackDraw::Put(SystemDraw& w, int x, int y) {
	GuiLock __;
	ASSERT(handle);
	LTIMING("BackDraw::Put");
#ifdef PLATFORM_WINCE
	::SetViewportOrgEx(handle, 0, 0, 0);
#else
	::SetWindowOrgEx(handle, 0, 0, NULL);
#endif
	::BitBlt(w, x, y, size.cx, size.cy, *this, 0, 0, SRCCOPY);
}

void BackDraw::Destroy() {
	GuiLock __;
	if(handle) {
		Unselect();
		::SelectObject(handle, hbmpold);
		::DeleteDC(handle);
		::DeleteObject(hbmp);
		handle = NULL;
		::DeleteDC(dcMem);
		dcMem = NULL;
	}
}

ScreenDraw::ScreenDraw(bool ic) {
	GuiLock __;
	Attach(ic ? CreateIC("DISPLAY", NULL, NULL, NULL) : CreateDC("DISPLAY", NULL, NULL, NULL));
	InitClip(GetVirtualScreenArea());
}

ScreenDraw::~ScreenDraw() {
	GuiLock __;
	Unselect();
	DeleteDC(handle);
}

#ifndef PLATFORM_WINCE

void PrintDraw::InitPrinter()
{
	GuiLock __;
	Init();
	style = PRINTER|DOTS;
	DotsMode();
	native = 0;
	actual_offset = Point(0, 0);
	aborted = false;
	pageSize.cx = 600 * nativeSize.cx / nativeDpi.cx; 
	pageSize.cy = 600 * nativeSize.cy / nativeDpi.cy;
	InitClip(pageSize);
}

void PrintDraw::StartPage()
{
	GuiLock __;
	if(aborted) return;
	Unselect();
	if(::StartPage(handle) <= 0)
		aborted = true;
	else
		InitPrinter();
}

void PrintDraw::EndPage()
{
	GuiLock __;
	if(aborted) return;
	Unselect();
	if(::EndPage(handle) <= 0)
		aborted = true;
}

PrintDraw::PrintDraw(HDC hdc, const char *docname)
   : SystemDraw(hdc)
{
	GuiLock __;
	DOCINFOW di;
	memset(&di, 0, sizeof(di));
	di.cbSize = sizeof(di);
	Vector<char16> sys_docname = ToSystemCharsetW(docname);
	di.lpszDocName = sys_docname;
	if(::StartDocW(hdc, &di) <= 0)
		aborted = true;
	else
		InitPrinter();
}

PrintDraw::~PrintDraw() {
	GuiLock __;
	if(aborted)
		::AbortDoc(handle);
	else
		::EndDoc(handle);
}
#endif

}

#endif
