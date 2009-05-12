#include "Draw.h"

NAMESPACE_UPP

#define LLOG(x) // LOG(x)
#define LTIMING(x) // RTIMING(x)

#ifdef PLATFORM_WIN32

static COLORREF sLightGray;

dword SystemDraw::GetInfo() const
{
	return native || !(style & DOTS) ? style|NATIVE : style;
}

Size SystemDraw::GetPageSize() const
{
	return native && Dots() ? nativeSize : pageSize;
}

Size SystemDraw::GetNativeDpi() const
{
	return Dots() ? nativeDpi : Size(96, 96);
}

void StaticExitDraw_()
{
	FontInfo::FreeFonts();
}

EXITBLOCK
{
	StaticExitDraw_();
}

#ifndef PLATFORM_WINCE
void Add(LOGPALETTE *pal, int r, int g, int b)
{
	pal->palPalEntry[pal->palNumEntries].peRed   = min(r, 255);
	pal->palPalEntry[pal->palNumEntries].peGreen = min(g, 255);
	pal->palPalEntry[pal->palNumEntries].peBlue  = min(b, 255);
	pal->palPalEntry[pal->palNumEntries++].peFlags = PC_NOCOLLAPSE;
}

HPALETTE GetQlibPalette()
{
	static HPALETTE hQlibPalette;
	if(hQlibPalette) return hQlibPalette;
	SystemDraw::InitColors();
	LOGPALETTE *pal = (LOGPALETTE *) new byte[sizeof(LOGPALETTE) + 256 * sizeof(PALETTEENTRY)];
	pal->palNumEntries = 0;
	pal->palVersion    = 0x300;
	for(int r = 0; r < 6; r++)
		for(int g = 0; g < 6; g++)
			for(int b = 0; b < 6; b++)
				Add(pal, 255 * r / 5, 255 * g / 5, 255 * b / 5);
	for(int q = 0; q <= 16; q++)
		Add(pal, 16 * q, 16 * q, 16 * q);
	Add(pal, GetRValue(sLightGray), GetGValue(sLightGray), GetBValue(sLightGray));
	hQlibPalette = CreatePalette(pal);
	delete[] pal;
	return hQlibPalette;
}
#endif

SystemDraw& GLOBAL_VP(ScreenDraw, ScreenInfo, (true))

HDC ScreenHDC()
{
	static HDC hdc;
	ONCELOCK {
		hdc = CreateDC("DISPLAY", NULL, NULL, NULL);
	}
	return hdc;
}

static bool _AutoPalette = true;
bool SystemDraw::AutoPalette() { return _AutoPalette; }
void SystemDraw::SetAutoPalette(bool ap) { _AutoPalette = ap; }

COLORREF SystemDraw::GetColor(Color c) const {
	COLORREF color = c;
#ifdef PLATFORM_WINCE
	return color;
#else
	if(!palette)
		return color;
	static Index<dword> *SColor;
	ONCELOCK {
		static Index<dword> StaticColor;
		StaticColor << RGB(0x00, 0x00, 0x00) << RGB(0x80, 0x00, 0x00) << RGB(0x00, 0x80, 0x00)
					<< RGB(0x80, 0x80, 0x00) << RGB(0x00, 0x00, 0x80) << RGB(0x80, 0x00, 0x80)
					<< RGB(0x00, 0x80, 0x80) << RGB(0xC0, 0xC0, 0xC0) << RGB(0xC0, 0xDC, 0xC0)
					<< RGB(0xA6, 0xCA, 0xF0) << RGB(0xFF, 0xFB, 0xF0) << RGB(0xA0, 0xA0, 0xA4)
					<< RGB(0x80, 0x80, 0x80) << RGB(0xFF, 0x00, 0x00) << RGB(0x00, 0xFF, 0x00)
					<< RGB(0xFF, 0xFF, 0x00) << RGB(0x00, 0x00, 0xFF) << RGB(0xFF, 0x00, 0xFF)
					<< RGB(0x00, 0xFF, 0xFF) << RGB(0xFF, 0xFF, 0xFF);
		SColor = &StaticColor;
	}
	if(color16 || !AutoPalette())
		return GetNearestColor(handle, color);
	if(SColor->Find(color) >= 0)
		return color;
	if(color == sLightGray)
		return PALETTEINDEX(216 + 17);
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);
	return PALETTEINDEX(r == g && g == b ? (r + 8) / 16 + 216
		                                 : (r + 25) / 51 * 36 +
		                                   (g + 25) / 51 * 6 +
		                                   (b + 25) / 51);
#endif
}

void SystemDraw::InitColors()
{
}

void SystemDraw::SetColor(Color color)
{
	DrawLock __;
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
	DrawLock __;
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

void SystemDraw::SetFont(Font font, int angle) {
	DrawLock __;
	LLOG("Set font: " << font << " face: " << font.GetFaceName());
	if(lastFont && lastFont.IsEqual(CHARSET_UNICODE, font, angle))
		return;
	lastFont = FontInfo::AcquireFontInfo0(font, GetHandle(), angle);
	HFONT h = (HFONT) SelectObject(handle, lastFont.ptr->hfont);
	if(!orgFont) orgFont = h;
}

void SystemDraw::SetOrg() {
	DrawLock __;
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
	DrawLock __;
	::LPtoDP(handle, p, 1);
	return p;
}

Point SystemDraw::DPtoLP(Point p) const {
	DrawLock __;
	::DPtoLP(handle, p, 1);
	return p;
}

Rect  SystemDraw::LPtoDP(const Rect& r) const {
	DrawLock __;
	Rect w = r;
	::LPtoDP(handle, reinterpret_cast<POINT *>(&w), 2);
	return w;
}

Rect  SystemDraw::DPtoLP(const Rect& r) const {
	DrawLock __;
	Rect w = r;
	::LPtoDP(handle, reinterpret_cast<POINT *>(&w), 2);
	return w;
}
#endif

Size SystemDraw::GetSizeCaps(int i, int j) const {
	DrawLock __;
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
	if(GetPixelsPerInch() != nativeDpi && --native == 0) {
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
	DrawLock __;
	color16 = false;
	palette = (GetDeviceCaps(handle, RASTERCAPS) & RC_PALETTE);
	if(palette)
		color16 = GetDeviceCaps(handle, SIZEPALETTE) != 256;
	nativeSize = pageSize = GetSizeCaps(HORZRES, VERTRES);
	nativeDpi = GetSizeCaps(LOGPIXELSX, LOGPIXELSY);
	is_mono = GetDeviceCaps(handle, BITSPIXEL) == 1 && GetDeviceCaps(handle, PLANES) == 1;
}

void SystemDraw::Cinit() {
	DrawLock __;
	lastColor = Color::FromCR(COLORREF(-5));
	lastPenColor = Color::FromCR(COLORREF(-5));
	lastTextColor = COLORREF(-1);
	lastPen = Null;
	actBrush = orgBrush = NULL;
	actPen = orgPen = NULL;
	orgFont = NULL;
	lastFont.Clear();
}

void SystemDraw::Init() {
	DrawLock __;
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

void SystemDraw::Reset() {
	DrawLock __;
	style = GUI;
}

SystemDraw::SystemDraw() {
	DrawLock __;
	native = 0;
	InitColors();
	FontInfo::InitFonts();
	actual_offset = Point(0, 0);
	Reset();
	handle = NULL;
}

SystemDraw::SystemDraw(HDC hdc) {
	DrawLock __;
	native = 0;
	InitColors();
	FontInfo::InitFonts();
	Reset();
	Attach(hdc);
}

void SystemDraw::Unselect0() {
	DrawLock __;
	if(orgPen) SelectObject(handle, orgPen);
	if(orgBrush) SelectObject(handle, orgBrush);
	if(orgFont) SelectObject(handle, orgFont);
	if(actPen) DeleteObject(actPen);
	if(actBrush) DeleteObject(actBrush);
	Cinit();
}

void SystemDraw::Unselect() {
	DrawLock __;
	while(cloff.GetCount())
		End();
	Unselect0();
}

SystemDraw::~SystemDraw() {
	DrawLock __;
	if(handle)
		Unselect();
}

HDC SystemDraw::BeginGdi() {
	DrawLock __;
	Begin();
	return handle;
}

void SystemDraw::EndGdi() {
	DrawLock __;
	Unselect0();
	End();
}

void BackDraw::Create(SystemDraw& w, int cx, int cy) {
	ASSERT(w.GetHandle());
	DrawLock __;
	Destroy();
	size.cx = cx;
	size.cy = cy;
	hbmp = ::CreateCompatibleBitmap(w.GetHandle(), cx, cy);
	handle = ::CreateCompatibleDC(w.GetHandle());
	ASSERT(hbmp);
	ASSERT(handle);
#ifndef PLATFORM_WINCE
	if(AutoPalette()) {
		::SelectPalette(handle, GetQlibPalette(), FALSE);
		::RealizePalette(handle);
	}
#endif
	hbmpold = (HBITMAP) ::SelectObject(handle, hbmp);
	Init();
	style = GUI|BACK;
}

void BackDraw::Put(SystemDraw& w, int x, int y) {
	DrawLock __;
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
	DrawLock __;
	if(handle) {
		Unselect();
		::SelectObject(handle, hbmpold);
		::DeleteDC(handle);
		::DeleteObject(hbmp);
		handle = NULL;
	}
}

ScreenDraw::ScreenDraw(bool ic) {
	DrawLock __;
#ifdef PLATFORM_WINCE
	Attach(CreateDC(NULL, NULL, NULL, NULL));
#else
	Attach(ic ? CreateIC("DISPLAY", NULL, NULL, NULL) : CreateDC("DISPLAY", NULL, NULL, NULL));
	if(AutoPalette()) {
		SelectPalette(handle, GetQlibPalette(), TRUE);
		RealizePalette(handle);
	}
#endif
}

ScreenDraw::~ScreenDraw() {
	DrawLock __;
	Unselect();
	DeleteDC(handle);
}

#ifndef PLATFORM_WINCE

void PrintDraw::InitPrinter()
{
	DrawLock __;
	Init();
	style = PRINTER|DOTS;
	DotsMode();
	native = 0;
	actual_offset = Point(0, 0);
	aborted = false;
	pageSize.cx = 600 * nativeSize.cx / nativeDpi.cx; 
	pageSize.cy = 600 * nativeSize.cy / nativeDpi.cy; 
}

void PrintDraw::StartPage()
{
	DrawLock __;
	if(aborted) return;
	Unselect();
	if(::StartPage(handle) <= 0)
		aborted = true;
	else
		InitPrinter();
}

void PrintDraw::EndPage()
{
	DrawLock __;
	if(aborted) return;
	Unselect();
	if(::EndPage(handle) <= 0)
		aborted = true;
}

PrintDraw::PrintDraw(HDC hdc, const char *docname)
   : SystemDraw(hdc)
{
	DrawLock __;
	DOCINFO di;
	memset(&di, 0, sizeof(di));
	di.cbSize = sizeof(di);
	String sys_docname = ToSystemCharset(docname);
	di.lpszDocName = ~sys_docname;
	if(::StartDoc(hdc, &di) <= 0)
		aborted = true;
	else
		InitPrinter();
}

PrintDraw::~PrintDraw() {
	DrawLock __;
	if(aborted)
		::AbortDoc(handle);
	else
		::EndDoc(handle);
	DeleteDC(handle);
	handle = NULL;
}
#endif


bool ScreenInPaletteMode()
{
	return ScreenInfo().PaletteMode();
}

Size GetScreenSize()
{
	return ScreenInfo().GetPageSize();
}

#endif

END_UPP_NAMESPACE
